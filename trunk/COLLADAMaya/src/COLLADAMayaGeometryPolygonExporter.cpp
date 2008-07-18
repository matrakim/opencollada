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
#include "COLLADAMayaGeometryPolygonExporter.h"
#include "COLLADAMayaExportOptions.h"
#include "COLLADAMayaSyntax.h"
#include "COLLADAMayaSceneGraph.h"
#include <algorithm>
#include <assert.h>
#include <time.h>

#include <maya/MItDependencyNodes.h>
#include <maya/MFnMesh.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>
#include <maya/MItDag.h>

#include "COLLADASource.h"
#include "COLLADABaseInputElement.h"
#include "COLLADAInputList.h"
#include "COLLADAExtraTechnique.h"


namespace COLLADAMaya
{

    // --------------------------------------------------------
    GeometryPolygonExporter::GeometryPolygonExporter ( COLLADA::StreamWriter* _streamWriter,
        DocumentExporter* _documentExporter )
        : COLLADA::LibraryGeometries ( _streamWriter )
        , mDocumentExporter ( _documentExporter )
        , mPolygonSources ( NULL )
        , mVertexSources ( NULL )
        , triangulated ( false )
        , holeCount ( 0 )
    {
    }

    // --------------------------------------------------------
    GeometryPolygonExporter::~GeometryPolygonExporter ( void ) {}

    // -----------------------------------------------------------------------------------
    void GeometryPolygonExporter::exportPolygonSources ( 
        MFnMesh &fnMesh,
        String meshId,
        MStringArray uvSetNames,
        ColourSetList &colorSets,
        Sources* polygonSources,
        Sources* vertexSources,
        bool hasFaceVertexNorms )
    {
        // Initialize the members
        mMeshId = meshId;
        mUvSetNames = uvSetNames;
        mPolygonSources = polygonSources;
        mVertexSources = vertexSources;
        mHasFaceVertexNormals = hasFaceVertexNorms;
        mColorSets = colorSets;

        // If triangulation is requested, verify that it is feasible by checking with all the mesh polygons
        if ( ExportOptions::exportTriangles() )
        {
            triangulated = true;

            for ( MItMeshPolygon polyIt ( fnMesh.object() ); triangulated && !polyIt.isDone(); polyIt.next() )
            {
                triangulated = polyIt.hasValidTriangulation();
            }
        }

        // If we have a hole in a polygon, we can't write a <polylist>.
        // Either we write <polygons> with holes or we write triangles.
        // Get hole information from the mesh node.
        // The format for the holes information is explained in the MFnMesh documentation.
        MStatus status;
        fnMesh.getHoles ( mHoleInfoArray, mHoleVertexArray, &status );
        holeCount = ( status != MStatus::kSuccess ) ? 0 : ( mHoleInfoArray.length() / 3 );

        // Find how many shaders are used by this instance of the mesh
        MObjectArray shaders;
        MIntArray shaderIndices;
        fnMesh.getConnectedShaders ( 0, shaders, shaderIndices );

        // Find the polygons that correspond to each materials and export them
        uint realShaderCount = ( uint ) shaders.length();
        uint numShaders = ( uint ) max ( ( size_t ) 1, ( size_t ) shaders.length() );

        for ( uint shaderPosition = 0; shaderPosition<numShaders; ++shaderPosition )
        {
            // Export the polygons of the current shader
            exportShaderPolygons ( fnMesh,
                shaderPosition,
                realShaderCount,
                shaders,
                shaderIndices );
        }
    }

    //---------------------------------------------------------------
    void GeometryPolygonExporter::exportShaderPolygons ( 
        MFnMesh &fnMesh,
        uint shaderPosition,
        uint realShaderCount,
        MObjectArray shaders,
        const MIntArray &shaderIndices )
    {
//         // The list with the shader polygons with the vertex indexes
//         PolygonSourceList shaderPolygons;

        // Just create a polylist, if there are polygons to export
        // If we have holes in the polygon, we have to use <polygons> instead of <polylist>.
        // If we want to export as triangles, we have to use <triangles>.
        COLLADA::PrimitivesBase dummyPrimitivesBase ( mSW );

        // Get the polygons count.
        uint numPolygons = getShaderPolygonsCount(  fnMesh, shaderPosition, realShaderCount, shaderIndices );

//         uint numPolygons = createShaderPolygons(
//             fnMesh, shaderPosition, realShaderCount, shaderIndices, 
//             &dummyPrimitivesBase, shaderPolygons, currentShapeIsHoled);

        // Just create a polylist, if there are polygons to export
        // If we have holes in the polygon, we have to use <polygons> instead of <polylist>.
        // If we want to export as triangles, we have to use <triangles>.
        COLLADA::PrimitivesBase* primitivesBasePoly = NULL;

        // Just create the polylist, if there are polygons in the shader to export
        if ( numPolygons > 0 )
        {
            // Retrieves the information, if we have a hole in any polygon under the current shape
            // (then we have to create a polygon element, instead of a polylist element).
            bool isHoledShape = verifyPolygonsForHoles ( fnMesh, shaderPosition, realShaderCount, shaderIndices );

            // Determine the export type (polylist/polygon/triangles)
            uint exportType = determinePrimitivesBaseExportType ( isHoledShape );

            // The list for the vertex counts
            std::vector<unsigned long> vertexCountList;

            // Retrieve the vertex count list for the polylist element.
            if ( exportType == POLYLIST )
            {
                // Retrieves the vertex count list to export.
                getVertexCountList(
                    fnMesh, shaderPosition, realShaderCount, shaderIndices, vertexCountList );
            }

            // Create the polylist/polygon/triangles element and set the 
            // materials and the vertices count list before writing the 
            // list of the polylist/polygon/triangles vertices.
            primitivesBasePoly = preparePrimitivesBase( 
                dummyPrimitivesBase, numPolygons, vertexCountList, 
                shaderPosition, realShaderCount, exportType, shaders);

            // Retrieve the shader polygon vertices and write them directly into the collada file.
            writeShaderPolygons( 
                primitivesBasePoly, exportType, 
                fnMesh, shaderIndices, shaderPosition, realShaderCount );

//             primitivesBasePoly = preparePrimitivesBasePoly( 
//                 dummyPrimitivesBase, numPolygons, shaderPolygons, 
//                 shaderPosition, realShaderCount, exportType, shaders);
// 
//             // Iterate through the list of polygons and 
//             // write them into the collada file.
//             writeShaderPolygons(shaderPolygons, primitivesBasePoly, exportType);
        }

        // Delete the created primitivesBasePoly
        if ( primitivesBasePoly != NULL )
        {
            delete primitivesBasePoly;
            primitivesBasePoly = NULL;
        }

//         // Delete the polygons in the list
//         for ( uint p=0; p<shaderPolygons.size(); ++p )
//         {
//             PolygonSource* polygon = shaderPolygons[p];
//             delete polygon;
//             polygon = NULL;
//         }
//         shaderPolygons.clear();
    }

