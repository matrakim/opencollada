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
#include "COLLADAMayaDocumentImporter.h"
#include "COLLADAMayaReferenceManager.h"
#include "COLLADAMayaImportOptions.h"
#include "COLLADAMayaMaterialImporter.h"
#include "COLLADAMayaEffectImporter.h"
#include "COLLADAMayaGeometryImporter.h"
#include "COLLADAMayaCameraImporter.h"
#include "COLLADAMayaLightImporter.h"
#include "COLLADAMayaImageImporter.h"
#include "COLLADAMayaAnimationImporter.h"
#include "COLLADAMayaVisualSceneImporter.h"

#include "COLLADAFWRoot.h"
#include "COLLADAFWAnimationList.h"

#include "COLLADASaxFWLLoader.h"

#include <maya/MFileIO.h>

#include <fstream>
#include <time.h>

#include "MayaDMScript.h"


namespace COLLADAMaya
{

    const String DocumentImporter::ASCII_PATH_EXTENSION = ".ma";
    const String DocumentImporter::ASCII_PATH_EXTENSION_DEBUG = ".nextgen.ma";
    const int DocumentImporter::BUFFERSIZE = 2097152;


    //---------------------------------
    DocumentImporter::DocumentImporter ( const String& fileName )
        : mColladaFileName ( fileName )
        , mSceneId ( "MayaScene" )
        , mFile ( 0 )
        , mVisualSceneImporter (0)
        , mGeometryImporter (0)
        , mMaterialImporter (0)
        , mEffectImporter (0)
        , mCameraImporter (0)
        , mLightImporter (0)
        , mImageImporter (0)
        , mAnimationImporter (0)
        , mSceneGraphWritten (false)
        , mLibraryNodesWritten (false)
        , mAssetWritten (false)
        , mAnimationsWritten (false)
        , mSceneGraphRead (false)
        , mLibraryNodesRead (false)
        , mGeometryRead (false)
        , mCameraRead (false)
        , mLightRead (false)
        , mImageRead (false)
        , mAnimationRead (false)
        , mNumDocumentParses (0)
        , mUpAxisType ( COLLADAFW::FileInfo::Y_UP )
        , mLinearUnitMeter ( COLLADAFW::FileInfo::Unit::LINEAR_UNIT_CENTIMETER )
    {
    }

    //---------------------------------
    DocumentImporter::~DocumentImporter()
    {
        releaseLibraries(); 

        // Close the maya ascii file
        closeMayaAsciiFile ();
    }

    //---------------------------------
    void DocumentImporter::createLibraries()
    {
        // First release the existing libraries.
        releaseLibraries();

        mVisualSceneImporter = new VisualSceneImporter ( this );
        mGeometryImporter = new GeometryImporter ( this );
        mMaterialImporter = new MaterialImporter ( this );
        mEffectImporter = new EffectImporter ( this );
        mCameraImporter = new CameraImporter ( this );
        mLightImporter = new LightImporter ( this );
        mImageImporter = new ImageImporter (this);
        mAnimationImporter = new AnimationImporter (this);

        // Get the sceneID (assign a name to the scene)
        MString sceneName = MFileIO::currentFile ();
        if ( sceneName.length() != 0 ) mSceneId = sceneName.asChar();

        // Initialize the reference manager
        ReferenceManager::getInstance()->initialize ();
    }

    //---------------------------------
    void DocumentImporter::releaseLibraries()
    {
        delete mVisualSceneImporter;
        delete mGeometryImporter;
        delete mMaterialImporter;
        delete mEffectImporter;
        delete mCameraImporter;
        delete mLightImporter;
        delete mImageImporter;
        delete mAnimationImporter;
    }

    //-----------------------------
    void DocumentImporter::importCurrentScene()
    {
        // Create the import/export library helpers.
        createLibraries ();

        // Load the collada document into the collada framework.
        readColladaDocument();

        // Close the maya file.
        closeMayaAsciiFile ();
    }

