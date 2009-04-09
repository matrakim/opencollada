/*
    Copyright (c) 2008 NetAllied Systems GmbH

	This file is part of COLLADAMaya.

    Portions of the code are:
    Copyright (c) 2005-2007 Feeling Software Inc.
    Copyright (c) 2005-2007 Sony Computer Entertainment America
    Copyright (c) 2004-2005 Alias Systems Corp.
	
    Licensed under the MIT Open Source License, 
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/

#include "COLLADAMayaStableHeaders.h"
#include "COLLADAMayaSceneGraph.h"
#include "COLLADAMayaEffectExporter.h"
#include "COLLADAMayaLightExporter.h"
#include "COLLADAMayaExportOptions.h"
#include "COLLADAMayaAnimationExporter.h"

#include <maya/MFnLight.h>
#include <maya/MFnNonAmbientLight.h>
#include <maya/MFnAmbientLight.h>
#include <maya/MFnSpotLight.h>


namespace COLLADAMaya
{

    //---------------------------------------------------------------
    LightExporter::LightExporter ( 
        COLLADASW::StreamWriter* streamWriter,
        DocumentExporter* documentExporter )
    : COLLADASW::LibraryLights ( streamWriter )
    , mDocumentExporter ( documentExporter )
    {}


    //---------------------------------------------------------------
    void LightExporter::exportLights ( )
    {
        if ( !ExportOptions::exportLights() ) return;

        // Get the list with the transform nodes.
        SceneGraph* sceneGraph = mDocumentExporter->getSceneGraph();
        SceneElementsList* exportNodesTree = sceneGraph->getExportNodesTree();

        // Export all/selected DAG nodes
        size_t length = exportNodesTree->size();
        for ( uint i = 0; i < length; ++i )
        {
            SceneElement* sceneElement = ( *exportNodesTree ) [i];
            exportLights ( sceneElement );
        }

        closeLibrary();
    }

    //---------------------------------------------------------------
    void LightExporter::exportLights ( SceneElement* sceneElement )
    {
        // If we have a external reference, we don't need to export the data here.
        if ( !sceneElement->getIsLocal() ) return;

        // Get the current dag path
        MDagPath dagPath = sceneElement->getPath();

        // Check for instance
        bool isInstance = ( dagPath.isInstanced() && dagPath.instanceNumber() > 0 );

        // Check if it is a mesh and an export node
        if ( sceneElement->getType() == SceneElement::LIGHT &&
             sceneElement->getIsExportNode() && !isInstance )
        {
            // Export the geometry 
            bool exported = exportLight ( dagPath );

            // Push it in the list of exported elements.
            if ( exported )
                mDocumentExporter->getSceneGraph()->addExportedElement( sceneElement );
        }


        // Recursive call for all the child elements
        for ( uint i=0; i<sceneElement->getChildCount(); ++i )
        {
            SceneElement* childElement = sceneElement->getChild ( i );
            exportLights ( childElement );
        }
    }

    // --------------------------------------------------------
    bool LightExporter::exportLight ( const MDagPath& dagPath )
    {
        if ( !ExportOptions::exportLights() ) return false;

        MObject lightNode = dagPath.node();

        // Retrieve the Maya light object
        MStatus status;
        MFnLight lightFn(lightNode, &status); CHECK_STAT(status);
        if (status != MStatus::kSuccess) return false;

        // Generate a COLLADA id for the new light object
        String lightId = mDocumentExporter->dagPathToColladaId ( dagPath );

        // Get a pointer to the stream writer.
        COLLADASW::StreamWriter* streamWriter = mDocumentExporter->getStreamWriter();

        // Figure out the type of light and create it
        COLLADASW::Light* light = NULL;
        MFn::Type type = lightNode.apiType();
        switch (type)
        {
        case MFn::kAmbientLight: 
            light = new COLLADASW::AmbientLight( streamWriter, lightId ); 
            break; 
        case MFn::kDirectionalLight: 
            light = new COLLADASW::DirectionalLight( streamWriter, lightId ); 
            break;
        case MFn::kSpotLight: 
            light = new COLLADASW::SpotLight( streamWriter, lightId ); 
            break;
        case MFn::kPointLight: // Intentional pass-through
        default: 
            light = new COLLADASW::PointLight( streamWriter, lightId ); 
            break;
        }

        // The light name
        String lightName = mDocumentExporter->dagPathToColladaName ( dagPath );
        light->setLightName( lightName );

        // Get a pointer to the animation exporter.
        AnimationExporter* anim = mDocumentExporter->getAnimationExporter();
        bool animated = false;
        
        // Color/Intensity are the common attributes of all lights
        MColor mayaColor = lightFn.color ( &status ); CHECK_STAT(status);
        COLLADASW::Color lightColor ( mayaColor.r, mayaColor.g, mayaColor.b, mayaColor.a );
        animated = anim->addNodeAnimation ( lightNode, ATTR_COLOR, kColour );
        light->setColor( lightColor, animated );

        float intensity = lightFn.intensity ( &status ); CHECK_STAT(status);
        animated = anim->addNodeAnimation ( lightNode, ATTR_INTENSITY, kSingle );
        light->setIntensity( intensity, animated );

        // Add the type specific attributes
        if (lightNode.hasFn(MFn::kNonAmbientLight))
        {
            // Needed Point and Spot light types parameters: Attenuation and Attenuation_Scale
            // Attenuation in COLLADA is equal to Decay in Maya.
            MFnNonAmbientLight naLightFn(lightNode);
            int decayRate = naLightFn.decayRate(&status); CHECK_STAT(status);
            decayRate = std::min ( decayRate, 2 ); 
            decayRate = std::max ( decayRate, 0 );

            light->setConstantAttenuation ( ( decayRate == 0 ) ? 1.0f : 0.0f);
            light->setLinearAttenuation ( ( decayRate == 1 ) ? 1.0f : 0.0f);
            light->setQuadraticAttenuation ( ( decayRate == 2 ) ? 1.0f : 0.0f);
        }
        else if (lightNode.hasFn(MFn::kAmbientLight))
        {
            MFnAmbientLight ambientLightFn ( lightNode );
            float ambientShade = ambientLightFn.ambientShade();
            String paramSid = "";
            animated = anim->addNodeAnimation ( lightNode, ATTR_AMBIENT_SHADE, kSingle );
            if ( animated ) paramSid = ATTR_AMBIENT_SHADE;
            light->addExtraTechniqueParameter ( 
                COLLADASW::CSWC::CSW_PROFILE_MAYA, MAYA_AMBIENTSHADE_LIGHT_PARAMETER, ambientShade, paramSid );
        }

        if (lightNode.hasFn(MFn::kSpotLight))
        {
            // Put in the needed spot light type attributes : Falloff, Falloff_Scale and Angle
            MFnSpotLight spotFn(lightNode);

            float fallOffAngle = COLLADABU::Math::Utils::radToDegF ( (float)spotFn.coneAngle( &status ) ); CHECK_STAT(status);
            animated = anim->addNodeAnimation ( lightNode, ATTR_CONE_ANGLE, ( SampleType ) ( kSingle | kAngle ) );
            light->setFallOffAngle ( fallOffAngle, animated );

            light->setFallOffExponent ( 1.0f );

            float penumbraValue = COLLADABU::Math::Utils::radToDegF ( (float)spotFn.penumbraAngle( &status ) ); CHECK_STAT(status);
            animated = anim->addNodeAnimation ( lightNode, ATTR_PENUMBRA_ANGLE, ( SampleType ) ( kSingle | kAngle ) );
            // TODO
//            FCDLightTools::LoadPenumbra(light, penumbraValue, colladaLight->GetOuterAngle().GetAnimated());

            // TODO
//             animated = anim->addNodeAnimation ( lightNode, ATTR_DROP_OFF, kSingle );
//             light->setDropOff ( (float) spotFn.dropOff ( &status ), animated ); CHECK_MSTATUS(status);
        }
        
        addLight ( *light );

        return true;
    }

}