    // ----------------------------------------
    void GeometryPolygonExporter::writeShaderPolygons( 
        COLLADA::PrimitivesBase* primitivesBasePoly,
        const uint exportType,
        MFnMesh &fnMesh, 
        const MIntArray &shaderIndices, 
        const uint shaderPosition, 
        const uint realShaderCount )
    {
        // Number of polygons (could also be triangles)
        uint numPolygons = 0; 

        // List for the polygon set inputs.
        Sources polygonSetInputs;

        // Generate the polygon set inputs.
        generatePolygonSetInputs ( primitivesBasePoly, &polygonSetInputs );

        // Iterate through all polygons of the current mesh and create them to export
        MItMeshPolygon meshPolygonsIter ( fnMesh.object() );
        for ( meshPolygonsIter.reset(); !meshPolygonsIter.isDone(); meshPolygonsIter.next() )
        {
            // Is this polygon shaded by this shader?
            int polyIndex = meshPolygonsIter.index();
            if ( shaderPosition < realShaderCount && 
                ( uint ) shaderIndices[polyIndex] != shaderPosition ) continue;
            if ( shaderPosition >= realShaderCount && 
                ( shaderIndices[polyIndex] >= 0 && 
                shaderIndices[polyIndex] < ( int ) realShaderCount ) ) continue;

            // Create a polygon to store the vertex indexes to export
            PolygonSource polygon ( polygonSetInputs );

            // Create the polygon with the initialization data
            uint numPolygons = initializePolygonSource( &polygon, fnMesh, meshPolygonsIter );

             // If we have polygons to export, push it into the polygon list
            if ( numPolygons > 0 )
            {
                writeElementVertexIndices( 
                    primitivesBasePoly, &polygon, 
                    fnMesh, meshPolygonsIter, exportType );
            }
        }
        
        primitivesBasePoly->finish();
    }

    // --------------------------------------------------------
    uint GeometryPolygonExporter::initializePolygonSource(
        PolygonSource* polygon,
        const MFnMesh &fnMesh, 
        MItMeshPolygon &meshPolygonsIter )
    {
        // Collect data in order to handle triangle-only export option
        uint numPolygons = 0, numVertices = 0;

        // Retrieve the vertex indices and establish the number of polygons (in case of
        // triangulation more than one is possible) and the number of vertexes in the polygon.
        MIntArray vertexIndices;
        retrieveVertexIndices ( 
            &vertexIndices,
            meshPolygonsIter,
            numPolygons,
            numVertices,
            polygon->getVertexCountList() );

        // Iterate through the polygons (normally just one polygon,
        // just in case of triangulation it could be more than one)
        for ( uint polygonPosition=0; polygonPosition<numPolygons; ++polygonPosition )
        {
            // TODO Here i am

            // Put the current face in the list of faces
            polygon->getFaceVertexCounts().push_back ( numVertices );

            // Get the index of the current polygon
            int polyIndex = meshPolygonsIter.index();

            // Iterate through the vertexes of the current polygon
            for ( uint j=0; j<numVertices; j++ )
            {
                // Handle front face vs back face by walking
                // the vertexes backward on the back-face
                int iteratorVertexIndex = vertexIndices[polygonPosition * numVertices + j];

                // Handle front face vs back face by walking the vertices backward on the backface
                int vertexIndex = meshPolygonsIter.vertexIndex ( iteratorVertexIndex );

                // Get the index of the current Polygon
                int polyIndex = meshPolygonsIter.index();

                // Look for holes in this polygon
                // ASSUMPTION: Holes are automatically removed by triangulation.
                // ASSUMPTION: The iterator gives the hole vertices at the end of the enumeration.
                // ASSUMPTION: Hole vertices are never used as surface vertices or repeated between holes or inside a hole.
                if ( meshPolygonsIter.isHoled() && !triangulated )
                {
                    handleHoledPolygon(polygon, polyIndex, vertexIndex, numVertices, iteratorVertexIndex);
                }
            }

        }

        return numPolygons;
    }

    // --------------------------------------------------------
    void GeometryPolygonExporter::writeElementVertexIndices( 
        COLLADA::PrimitivesBase* primitivesBasePoly,
        PolygonSource* polygon, 
        MFnMesh &fnMesh,
        MItMeshPolygon &meshPolygonsIter,
        const uint exportType )
    {
        // Add the open tags for the polygons
        if ( exportType == POLYGONS )
        {
            if ( polygon->isHoled() )
            {
                ( ( COLLADA::Polygons* ) primitivesBasePoly )->openPolylistHole();
            }
        }

        // The face index
        uint currentFaceIndex = 0;

        // Initialize the data for polygons with holes
        uint numFaceVertices = polygon->getFaceVertexCounts().size();
        uint sumOfFaceVertexCounts = polygon->getFaceVertexCounts()[currentFaceIndex];

        // Check if the current face is a normal polygon or a hole and open the corresponding tag.
        if ( exportType == POLYGONS ) 
        {
            openPolygonOrHoleElement ( 
                primitivesBasePoly, 
                polygon,
                currentFaceIndex );
        }

        // Get the index of the current polygon
        int polyIndex = meshPolygonsIter.index();

        // Buffer the face normal indices
        MIntArray normalIndices;
        if ( mHasFaceVertexNormals )
        {
            fnMesh.getFaceNormalIds ( polyIndex, normalIndices );
        }

        // Retrieve the vertex indices and establish the number of polygons (in case of
        // triangulation more than one is possible) and the number of vertexes in the polygon.
        uint numPolygons = 0, numVertices = 0;
        MIntArray vertexIndices;
        retrieveVertexIndices ( 
            &vertexIndices,
            meshPolygonsIter,
            numPolygons,
            numVertices,
            polygon->getVertexCountList() );

        // Iterate through the polygons (normally just one polygon,
        // just in case of triangulation it could be more than one)
        for ( uint polygonPosition=0; polygonPosition<numPolygons; ++polygonPosition )
        {
            // Iterate through the vertexes of the current polygon
            for ( uint vertexPosition=0; vertexPosition<numVertices; ++vertexPosition )
            {
                // If we write a holed polygon and the actual vertex position is the last
                // position of the current face, then go to the next face in the list.
                if ( exportType == POLYGONS &&
                     polygon->isHoled() &&
                     vertexPosition == sumOfFaceVertexCounts )
                {
                    // Check if the current face is a normal polygon or a hole and open the corresponding tag.
                    writePolygonOrHoleElement ( primitivesBasePoly, polygon, sumOfFaceVertexCounts, currentFaceIndex );
                }

                // Handle front face vs back face by walking
                // the vertexes backward on the back-face
                int iteratorVertexIndex = vertexIndices[polygonPosition * numVertices + vertexPosition];

                // Handle front face vs back face by walking the vertices backward on the backface
                int vertexIndex = meshPolygonsIter.vertexIndex ( iteratorVertexIndex );

                // Get the index of the current Polygon
                int polyIndex = meshPolygonsIter.index();

                // Write the vertex indices
                writeVertexIndices(
                    primitivesBasePoly, polygon, vertexIndex, normalIndices, 
                    iteratorVertexIndex, meshPolygonsIter, fnMesh, polyIndex );
            }
        }

        // Add the tags for the polygons
        if ( exportType == POLYGONS )
        {
            if ( polygon->isHoled() ) 
                ( ( COLLADA::Polygons* ) primitivesBasePoly )->closeElement();

            ( ( COLLADA::Polygons* ) primitivesBasePoly )->closeElement();
        }
    }