    //-----------------------------
    bool DocumentImporter::createMayaAsciiFile ()
    {
        // TODO
        mMayaAsciiFileURI.set ( mColladaFileName );

#ifdef NDEBUG
        mMayaAsciiFileURI.setPathExtension ( ASCII_PATH_EXTENSION );
#else
        mMayaAsciiFileURI.setPathExtension ( ASCII_PATH_EXTENSION_DEBUG );
#endif

        String mayaAsciiFileName = mMayaAsciiFileURI.getURIString ();

        // Check if the file already exist.
        if ( std::ifstream ( mayaAsciiFileName.c_str () ) )
        {
            // TODO Open a dialog and ask the user to save the file under an other name.
            //MGlobal::displayError ( "File already exists!\n" );
            MGlobal::doErrorLogEntry ( "File already exists!\n" );
        }

        // Get the current locale value
        mLocale = setlocale ( LC_NUMERIC, 0 );
        setlocale ( LC_NUMERIC, "C" );

        mFile = fopen ( mayaAsciiFileName.c_str (), "w" );
        if ( mFile == 0 ) 
        {
            MGlobal::displayError ( "Can't open maya ascii file!\n" );
            return false;
        }

        // Set the buffer
        mBuffer = new char[BUFFERSIZE];
        bool failed = ( setvbuf ( mFile , mBuffer, _IOFBF, BUFFERSIZE ) != 0 );
        if ( failed )
        {
            delete[] mBuffer;
            MGlobal::displayError ( "Could not set buffer for writing." );
            return false;
        }

        return true;
    }

    //-----------------------------
    void DocumentImporter::closeMayaAsciiFile ()
    {
        if ( mFile ) 
        {
            fclose ( mFile );
            mFile = 0;

            setlocale ( LC_NUMERIC, mLocale.c_str() );
        }
    }

    //---------------------------------
    const String& DocumentImporter::getColladaFilename() const
    {
        return mColladaFileName;
    }

    //-----------------------------
    void DocumentImporter::cancel ( const String& errorMessage )
    {

    }

    //-----------------------------
    void DocumentImporter::start ()
    {
        // Create the maya file.
        bool retValue = createMayaAsciiFile ();
        assert ( retValue );

        // TODO Initialise the maya default objects.
        if ( retValue ) 
        {
            mLightImporter->initialiseDefaultLightObjects ();
        }
    }

    //-----------------------------
    void DocumentImporter::finish ()
    {
        // There has not to be a <library_nodes> tag in the document!
        if ( mAssetWritten && mSceneGraphWritten && !mLibraryNodesWritten )
        {
            mLibraryNodesWritten = true;

            // Write the node instances, when all nodes of the visual scene 
            // and the library nodes are already imported!
            if ( mSceneGraphWritten && mLibraryNodesWritten )
            {
                // Write all the node instances
                mVisualSceneImporter->writeNodeInstances ();
            }

            if ( mGeometryRead || mCameraRead || mLightRead || mImageRead || mAnimationRead )
            {
                mGeometryRead = false;
                mCameraRead = false;
                mLightRead = false;
                mImageRead = false;
                mAnimationRead = false;
                readColladaDocument ();
            }
        }

        // If the last read is ready, we can write the connections and close the file.
        --mNumDocumentParses;
        if ( mNumDocumentParses == 0 ) 
        {
            // TODO After the complete read of the collada document, 
            // the connections can be written into the maya file.
            mMaterialImporter->writeConnections ();
            mLightImporter->writeConnections ();
            mEffectImporter->writeConnections ();

            // Close the file
            closeMayaAsciiFile ();
        }
    }

    //-----------------------------
    void DocumentImporter::readColladaDocument ()
    {
        COLLADASaxFWL::Loader loader ( &mSaxParserErrorHandler );
        COLLADAFW::Root root ( &loader, this );
        String filename = getColladaFilename ();
        String fileUriString = URI::nativePathToUri ( filename );

        ++mNumDocumentParses;
        root.loadDocument ( fileUriString );
    }

    //-----------------------------
    const COLLADABU::URI& DocumentImporter::getMayaAsciiFileURI () const
    {
        return mMayaAsciiFileURI;
    }

