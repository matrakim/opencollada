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

#ifndef __COLLADA_MAYA_CAMERA_EXPORTER_H__
#define __COLLADA_MAYA_CAMERA_EXPORTER_H__

#include "COLLADAMayaStableHeaders.h"
#include "COLLADAStreamWriter.h"
#include "COLLADALibraryCameras.h"


namespace COLLADAMaya
{

    /** This class writes the <library_cameras>. */
    class CameraExporter : public COLLADA::LibraryCameras
    {
    private:

        /** Pointer to the document exporter */
        DocumentExporter* mDocumentExporter;

    public:
        /**
        * @param streamWriter The stream the output will be written to
        * @param documentExporter The document exporter this material exporter is used in
        */
        CameraExporter ( COLLADA::StreamWriter* streamWriter, DocumentExporter* documentExporter );
        virtual ~CameraExporter() {};

        /** Exports the cameras in the scene graph. */
        void exportCameras (  );

        /** Exports the current scene element and all it's children. */
        void exportCameras ( SceneElement* sceneElement );

    private:

        /** Exports the geometry data of the current object, if it is a valid mesh object to export. */
        bool exportCamera ( const MDagPath& dagPath );

    };


}

#endif //__COLLADA_MAYA_CAMERA_EXPORTER_H__