    // ----------------------------------------
    bool GeometryPolygonExporter::verifyPolygonsForHoles( 
        const MFnMesh &fnMesh,
        const uint shaderPosition, 
        const uint realShaderCount, 
        const MIntArray &shaderIndices )
    {
        // If we want to export triangles, holes aren't of note.
        if ( triangulated ) return false;

        // Iterate through all polygons of the current mesh and 
        // verify their polygons for holes.
        MItMeshPolygon meshPolygonsIter ( fnMesh.object() );
        for ( meshPolygonsIter.reset(); !meshPolygonsIter.isDone(); meshPolygonsIter.next() )
        {
            // Is this polygon shaded by this shader?
            int polyIndex = meshPolygonsIter.index();

            if ( shaderPosition < realShaderCount && 
                ( uint ) shaderIndices[polyIndex] != shaderPosition ) 
                continue;

            if ( shaderPosition >= realShaderCount && 
                ( shaderIndices[polyIndex] >= 0 && 
                shaderIndices[polyIndex] < ( int ) realShaderCount ) ) 
                continue;
            
            // Look for holes in this polygon
            // ASSUMPTION: Holes are automatically removed by triangulation.
            // ASSUMPTION: The iterator gives the hole vertices at the end of the enumeration.
            // ASSUMPTION: Hole vertices are never used as surface vertices or repeated between holes or inside a hole.
            if ( meshPolygonsIter.isHoled() && !triangulated )
            {
                return true;
            }
        }

        return false;
    }

    // ----------------------------------------
    void GeometryPolygonExporter::getVertexCountList(
        const MFnMesh &fnMesh, 
        const uint shaderPosition, 
        const uint realShaderCount, 
        const MIntArray &shaderIndices, 
        std::vector<unsigned long> &vertexCountList )
    {
        // Iterate through all polygons of the current mesh.
        // Check their polygons for holes and retrieve the vertexCountList.
        MItMeshPolygon meshPolygonsIter ( fnMesh.object() );
        for ( meshPolygonsIter.reset(); !meshPolygonsIter.isDone(); meshPolygonsIter.next() )
        {
            // Is this polygon shaded by this shader?
            int polyIndex = meshPolygonsIter.index();

            if ( shaderPosition < realShaderCount && 
                ( uint ) shaderIndices[polyIndex] != shaderPosition ) 
                continue;

            if ( shaderPosition >= realShaderCount && 
                ( shaderIndices[polyIndex] >= 0 && 
                shaderIndices[polyIndex] < ( int ) realShaderCount ) ) 
                continue;

            // Get the polygon count
            getPolygonVertexCounts ( meshPolygonsIter, vertexCountList );

        }
    }

    // ----------------------------------------
    void GeometryPolygonExporter::getPolygonVertexCounts(
        MItMeshPolygon &meshPolygonsIter,
        std::vector<unsigned long> &vertexCountList )
    {
        // Establish the number of vertexes in the polygon.
        // We don't need the vertex count list for triangulation
        if ( triangulated ) return;

        // The number of vertices
        uint numVertices = 0;

        // Retrieve the vertices and increment polygon count
        // Get the number of vertices in the current mesh's polygon
        int polygonVertexCount = meshPolygonsIter.polygonVertexCount();
        if ( polygonVertexCount >= 3 )
        {
#ifdef VALIDATE_DATA
            // Skip over any duplicate vertices in this face.
            // Very rarely, a cunning user manages to corrupt
            // a face entry on the mesh and somehow configure
            // a face to include the same vertex multiple times.
            // This will cause the read-back of this data to
            // reject the face, and can crash other COLLADA
            // consumers, so better to lose the data here
            MIntArray vertexIndices;
            vertexIndices->setLength ( polygonVertexCount );
            for ( int pv = 0; pv < polygonVertexCount; ++pv )
            {
                ( *vertexIndices ) [pv] = pv;
            }

            for ( uint n = 0; n < vertexIndices->length() - 1; ++n )
            {
                for ( uint m = n + 1; m < vertexIndices->length(); )
                {
                    if ( ( *vertexIndices ) [n] == ( *vertexIndices ) [m] )
                    {
                        vertexIndices->remove ( m );
                    }
                    else ++m;
                }
            }
            // Get the number of vertices of the current polygon.
            numVertices = ( int ) vertexIndices->length();
#else
            numVertices = polygonVertexCount;
#endif

            // Push the number of vertices of the current polygon in the vcount list.
            vertexCountList.push_back ( numVertices );
        }
    }