    //-----------------------------
    void DocumentImporter::getCurrentDate ( std::stringstream& curDate )
    {
        // create a stringstream containing the current date and time in ISO 8601 format
        time_t _t;
        time ( &_t );
        struct tm *t = localtime ( &_t );

        int weekDay = t->tm_wday; // days since Sunday - [0,6]
        switch ( weekDay )
        {
        case 0: curDate << "Sun, "; break;
        case 1: curDate << "Mon, "; break;
        case 2: curDate << "Tue, "; break;
        case 3: curDate << "Wed, "; break;
        case 4: curDate << "Thu, "; break;
        case 5: curDate << "Fri, "; break;
        case 6: curDate << "Sat, "; break;
        default: assert ( weekDay < 7 );
        }

        int month = t->tm_mon; // months since January - [0,11]
        switch ( month )
        {
        case 0: curDate << "Jan "; break;
        case 1: curDate << "Feb "; break;
        case 2: curDate << "Mar "; break;
        case 3: curDate << "Apr "; break;
        case 4: curDate << "Mai "; break;
        case 5: curDate << "Jun "; break;
        case 6: curDate << "Jul "; break;
        case 7: curDate << "Aug "; break;
        case 8: curDate << "Sep "; break;
        case 9: curDate << "Oct "; break;
        case 10: curDate << "Nov "; break;
        case 11: curDate << "Dec "; break;
        default: assert ( month < 12 );
        }

        // Mon, Dec 01, 2008 02:02:39 PM
        curDate << t->tm_mday << " "; // day of the month - [1,31]
        if ( t->tm_hour < 12 )
            curDate << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << " AM\n";
        else if ( t->tm_hour == 12 )
            curDate << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << " PM\n";
        else 
            curDate << t->tm_hour-12 << ":" << t->tm_min << ":" << t->tm_sec << " AM\n";
    }

    //-----------------------------
    COLLADAMaya::String DocumentImporter::frameworkNameToMayaName ( const String& name )
    {
        // Replace offending characters by some that are supported within maya:
        // ':', '|', '-', '!' are replaced by '_'.

        const char* c = name.c_str ();
        size_t length = name.length();
        char* tmp = new char[length + 1];

        for ( size_t i = 0; i <= length; i++ )
        {
            char d = c[i];
            if ( d == '.' || d == '-' || d == '|' || d == ':' || d == '/' || d == '\\' || d == '(' || d == ')' || d == '[' || d == ']' )
            {
                d =  '_';
            }
            tmp[i] = d;
        }

        String newName = COLLADABU::Utils::checkNCName ( tmp );
        delete[] tmp;
        return newName;
    }

