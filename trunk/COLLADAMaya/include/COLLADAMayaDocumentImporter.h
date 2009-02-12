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

#ifndef __COLLADA_MAYA_DOCUMENT_IMPORTER_H__
#define __COLLADA_MAYA_DOCUMENT_IMPORTER_H__

#include "COLLADAMayaStableHeaders.h"
#include "COLLADAMayaPrerequisites.h"
#include "COLLADAMayaNode.h"

#include "COLLADAFWIWriter.h"
#include "COLLADAFWFileInfo.h"
#include "COLLADAFWVisualScene.h"
#include "COLLADAFWGeometry.h"
#include "COLLADAFWMaterial.h"
#include "COLLADAFWEffect.h"

#include "MayaDMMesh.h"

#include <set>


namespace COLLADAMaya
{
    class VisualSceneImporter;
    class GeometryImporter;
    class MaterialImporter;
    class EffectImporter;


    /** The main importer class. This class imports all data of the scene. */
    class DocumentImporter : public COLLADAFW::IWriter 
    {

    private:

        /** The name of the collada file. */
        String mColladaFileName;

        /** The name of the current maya ascii file. */
        COLLADABU::URI mMayaAsciiFileURI;

        /** The id of the current scene. */
        String mSceneId;

        /** The current maya ascii file to import the data. */
        FILE* mFile;

        /** The LC_NUMERIC locale that was set before the Streamwriter was instantiated. */
        String mLocale;			

        /**
        * How many real-world meters in one distance unit as a floating-point number.
        * For example, 1.0 for the name "meter"; 1000 for the name "kilometer";
        * 0.3048 for the name "foot".
         */
        double mLinearUnitMeter;
        COLLADAFW::FileInfo::UpAxisType mUpAxisType;

        bool mAssetWritten;
        bool mSceneGraphWritten;

        bool mSceneGraphRead;
        bool mGeometryRead;

        /** Pointer to the visual scene importer. */
        VisualSceneImporter* mVisualSceneImporter;

        /** Pointer to the geometry importer. */
        GeometryImporter* mGeometryImporter;

        /** Pointer to the geometry importer. */
        MaterialImporter* mMaterialImporter;

        /** Pointer to the geometry importer. */
        EffectImporter* mEffectImporter;

        /** The variable tells, how many times the document is read. */
        size_t mNumDocumentParses;

    public:

        /** Constructor. */
        DocumentImporter ( const String& fileName );

        /** Destructor. */
        virtual ~DocumentImporter ();

        /** The current maya ascii file to import the data. */
        FILE* getFile () const { return mFile; }
        void setFile ( FILE* val ) { mFile = val; }

        /** Imports the current scene. */
        void importCurrentScene ();

        /** Reads the collada document. */
        void readColladaDocument ();

        /** Create the maya ascii file (where with which name???) */
        bool createMayaAsciiFile ();
        void closeMayaAsciiFile ();

        /**
        * Returns the name of the current collada file to export.
        * @return const String& Name of the current collada file
        */
        const String& getColladaFilename () const;

        /**
        * Returns the name of the current maya ascii file to export.
        * @return const String& The current maya ascii file
        */
        const COLLADABU::URI& getMayaAsciiFileURI () const;

        /** Pointer to the visual scene importer. */
        VisualSceneImporter* getVisualSceneImporter () { return mVisualSceneImporter; }
        const VisualSceneImporter* getVisualSceneImporter () const { return mVisualSceneImporter; }

        /** Pointer to the geometry importer. */
        GeometryImporter* getGeometryImporter () { return mGeometryImporter; }
        const GeometryImporter* getGeometryImporter () const { return mGeometryImporter; }

        /** Pointer to the geometry importer. */
        MaterialImporter* getMaterialImporter () { return mMaterialImporter; }
        const MaterialImporter* getMaterialImporter () const { return mMaterialImporter; }

        /** Pointer to the geometry importer. */
        EffectImporter* getEffectImporter () { return mEffectImporter; }
        const EffectImporter* getEffectImporter () const { return mEffectImporter; }

        /** This method will be called if an error in the loading process occurred and the loader 
        cannot continue to to load. The writer should undo all operations that have been performed.
        @param errorMessage A message containing informations about the error that occurred.
        */
        virtual void cancel ( const String& errorMessage );

        /** This is the method called. The writer hast to prepare to receive data.*/
        virtual void start ();

        /** 
        * This method is called after the last write method. No other methods will be called after this.
        * After the read of the collada document, the connections can be written into the maya file.
        */
        virtual void finish ();

        /** Start the import of the model.
        @return True on success, false otherwise. */
        bool import();

        /** When this method is called, the writer must write the global document asset.
        @return The writer should return true, if writing succeeded, false otherwise.*/
        virtual bool writeGlobalAsset ( const COLLADAFW::FileInfo* asset );

        void getCurrentDate ( std::stringstream& curDate );

        /** Returns the type of the current up axis. */
        const COLLADAFW::FileInfo::UpAxisType& getUpAxisType () const { return mUpAxisType; }

        /**
        * How many real-world meters in one distance unit as a floating-point number.
        * For example, 1.0 for the name "meter"; 1000 for the name "kilometer";
        * 0.3048 for the name "foot".
        */
        const double getLinearUnitMeter () const { return mLinearUnitMeter; }

        /** When this method is called, the writer must write the entire visual scene.
        @return The writer should return true, if writing succeeded, false otherwise.*/
        virtual bool writeVisualScene ( const COLLADAFW::VisualScene* visualScene );

        /** When this method is called, the writer must write the geometry.
        @return The writer should return true, if writing succeeded, false otherwise.*/
        virtual bool writeGeometry ( const COLLADAFW::Geometry* geometry );

        /** When this method is called, the writer must handle all nodes contained in the 
        library nodes.
        @return The writer should return true, if writing succeeded, false otherwise.*/
        virtual bool writeLibraryNodes ( const COLLADAFW::LibraryNodes* libraryNodes );

        /** When this method is called, the writer must write the material.
        @return The writer should return true, if writing succeeded, false otherwise.*/
        virtual bool writeMaterial ( const COLLADAFW::Material* material );

        /** When this method is called, the writer must write the effect.
        @return The writer should return true, if writing succeeded, false otherwise.*/
        virtual bool writeEffect ( const COLLADAFW::Effect* effect );

        /** When this method is called, the writer must write the camera.
        @return The writer should return true, if writing succeeded, false otherwise.*/
        virtual bool writeCamera( const COLLADAFW::Camera* camera ) 
        {
            return true;
        }

    private:

        /** Imports the current scene. */
        void exportScene();

        /** Parse the current collada file and creates the parsing libraries: 
        we want access to the libraries during import/export time. */
        void createLibraries();

        /** Releases the import/export libraries */
        void releaseLibraries();

    };
}

#endif // __COLLADA_MAYA_DOCUMENT_IMPORTER_H__