    // ----------------------------------------
    uint GeometryPolygonExporter::getShaderPolygonsCount(
        const MFnMesh &fnMesh, 
        const uint shaderPosition, 
        const uint realShaderCount, 
        const MIntArray &shaderIndices )
    {
        uint numPolygons = 0;

        // Iterate through all polygons of the current mesh.
        // Check their polygons for holes and retrieve the vertexCountList.
        MItMeshPolygon meshPolygonsIter ( fnMesh.object() );
        for ( meshPolygonsIter.reset(); !meshPolygonsIter.isDone(); meshPolygonsIter.next() )
        {
            // Is this polygon shaded by this shader?
            int polyIndex = meshPolygonsIter.index();

            if ( shaderPosition < realShaderCount && 
                ( uint ) shaderIndices[polyIndex] != shaderPosition ) 
                continue;

            if ( shaderPosition >= realShaderCount && 
                ( shaderIndices[polyIndex] >= 0 && 
                shaderIndices[polyIndex] < ( int ) realShaderCount ) ) 
                continue;

            // Get the polygon count
            uint numMeshPolygons = 0, numVertices = 0;

            // TODO 
            // Establish the number of polygons 
            // (in case of triangulation more than one is possible) 

            // Get the number of vertices in the current mesh's polygon
            int polygonVertexCount = meshPolygonsIter.polygonVertexCount();
            if ( triangulated && polygonVertexCount > 3 )
            {
                int numTriangles;
                meshPolygonsIter.numTriangles ( numTriangles );
                if ( numTriangles > 0 )
                {
                    numMeshPolygons = (uint) numTriangles;
//                     uint numVertices = 3;
//                     MPointArray vertexPositions;
//                     MIntArray meshVertexIndices;
//                     meshPolygonsIter.getTriangles ( vertexPositions, meshVertexIndices );
//                     numMeshPolygons = meshVertexIndices.length() / numVertices;
                }
                else
                {
                    numMeshPolygons = 0;
                }
            }
            else if ( polygonVertexCount >= 3 )
            {
                numMeshPolygons = 1;
            }

           numPolygons += numMeshPolygons;
        }

        return numPolygons;
    }

    // --------------------------------------------------------
    void GeometryPolygonExporter::retrieveVertexIndices ( 
        MIntArray *vertexIndices,
        MItMeshPolygon &meshPolygonsIter,
        uint &numPolygons,
        uint &numVertices,
        std::vector<unsigned long> &vertexCountList )
    {
        // Get the number of vertices in the current mesh's polygon
        int polygonVertexCount = meshPolygonsIter.polygonVertexCount();
        if ( triangulated && polygonVertexCount > 3 )
        {
            int numTriangles;
            meshPolygonsIter.numTriangles ( numTriangles );
            if ( numTriangles > 0 )
            {
                numVertices = 3;
                MPointArray vertexPositions;
                MIntArray meshVertexIndices;
                meshPolygonsIter.getTriangles ( vertexPositions, meshVertexIndices );
                vertexIndices->setLength ( meshVertexIndices.length() );
                numPolygons = meshVertexIndices.length() / numVertices;

                // Map the vertex indices to iterator vertex indices so that we can
                // get information from the iterator about normals and such.
                uint meshVertexIndexCount = meshVertexIndices.length();
                for ( uint mvi = 0; mvi < meshVertexIndexCount; ++mvi )
                {
                    int meshVertexIndex = meshVertexIndices[mvi];
                    int polygonVertexCount = meshPolygonsIter.polygonVertexCount();
                    int iteratorVertexIndex = 0;
                    for ( int pv = 0; pv < polygonVertexCount; ++pv )
                    {
                        if ( ( int ) meshPolygonsIter.vertexIndex ( pv ) == meshVertexIndex )
                        {
                            iteratorVertexIndex = pv;
                        }
                    }
                    ( *vertexIndices ) [mvi] = iteratorVertexIndex;
                }

                // Push the number of vertices of the current polygon in the vcount list.
                vertexCountList.push_back ( meshVertexIndexCount );
            }
            else numPolygons = 0;
        }
        else if ( polygonVertexCount >= 3 )
        {
            numPolygons = 1;
            vertexIndices->setLength ( polygonVertexCount );

            for ( int pv = 0; pv < polygonVertexCount; ++pv )
            {
                ( *vertexIndices ) [pv] = pv;
            }

#ifdef VALIDATE_DATA
            // Skip over any duplicate vertices in this face.
            // Very rarely, a cunning user manages to corrupt
            // a face entry on the mesh and somehow configure
            // a face to include the same vertex multiple times.
            // This will cause the read-back of this data to
            // reject the face, and can crash other COLLADA
            // consumers, so better to lose the data here
            //
            for ( uint n = 0; n < vertexIndices->length() - 1; ++n )
            {
                for ( uint m = n + 1; m < vertexIndices->length(); )
                {
                    if ( ( *vertexIndices ) [n] == ( *vertexIndices ) [m] )
                    {
                        vertexIndices->remove ( m );
                    }
                    else ++m;
                }
            }
#endif

            // Get the number of vertices of the current polygon.
            numVertices = ( int ) vertexIndices->length();

            // Push the number of vertices of the current polygon in the vcount list.
            vertexCountList.push_back ( numVertices );
        }
    }

    // ----------------------------------------
    void GeometryPolygonExporter::writeShaderPolygons( 
        PolygonSourceList &shaderPolygons, 
        COLLADA::PrimitivesBase *primitivesBasePoly,
        const uint exportType)
    {
        // Iterate through the list of polygons and 
        // write them into the collada file.
        uint numPolygons = shaderPolygons.size();
        for ( uint pp=0; pp<numPolygons; ++pp )
        {
            PolygonSource* currentPolygon = shaderPolygons[pp];

            // Add the open tags for the polygons
            if ( exportType == POLYGONS )
            {
                if ( currentPolygon->isHoled() )
                {
                    ( ( COLLADA::Polygons* ) primitivesBasePoly )->openPolylistHole();
                }
            }

            // Insert the list of indices of the vertex attributes for the polygon list
            const Sources& vertexAttributes = currentPolygon->getVertexAttributes();

            // The face index
            uint currentFaceIndex = 0;

            // Initialize the data for polygons with holes
            uint sumOfFaceVertexCounts = currentPolygon->getFaceVertexCounts()[currentFaceIndex];

            // Check if the current face is a normal polygon or a hole and open the corresponding tag.
            if ( exportType == POLYGONS ) 
            {
                openPolygonOrHoleElement ( 
                    primitivesBasePoly, 
                    currentPolygon,
                    currentFaceIndex );
            }

            // --------------------------------------------
            // Iterate through the vertices

            uint numVertices = 0;
            size_t numAttributes = vertexAttributes.size();
            if ( numAttributes > 0 )
                numVertices = vertexAttributes[0].getNumVertexIndices();

            for ( uint vertexPosition=0; vertexPosition<numVertices; ++vertexPosition )
            {
                // If we write a holed polygon and the actual vertex position is the last
                // position of the current face, then go to the next face in the list.
                if ( exportType == POLYGONS &&
                    currentPolygon->isHoled() &&
                    vertexPosition == sumOfFaceVertexCounts )
                {
                    writePolygonOrHoleElement(
                        primitivesBasePoly, currentPolygon, 
                        sumOfFaceVertexCounts, currentFaceIndex );
                }

                // --------------------------------------
                // Write every vertex attribute

                for ( uint kk=0; kk<numAttributes; ++kk )
                {
                    const SourceInput sourceInput = vertexAttributes[kk];
                    const std::vector<int>& indexes = sourceInput.getIndices();

                    if ( vertexPosition < indexes.size() )
                    {
                        int index = indexes[vertexPosition];
                        primitivesBasePoly->appendValues ( index );
                    }
                    else
                    {
                        // Assert, cause the index position is wrong. 
                        // There is something wrong with the created polygons!
                        MGlobal::displayError ( "Index position in vertex attribute is wrong!" );
                        assert ( vertexPosition < indexes.size() ); 
                        return;
                    }
                }
            }

            // Add the tags for the polygons
            if ( exportType == POLYGONS )
            {
                if ( currentPolygon->isHoled() ) ( ( COLLADA::Polygons* ) primitivesBasePoly )->closeElement();

                ( ( COLLADA::Polygons* ) primitivesBasePoly )->closeElement();
            }
        }

        // Finish writing the values of the primitives base. 
        primitivesBasePoly->finish();
    }