    //-----------------------------
    bool DocumentImporter::writeGlobalAsset ( const COLLADAFW::FileInfo* asset )
    {
        if ( mAssetWritten ) return true;

        // Create the file, if not already done.
        if ( mFile == 0 ) start();

        // The maya version
        String mayaVersion ( MGlobal::mayaVersion ().asChar () );
        fprintf ( mFile, "//Maya ASCII %s scene\n", mayaVersion.c_str () );

        // We have to change the name on 64 bit machines. 
        // For example from "2008 x64" to "2008" or from "2008 Extension 2" to "2008".
        std::vector<String> words;
        String separator (" ");
        COLLADABU::Utils::split ( mayaVersion, separator, words );
        if ( words.size () > 1 ) 
        {
            fprintf ( mFile, "requires maya \"%s\";\n", words[0].c_str () );
        }
        else
        {
            fprintf ( mFile, "requires maya \"%s\";\n", mayaVersion.c_str () );
        }

        // Get the unit informations.
        const COLLADAFW::FileInfo::Unit& unit = asset->getUnit ();

        // Set the default value to centimeters.
        String linearUnitName = COLLADAFW::FileInfo::Unit::LINEAR_UNIT_CENTIMETER_NAME;
        if ( ImportOptions::importUnits () )
        {
            linearUnitName = unit.getLinearUnitName ();
            mLinearUnitMeter = unit.getLinearUnitMeter ();

            // Set the linear unit in meters.
            // Maya knows: millimeter, centimeter, meter, foot, inch and yard.
            switch ( unit.getLinearUnitUnit () )
            {
            case COLLADAFW::FileInfo::Unit::KILOMETER:
                {
                    // Convert to meters
                    linearUnitName = COLLADAFW::FileInfo::Unit::LINEAR_UNIT_METER_NAME;
                    mLinearUnitMeter = COLLADAFW::FileInfo::Unit::LINEAR_UNIT_KILOMETER / COLLADAFW::FileInfo::Unit::LINEAR_UNIT_METER;;
                    break;
                }
            case COLLADAFW::FileInfo::Unit::METER:
                {
                    // Don't convert 
                    linearUnitName = COLLADAFW::FileInfo::Unit::LINEAR_UNIT_METER_NAME;
                    mLinearUnitMeter =  COLLADAFW::FileInfo::Unit::LINEAR_UNIT_METER;
                    break;
                }
            case COLLADAFW::FileInfo::Unit::DECIMETER:
                {
                    // Convert to meters
                    linearUnitName = COLLADAFW::FileInfo::Unit::LINEAR_UNIT_METER_NAME;
                    mLinearUnitMeter = COLLADAFW::FileInfo::Unit::LINEAR_UNIT_DECIMETER;
                    break;
                }
            case COLLADAFW::FileInfo::Unit::CENTIMETER:
                {
                    // Don't convert 
                    linearUnitName = COLLADAFW::FileInfo::Unit::LINEAR_UNIT_CENTIMETER_NAME;
                    mLinearUnitMeter =  COLLADAFW::FileInfo::Unit::LINEAR_UNIT_CENTIMETER / COLLADAFW::FileInfo::Unit::LINEAR_UNIT_CENTIMETER;
                    break;
                }
            case COLLADAFW::FileInfo::Unit::MILLIMETER:
                {
                    // Convert to centimeters
                    linearUnitName = COLLADAFW::FileInfo::Unit::LINEAR_UNIT_CENTIMETER_NAME;
                    mLinearUnitMeter = COLLADAFW::FileInfo::Unit::LINEAR_UNIT_MILLIMETER / COLLADAFW::FileInfo::Unit::LINEAR_UNIT_CENTIMETER;
                    break;
                }
            case COLLADAFW::FileInfo::Unit::FOOT:
                {
                    // Don't convert 
                    linearUnitName = COLLADAFW::FileInfo::Unit::LINEAR_UNIT_FOOT_NAME;
                    mLinearUnitMeter =  COLLADAFW::FileInfo::Unit::LINEAR_UNIT_FOOT / COLLADAFW::FileInfo::Unit::LINEAR_UNIT_FOOT;
                    break;
                }
            case COLLADAFW::FileInfo::Unit::INCH:
                {
                    // Don't convert 
                    linearUnitName = COLLADAFW::FileInfo::Unit::LINEAR_UNIT_INCH_NAME;
                    mLinearUnitMeter =  COLLADAFW::FileInfo::Unit::LINEAR_UNIT_INCH / COLLADAFW::FileInfo::Unit::LINEAR_UNIT_INCH;
                    break;
                }
            case COLLADAFW::FileInfo::Unit::YARD:
                {
                    // Don't convert 
                    linearUnitName = COLLADAFW::FileInfo::Unit::LINEAR_UNIT_YARD_NAME;
                    mLinearUnitMeter =  COLLADAFW::FileInfo::Unit::LINEAR_UNIT_YARD / COLLADAFW::FileInfo::Unit::LINEAR_UNIT_YARD;
                    break;
                }
            default:
                {
                    // Set to centimeters
                    linearUnitName = COLLADAFW::FileInfo::Unit::LINEAR_UNIT_CENTIMETER_NAME;
                    mLinearUnitMeter = COLLADAFW::FileInfo::Unit::LINEAR_UNIT_CENTIMETER;
                    break;
                }
            }
        }

        fprintf ( mFile, "currentUnit -l %s -a %s -t %s;\n", 
            linearUnitName.c_str (), unit.getAngularUnitName ().c_str (), unit.getTimeUnitName ().c_str () );

        if ( ImportOptions::importUpAxis () )
        {
            mUpAxisType = asset->getUpAxisType ();
        }
        else
        {
            mUpAxisType = asset->getUpAxisType ();
            String upAxis = "y";
            switch ( mUpAxisType )
            {
            case COLLADAFW::FileInfo::Y_UP:
                upAxis = "y"; break;
            case COLLADAFW::FileInfo::Z_UP:
                upAxis = "z"; break;
            default:
                upAxis = "y"; break;
            }
            // createNode script -name "upAxisScriptNode";
            //      setAttr ".before" -type "string" "string $currentAxis = `upAxis -q -ax`; if ($currentAxis != \"z\") { upAxis -ax \"z\"; viewSet -home persp; }";
            //      setAttr ".scriptType" 2;
            MayaDM::Script scriptNode ( mFile, "upAxisScriptNode" );
            String scriptValue = "string $currentAxis = `upAxis -q -ax`; if ($currentAxis != \\\"" + upAxis + "\\\") { upAxis -ax \\\"" + upAxis + "\\\"; viewSet -home persp; }"; // -rv
            
            scriptNode.setBefore ( scriptValue );
            scriptNode.setScriptType ( 2 );
        }

//         String application ( MGlobal::executeCommandStringResult ( "about -application" ).asChar () );
//         fprintf ( mFile, "fileInfo \"application\" \"%s\";\n", application.c_str () );
//         String product ( MGlobal::executeCommandStringResult ( "about -product" ).asChar () );
//         fprintf ( mFile, "fileInfo \"product\" \"%s\";\n", product.c_str () );
//         fprintf ( mFile, "fileInfo \"version\" \"%s\";\n", mayaVersion.c_str () );
//         String cutIdentifier ( MGlobal::executeCommandStringResult ( "product -cutIdentifier" ).asChar () );
//         fprintf ( mFile, "fileInfo \"cutIdentifier\" \"%s\";\n", cutIdentifier.c_str () );
//         String operatingSystemVersion ( MGlobal::executeCommandStringResult ( "product -operatingSystemVersion" ).asChar () );
//         fprintf ( mFile, "fileInfo \"osv\" \"%s\";\n", operatingSystemVersion.c_str () );

        mAssetWritten = true;

        if ( mSceneGraphRead || mLibraryNodesRead || mGeometryRead || mCameraRead || mLightRead || mImageRead )
        {
            mSceneGraphRead = false;
            mLibraryNodesRead = false;
            mGeometryRead = false;
            mCameraRead = false;
            mLightRead = false;
            mImageRead = false;
            readColladaDocument ();
        }

        return true;
    }