    // ----------------------------------------------------------------------------------
    void GeometryPolygonExporter::setVertexCountList ( 
        COLLADA::PrimitivesBase* primitivesBasePoly,
        PolygonSourceList* polygons )
    {
        PolygonSource* polygon;
        std::vector<unsigned long> polyonVertexCountList;

        // Iterate through the polygons
        uint numPolygons = polygons->size();
        for ( uint p=0; p<numPolygons; ++p )
        {
            polygon = ( *polygons ) [p];
            polyonVertexCountList = polygon->getVertexCountList();

            // Iterate through the polygons vertex count list
            for ( uint v=0; v<polyonVertexCountList.size(); ++v )
            {
                // Set vertex count in the list of the POLYLIST
                unsigned long polyonVertexCount = polyonVertexCountList[v];
                primitivesBasePoly->getVCountList().push_back ( polyonVertexCount );
            }
        }
    }

    // ----------------------------------------------------------------------------------
    uint GeometryPolygonExporter::determinePrimitivesBaseExportType ( const bool currentShapeIsHoled )
    {
        uint exportType;

        // Just create the polylist, if there are polygons to export
        if ( currentShapeIsHoled && !triangulated )
        {
            exportType = POLYGONS;
        }
        else if ( triangulated )
        {
            exportType = TRIANGLES;
        }
        else
        {
            exportType = POLYLIST;
        }

        return exportType;
    }

    // ----------------------------------------------------------------------------------
    COLLADA::PrimitivesBase* GeometryPolygonExporter::createPrimitivesBase ( 
        COLLADA::PrimitivesBase dummyPoly,
        const uint exportType )
    {
        COLLADA::PrimitivesBase* primitivesBasePoly;

        switch ( exportType )
        {
        case POLYGONS:
            primitivesBasePoly = new COLLADA::Polygons ( dummyPoly );
            break;
        case TRIANGLES:
            primitivesBasePoly = new COLLADA::Triangles ( dummyPoly );
            break;
        case POLYLIST:
        default:
            primitivesBasePoly = new COLLADA::Polylist ( dummyPoly );
            break;
        }

        return primitivesBasePoly;
    }

    // ----------------------------------------------------------------------------------
    void GeometryPolygonExporter::preparePrimitivesBaseToAppendValues ( 
        COLLADA::PrimitivesBase* polylist,
        const uint exportType )
    {
        switch ( exportType )
        {
        case POLYGONS:
            ( ( COLLADA::Polygons* ) polylist )->prepareToAppendValues();
            break;
        case TRIANGLES:
            ( ( COLLADA::Triangles* ) polylist )->prepareToAppendValues();
            break;
        case POLYLIST:
        default:
            ( ( COLLADA::Polylist* ) polylist )->prepareToAppendValues();
            break;
        }
    }

    // ----------------------------------------------------------------------------------
    void GeometryPolygonExporter::openPolygonOrHoleElement ( 
        COLLADA::PrimitivesBase* polylist,
        PolygonSource* poly,
        const uint currentFaceIndex )
    {
        bool currentFaceIsHole = checkForHole ( poly, currentFaceIndex );
        if ( currentFaceIsHole )
        {
            ( ( COLLADA::Polygons* ) polylist )->openHole();
        }
        else
        {
            ( ( COLLADA::Polygons* ) polylist )->openPolylist();
        }
    }

    // ----------------------------------------------------------------------------------
    void GeometryPolygonExporter::writePolygonOrHoleElement( 
        COLLADA::PrimitivesBase *primitivesBasePoly, 
        PolygonSource *currentPolygon, 
        uint &sumOfFaceVertexCounts,
        uint &currentFaceIndex )
    {
        // Increment, cause we have found the next face
        ++currentFaceIndex;

        // Close the tag for the last face
        ( ( COLLADA::Polygons* ) primitivesBasePoly )->closeElement();

        // Get the vertex count of the current face
        uint currentFaceVertexCount = currentPolygon->getFaceVertexCounts()[currentFaceIndex];

        // Add the vertex count of the current face to the sum of face vertexes
        sumOfFaceVertexCounts += currentFaceVertexCount;

        // Check if the current face is a normal polygon or a hole and open the corresponding tag.
        openPolygonOrHoleElement ( primitivesBasePoly, currentPolygon, currentFaceIndex );
    }

    // ----------------------------------------------------------------------------------
    bool GeometryPolygonExporter::checkForHole ( const PolygonSource* polygon, const uint currentFaceIndex )
    {
        // Check if the current face is a normal polygon or a hole
        bool currentFaceIsHole = false;
        uint numHoles = polygon->getHoleFaces().size();
        for ( uint holeIndex=0; holeIndex<numHoles && !currentFaceIsHole; ++holeIndex )
        {
            uint holeFaceIndex = polygon->getHoleFaces()[holeIndex];
            if ( holeFaceIndex == currentFaceIndex )
            {
                currentFaceIsHole = true;
            }
        }

        return currentFaceIsHole;
    }

    // ----------------------------------------------------------------------------------
    int GeometryPolygonExporter::exportPolygonVertices ( 
        MFnMesh &fnMesh,
        MItMeshPolygon &meshPolygonsIter,
        COLLADA::PrimitivesBase* polylist,
        PolygonSource* polygon )
    {
        // Collect data in order to handle triangle-only export option
        uint numPolygons = 0, numVertices = 0;

        // Retrieve the vertex indices and establish the number of polygons (in case of
        // triangulation more than one is possible) and the number of vertexes in the polygon.
        MIntArray vertexIndices;
        retrieveVertexIndices ( 
            &vertexIndices,
            meshPolygonsIter,
            numPolygons,
            numVertices,
            polygon->getVertexCountList() );

        // Iterate through the polygons (normally just one polygon,
        // just in case of triangulation it could be more than one)
        for ( uint polygonPosition=0; polygonPosition<numPolygons; ++polygonPosition )
        {
            // Put the current face in the list of faces
            polygon->getFaceVertexCounts().push_back ( numVertices );

            // Get the index of the current polygon
            int polyIndex = meshPolygonsIter.index();

            // Buffer the face normal indices
            MIntArray normalIndices;
            if ( mHasFaceVertexNormals )
            {
                fnMesh.getFaceNormalIds ( polyIndex, normalIndices );
            }

            // Iterate through the vertexes of the current polygon
            for ( uint j=0; j<numVertices; j++ )
            {
                // Handle front face vs back face by walking
                // the vertexes backward on the back-face
                int iteratorVertexIndex = vertexIndices[polygonPosition * numVertices + j];

                // Exports the indices of the current vertex
                exportVertexIndices ( 
                    fnMesh,
                    meshPolygonsIter,
                    polygon,
                    normalIndices,
                    iteratorVertexIndex,
                    numVertices );
            }
        }

        return numPolygons;
    }

    // ----------------------------------------------------------------------------------
    void GeometryPolygonExporter::exportVertexIndices ( 
        MFnMesh &fnMesh,
        MItMeshPolygon &meshPolygonsIter,
        PolygonSource* polygon,
        MIntArray &normalIndices,
        int iteratorVertexIndex,
        int numVertices )
    {
        // Handle front face vs back face by walking the vertices backward on the backface
        int vertexIndex = meshPolygonsIter.vertexIndex ( iteratorVertexIndex );

        // Get the index of the current Polygon
        int polyIndex = meshPolygonsIter.index();

        // Look for holes in this polygon
        // ASSUMPTION: Holes are automatically removed by triangulation.
        // ASSUMPTION: The iterator gives the hole vertices at the end of the enumeration.
        // ASSUMPTION: Hole vertices are never used as surface vertices or repeated between holes or inside a hole.
        if ( meshPolygonsIter.isHoled() && !triangulated )
        {
            handleHoledPolygon(polygon, polyIndex, vertexIndex, numVertices, iteratorVertexIndex);
        }

        getVertexIndices(polygon, vertexIndex, normalIndices, iteratorVertexIndex, meshPolygonsIter, fnMesh, polyIndex);

        return;
    }

    // ---------------------------------------------
    void GeometryPolygonExporter::handleHoledPolygon( 
        PolygonSource* polygon, 
        const int polyIndex, 
        const int vertexIndex, 
        const int numVertices, 
        const int iteratorVertexIndex )
    {
        // Set the flag to the polygon, that it is a holed one
        polygon->isHoled(true);

        // Put the index of the hole in the list of holes and put the face in the list of faces
        for ( uint holePosition = 0; holePosition < holeCount; ++holePosition )
        {
            if ( mHoleInfoArray[holePosition*3] == polyIndex )
            {
                uint holeVertexOffset = mHoleInfoArray[holePosition*3+2];
                if ( holeVertexOffset <= mHoleVertexArray.length() &&
                    mHoleVertexArray[holeVertexOffset] == vertexIndex )
                {
                    // Decrement the index of the last face for the index of the hole face
                    size_t faceIndex = polygon->getFaceVertexCounts().size();
                    polygon->getFaceVertexCounts()[faceIndex-1] -= ( numVertices - iteratorVertexIndex );

                    // Put the index of the hole in the list of holes
                    polygon->getHoleFaces().push_back ( ( uint ) faceIndex );

                    // put the face in the list of faces
                    polygon->getFaceVertexCounts().push_back ( numVertices - iteratorVertexIndex );
                }
            }
        }
    }

    // ---------------------------------------------
    void GeometryPolygonExporter::generatePolygonSetInputs ( 
        COLLADA::PrimitivesBase* polylist,
        Sources* vertexAttributes )
    {
        // Generate the polygon set inputs.
        int nextIdx = 1, normalsIdx = -1;
        int offset = 0; // Offset for the input list
        size_t inputCount = mPolygonSources->size();

        for ( size_t p = 0; p < inputCount; ++p )
        {
            const SourceInput param = ( *mPolygonSources ) [p];
            const COLLADA::SourceBase source = param.getSource();
            const COLLADA::Semantics type = param.getType();

            // Figure out which idx this parameter will use
            int foundIdx = -1;

            // For geometric tangents and bi-normals, use the same idx as the normals.
            // For texture tangents and bi-normals, group together for each UV set.
            if ( type == COLLADA::NORMAL || type == COLLADA::GEOTANGENT || type == COLLADA::GEOBINORMAL )
            {
                foundIdx = normalsIdx;
            }

            // Check for duplicate vertex attributes to use their idx
            if ( foundIdx == -1 )
            {
                size_t vertexAttributeCount = vertexAttributes->size();
                for ( size_t v = 0; v < vertexAttributeCount; ++v )
                {
                    if ( ( *vertexAttributes ) [v].getType() == type &&
                        ( *vertexAttributes ) [v].getIdx() == param.getIdx() )
                    {
                        foundIdx = ( int ) v;
                        break;
                    }
                }
            }

            // Check if the vertex is already registered
            bool isVertexSource = SourceInput::containsSourceBase ( mVertexSources, &source );

            // New vertex attribute, so generate a new idx
            bool newIdx = foundIdx == -1;
            if ( newIdx )
            {
                // Assign it the correct offset/idx.
                foundIdx = ( !isVertexSource ) ? nextIdx++ : 0;

                if ( type == COLLADA::NORMAL || type == COLLADA::GEOTANGENT || type == COLLADA::GEOBINORMAL )
                {
                    normalsIdx = foundIdx;
                }
            }

            // Add the per-face, per-vertex input to the polygons description
            if ( !isVertexSource )
            {
                // Get the input list
                COLLADA::InputList* inputList = &polylist->getInputList();

                // Get the generated id of the source for reference
                String sourceId = source.getId();

                // The vertex sources must reference to the vertexes element
                if ( type == COLLADA::VERTEX )
                {
                    String suffix = getSuffixBySemantic ( type );
                    sourceId = mMeshId + suffix;
                }

                if ( type == COLLADA::TEXCOORD )
                {
                    // For texture coordinate-related inputs: set the 'set' attribute.
                    inputList->push_back ( COLLADA::Input ( type, "#" + sourceId, offset++, param.getIdx() ) );
                }
                else
                {
                    inputList->push_back ( COLLADA::Input ( type, "#" + sourceId, offset++ ) );
                }
            }


            if ( newIdx )
            {
                // param for the last time.
                vertexAttributes->push_back ( SourceInput(param) );
            }

        }

    }