    //-----------------------------
    bool DocumentImporter::writeVisualScene ( const COLLADAFW::VisualScene* visualScene )
    {
        // Order: asset, scene graph, others
        if ( !mAssetWritten ) 
        {
            mSceneGraphRead = true;
            return true;
        }
        if ( mSceneGraphWritten ) return true;

        // Create the file, if not already done.
        if ( mFile == 0 ) start();

        // Import the data.
        mVisualSceneImporter->importVisualScene ( visualScene );
        mSceneGraphWritten = true;

        // Write the node instances, when all nodes of the visual scene 
        // and the library nodes are already imported!
        if ( mSceneGraphWritten && mLibraryNodesWritten )
        {
            // Write all the node instances
            mVisualSceneImporter->writeNodeInstances ();
        }

        if ( mGeometryRead || mCameraRead || mLightRead || mImageRead || mAnimationRead )
        {
            mGeometryRead = false;
            mCameraRead = false;
            mLightRead = false;
            mImageRead = false;
            mAnimationRead = false;
            readColladaDocument ();
        }

        return true;
    }

    //-----------------------------
    bool DocumentImporter::writeLibraryNodes ( const COLLADAFW::LibraryNodes* libraryNodes )
    {
        // Order: asset, scene graph, library nodes, others
        if ( !mAssetWritten ) 
        {
            mLibraryNodesRead = true;
            return true;
        }
        if ( mLibraryNodesWritten ) return true;

        // Create the file, if not already done.
        if ( mFile == 0 ) start();

        // Import the data.
        mVisualSceneImporter->importLibraryNodes ( libraryNodes );
        mLibraryNodesWritten = true;

        // Write the node instances, when all nodes of the visual scene 
        // and the library nodes are already imported!
        if ( mSceneGraphWritten && mLibraryNodesWritten )
        {
            // Write all the node instances
            mVisualSceneImporter->writeNodeInstances ();
        }

        if ( mGeometryRead || mCameraRead || mLightRead || mImageRead || mAnimationRead )
        {
            mGeometryRead = false;
            mCameraRead = false;
            mLightRead = false;
            mImageRead = false;
            mAnimationRead = false;
            readColladaDocument ();
        }

        return true;
    }