    // --------------------------------------------------------
    void GeometryPolygonExporter::addHole ( uint index )
    {
        // The hole mustn't already be inserted in the hole faces list.
        std::vector<uint>::iterator searchIter;
        searchIter = find ( mHoleFaces.begin(), mHoleFaces.end(), index );
        assert ( searchIter != mHoleFaces.end() );

        // Ordered insert
        std::vector<uint>::iterator it = mHoleFaces.begin();
        for ( ; it != mHoleFaces.end(); ++it )
        {
            if ( index < ( *it ) ) break;
        }

        mHoleFaces.insert ( searchIter, index );
    }

    // --------------------------------------------------------
    uint GeometryPolygonExporter::createShaderPolygons( 
        MFnMesh &fnMesh, 
        const uint shaderPosition, 
        const uint realShaderCount, 
        const MIntArray &shaderIndices, 
        COLLADA::PrimitivesBase *dummyPrimitivesBase, 
        PolygonSourceList &shaderPolygons, 
        bool &currentShapeIsHoled )
    {
        // Number of polygons (could also be triangles)
        uint numPolygons = 0; 

        // List for the polygon set inputs.
        Sources polygonSetInputs;

        // Iterate through all polygons of the current mesh and create them to export
        MItMeshPolygon meshPolygonsIter ( fnMesh.object() );
        for ( meshPolygonsIter.reset(); !meshPolygonsIter.isDone(); meshPolygonsIter.next() )
        {
            // Is this polygon shaded by this shader?
            int polyIndex = meshPolygonsIter.index();

            if ( shaderPosition < realShaderCount && 
                ( uint ) shaderIndices[polyIndex] != shaderPosition ) 
                continue;

            if ( shaderPosition >= realShaderCount && 
                ( shaderIndices[polyIndex] >= 0 && 
                shaderIndices[polyIndex] < ( int ) realShaderCount ) ) 
                continue;

            // Initialize the polylist if it is the first polygon to export
            if ( numPolygons == 0 )
            {
                // Generate the polygon set inputs.
                generatePolygonSetInputs ( dummyPrimitivesBase, &polygonSetInputs );
            }

            // Create a polygon to store the vertex indexes to export
            PolygonSource* polygon = new PolygonSource ( polygonSetInputs );

            // Export the vertices and increment polygon count
            uint currentNumPolygons = exportPolygonVertices ( 
                fnMesh,
                meshPolygonsIter,
                dummyPrimitivesBase,
                polygon );

            numPolygons += currentNumPolygons;

            // If we have polygons to export, push it into the polygon list
            if ( currentNumPolygons > 0 )
            {
                shaderPolygons.push_back ( polygon );
                if ( polygon->isHoled() ) currentShapeIsHoled = true;
            }
            else
            {
                delete polygon;
            }
        }

        return numPolygons;
    }

    // --------------------------------------------------------
    void GeometryPolygonExporter::getVertexIndices( 
        PolygonSource* polygon, 
        int vertexIndex, 
        MIntArray & normalIndices, 
        int iteratorVertexIndex, 
        MItMeshPolygon &meshPolygonsIter, 
        MFnMesh &fnMesh, 
        int polyIndex )
    {
        // Dump the indices
        size_t numAttributes = polygon->getVertexAttributes().size();

        // Output each vertex attribute we need
        for ( size_t kk = 0; kk < numAttributes; ++kk )
        {
            SourceInput& vertexAttributes = polygon->getVertexAttributes()[kk];
            switch ( vertexAttributes.getType() )
            {
            case COLLADA::VERTEX:
            case COLLADA::POSITION:
                vertexAttributes.getIndices().push_back ( vertexIndex );
                break;
            case COLLADA::NORMAL:
            case COLLADA::GEOTANGENT:
            case COLLADA::GEOBINORMAL:
                {
                    if (mHasFaceVertexNormals)
                    {
                        int currentVertexIndex = normalIndices[iteratorVertexIndex];
                        vertexAttributes.getIndices().push_back ( currentVertexIndex );
                    }
                    else
                    {
                        // Assert, if we don't have initialized the normal indices, 
                        // but want to read them out here!
                        MGlobal::displayError("No face vertex normals to proceed!");
                        assert ( mHasFaceVertexNormals );
                        return;
                    }
                    break;
                }
            case COLLADA::TEXCOORD:
                {
                    int uvIndex = 0;
                    meshPolygonsIter.getUVIndex ( iteratorVertexIndex, uvIndex, &mUvSetNames[vertexAttributes.getIdx()] );
                    vertexAttributes.getIndices().push_back ( uvIndex );
                    break;
                }
            case COLLADA::COLOR:
                {
                    ColourSet& set = *mColorSets[vertexAttributes.getIdx()];
                    int colorIndex = 0;
                    if ( set.indices.length() > 0 )
                    {
                        fnMesh.getFaceVertexColorIndex ( polyIndex, iteratorVertexIndex, colorIndex );
                        if ( colorIndex >= 0 && colorIndex < ( int ) set.indices.length() )
                            colorIndex = set.indices[colorIndex];
                    }
                    else
                    {
#if MAYA_API_VERSION >= 700
                        fnMesh.getColorIndex ( polyIndex, iteratorVertexIndex, colorIndex, &set.name );
#else
                        fnMesh.getFaceVertexColorIndex ( polyIndex, iteratorVertexIndex, colorIndex );
#endif
                    }
                    // Its possible for colorIndex to be -1, however COLLADA doesn't support
                    // non-colored vertexes, so simply use the white color index
                    if ( colorIndex < 0 ) colorIndex = set.whiteColorIndex;
                    vertexAttributes.getIndices().push_back ( colorIndex );
                    break;
                }
            case COLLADA::UNKNOWN:
            case COLLADA::UV:
            case COLLADA::EXTRA:
            default:
                break; // Not exported/supported
            }
        }
    }

    // --------------------------------------------------------
    void GeometryPolygonExporter::writeVertexIndices(
        COLLADA::PrimitivesBase* primitivesBasePoly,
        PolygonSource *polygon, 
        int vertexIndex, 
        MIntArray &normalIndices, 
        int iteratorVertexIndex, 
        MItMeshPolygon &meshPolygonsIter, 
        MFnMesh &fnMesh, 
        int polyIndex )
    {
        // Dump the indices
        uint numAttributes = polygon->getVertexAttributes().size();

        // Output each vertex attribute we need
        for ( size_t kk = 0; kk < numAttributes; ++kk )
        {
            SourceInput& vertexAttributes = polygon->getVertexAttributes()[kk];
            switch ( vertexAttributes.getType() )
            {
            case COLLADA::VERTEX:
            case COLLADA::POSITION:
                primitivesBasePoly->appendValues ( vertexIndex );
                break;
            case COLLADA::NORMAL:
            case COLLADA::GEOTANGENT:
            case COLLADA::GEOBINORMAL:
                {
                    if (mHasFaceVertexNormals)
                    {
                        int currentVertexIndex = normalIndices[iteratorVertexIndex];
                        primitivesBasePoly->appendValues ( currentVertexIndex );
                    }
                    else
                    {
                        // Assert, if we don't have initialized the normal indices, 
                        // but want to read them out here!
                        MGlobal::displayError("No face vertex normals to proceed!");
                        assert ( mHasFaceVertexNormals );
                        return;
                    }
                    break;
                }
            case COLLADA::TEXCOORD:
                {
                    int uvIndex = 0;
                    meshPolygonsIter.getUVIndex ( iteratorVertexIndex, uvIndex, &mUvSetNames[vertexAttributes.getIdx()] );
                    primitivesBasePoly->appendValues ( uvIndex );
                    break;
                }
            case COLLADA::COLOR:
                {
                    ColourSet& set = *mColorSets[vertexAttributes.getIdx()];
                    int colorIndex = 0;
                    if ( set.indices.length() > 0 )
                    {
                        fnMesh.getFaceVertexColorIndex ( polyIndex, iteratorVertexIndex, colorIndex );
                        if ( colorIndex >= 0 && colorIndex < ( int ) set.indices.length() )
                            colorIndex = set.indices[colorIndex];
                    }
                    else
                    {
#if MAYA_API_VERSION >= 700
                        fnMesh.getColorIndex ( polyIndex, iteratorVertexIndex, colorIndex, &set.name );
#else
                        fnMesh.getFaceVertexColorIndex ( polyIndex, iteratorVertexIndex, colorIndex );
#endif
                    }
                    // Its possible for colorIndex to be -1, however COLLADA doesn't support
                    // non-colored vertexes, so simply use the white color index
                    if ( colorIndex < 0 ) colorIndex = set.whiteColorIndex;
                    primitivesBasePoly->appendValues ( colorIndex );
                    break;
                }
            case COLLADA::UNKNOWN:
            case COLLADA::UV:
            case COLLADA::EXTRA:
            default:
                break; // Not exported/supported
            }
        }
    }

    // --------------------------------------------------------
    COLLADA::PrimitivesBase* GeometryPolygonExporter::preparePrimitivesBasePoly( 
        COLLADA::PrimitivesBase dummyPrimitivesBase, 
        uint numPolygons, 
        PolygonSourceList shaderPolygons, 
        uint shaderPosition, 
        uint realShaderCount, 
        uint exportType, 
        MObjectArray shaders )
    {
        // Just create a polylist, if there are polygons to export
        // If we have holes in the polygon, we have to use <polygons> instead of <polylist>.
        // If we want to export as triangles, we have to use <triangles>.
        COLLADA::PrimitivesBase* primitivesBasePoly = NULL;

        // Now create the real Polylist/Polygons/Triangles element in depend
        // of the current export type. If the export type is POLYLIST, set the vertex count list.
        primitivesBasePoly = createPrimitivesBase ( dummyPrimitivesBase, exportType );

        // Set the number of polygons
        primitivesBasePoly->setCount ( numPolygons );

        // Set the vertex count list, if we have a POLYLIST
        if ( exportType == POLYLIST )
        {
            setVertexCountList ( primitivesBasePoly, &shaderPolygons );
        }

        // Check if the material should be set
        if ( shaderPosition < realShaderCount )
        {
            // Add shader-specific parameters (TexCoords sets).
            // Add symbolic name for the material used on this polygon set.
            MFnDependencyNode shaderFn ( shaders[shaderPosition] );
            String shaderName = shaderFn.name().asChar();
            String materialName = mDocumentExporter->mayaNameToColladaName ( shaderFn.name() );
            primitivesBasePoly->setMaterial ( materialName );
        }

        // Prepare the list for add the vertex indexes
        preparePrimitivesBaseToAppendValues ( primitivesBasePoly, exportType );

        return primitivesBasePoly;
    }

    // --------------------------------------------------------
    COLLADA::PrimitivesBase* GeometryPolygonExporter::preparePrimitivesBase( 
        COLLADA::PrimitivesBase& dummyPrimitivesBase, 
        uint numPolygons, 
        std::vector<unsigned long>& vCountList, 
        uint shaderPosition, 
        uint realShaderCount, 
        uint exportType, 
        MObjectArray shaders )
    {
        // Just create a polylist, if there are polygons to export
        // If we have holes in the polygon, we have to use <polygons> instead of <polylist>.
        // If we want to export as triangles, we have to use <triangles>.
        COLLADA::PrimitivesBase* primitivesBasePoly = NULL;

        // List for the polygon set inputs.
        Sources polygonSetInputs;

        // Generate the polygon set inputs.
        generatePolygonSetInputs ( &dummyPrimitivesBase, &polygonSetInputs );

        // Now create the real Polylist/Polygons/Triangles element in depend
        // of the current export type. If the export type is POLYLIST, set the vertex count list.
        primitivesBasePoly = createPrimitivesBase ( dummyPrimitivesBase, exportType );

        // Set the number of polygons
        primitivesBasePoly->setCount ( numPolygons );

        // Set the vertex count list, if we have a POLYLIST
        if ( exportType == POLYLIST )
        {
            // TODO Method writeVCountList()
            primitivesBasePoly->setVCountList( vCountList );
        }

        // TODO Method to get all materials of the current mesh.
        // Check if the material should be set
        if ( shaderPosition < realShaderCount )
        {
            // Add shader-specific parameters (TexCoords sets).
            // Add symbolic name for the material used on this polygon set.
            MFnDependencyNode shaderFn ( shaders[shaderPosition] );
            String shaderName = shaderFn.name().asChar();
            String materialName = mDocumentExporter->mayaNameToColladaName ( shaderFn.name() );
            primitivesBasePoly->setMaterial ( materialName );
        }

        // Prepare the list for add the vertex indexes
        preparePrimitivesBaseToAppendValues ( primitivesBasePoly, exportType );

        return primitivesBasePoly;
    }

}