    //-----------------------------
    bool DocumentImporter::writeGeometry ( const COLLADAFW::Geometry* geometry )
    {
        // Order: asset, scene graph, library nodes, others
        if ( !mAssetWritten || !mSceneGraphWritten || !mLibraryNodesWritten ) 
        {
            mGeometryRead = true;
            return true;
        }

        // Create the file, if not already done.
        if ( mFile == 0 ) start();

        // Import the data.
        mGeometryImporter->importGeometry ( geometry );

        return true;
    }

    //-----------------------------
    bool DocumentImporter::writeMaterial ( const COLLADAFW::Material* material )
    {
        // Create the file, if not already done.
        if ( mFile == 0 ) start();

        // Import the data.
        mMaterialImporter->importMaterial ( material );

        return true;
    }

    //-----------------------------
    bool DocumentImporter::writeEffect ( const COLLADAFW::Effect* effect )
    {
        // Create the file, if not already done.
        if ( mFile == 0 ) start();

        // Import the data.
        mEffectImporter->importEffect ( effect );

        return true;
    }

    //-----------------------------
    bool DocumentImporter::writeCamera ( const COLLADAFW::Camera* camera )
    {
        // Order: asset, scene graph, library nodes, others
        if ( !mAssetWritten || !mSceneGraphWritten || !mLibraryNodesWritten ) 
        {
            mCameraRead = true;
            return true;
        }

        // Create the file, if not already done.
        if ( mFile == 0 ) start();

        // Import the data.
        mCameraImporter->importCamera ( camera );

        return true;
    }

    //-----------------------------
    bool DocumentImporter::writeLight ( const COLLADAFW::Light* light )
    {
        // Order: asset, scene graph, library nodes, others
        if ( !mAssetWritten || !mSceneGraphWritten || !mLibraryNodesWritten ) 
        {
            mLightRead = true;
            return true;
        }

        // Create the file, if not already done.
        if ( mFile == 0 ) start();

        // Import the data.
        mLightImporter->importLight ( light );

        return true;
    }

    //-----------------------------
    bool DocumentImporter::writeImage ( const COLLADAFW::Image* image )
    {
        // Order: asset, scene graph, library nodes, others
        if ( !mAssetWritten || !mSceneGraphWritten || !mLibraryNodesWritten ) 
        {
            mImageRead = true;
            return true;
        }

        // Create the file, if not already done.
        if ( mFile == 0 ) start();

        // Import the data.
        mImageImporter->importImage ( image );

        return true;
    }

    //-----------------------------
    bool DocumentImporter::writeAnimation ( const COLLADAFW::Animation* animation )
    {
        // Order: asset, scene graph, library nodes, others
        if ( !mAssetWritten || !mSceneGraphWritten || !mLibraryNodesWritten ) 
        {
            mAnimationRead = true;
            return true;
        }

        // Create the file, if not already done.
        if ( mFile == 0 ) start();

        mAnimationImporter->importAnimation ( animation );
        mAnimationsWritten = true;

        return true;
    }

    //-----------------------------
    bool DocumentImporter::writeAnimationList ( const COLLADAFW::AnimationList* animationList )
    {
        // Order: asset, scene graph, library nodes, others, animation list
        if ( !mAssetWritten || !mSceneGraphWritten || !mLibraryNodesWritten || !mAnimationsWritten ) 
        {
            return true;
        }

        // Create the file, if not already done.
        if ( mFile == 0 ) start();

        getAnimationImporter ()->writeConnections ( animationList );

        return true;
    }

}