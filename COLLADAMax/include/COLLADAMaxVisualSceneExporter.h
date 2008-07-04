/*
    Copyright (c) 2008 NetAllied Systems GmbH
	
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/


#ifndef __COLLADAMAX_VISUALSCENEEXPORTER_H__
#define __COLLADAMAX_VISUALSCENEEXPORTER_H__

#include "COLLADAMaxPrerequisites.h"

#include "COLLADAStreamWriter.h"
#include "COLLADAMaxEffectExporter.h"
#include "COLLADALibraryVisualScenes.h"
#include "COLLADAMaxDocumentExporter.h"

namespace COLLADA
{

    class Node;
}

namespace COLLADAMax
{

    class ExportSceneGraph;

    class ExportNode;

    /** Class to export the visual scene.*/

    class VisualSceneExporter : public COLLADA::LibraryVisualScenes
    {

    public:
        /** Constructor
        @param streamWriter The stream to write the visual seen to.
        @param exportSceneGraph The scene graph of all the nodes that should be exported.
        @param sceneId The id of the scene.
        @param documentExporter
        */
        VisualSceneExporter ( COLLADA::StreamWriter * streamWriter, ExportSceneGraph * exportSceneGraph, const String & sceneId, DocumentExporter * documentExporter );
        virtual ~VisualSceneExporter()
        {}

        ;

        /** Exports the library visual scene.*/
        void doExport();

        static const String TRANSLATE_SID;
        static const String ROTATE_SID;
        static const String ROTATE_X_SID;
        static const String ROTATE_Y_SID;
        static const String ROTATE_Z_SID;
        static const String ROTATE_SCALE_AXIS_SID;
        static const String ROTATE_SCALE_AXIS_INVERSE_SID;
        static const String SCALE_SID;

        static const String TRANSLATION_PARAMETERS[ 3 ];
        static const String ROTATION_PARAMETER[ 1 ];
        static const String ROTATION_PARAMETERS[ 4 ];


    private:
        ExportSceneGraph * mExportSceneGraph;

        const EffectMap & mEffectMap;

        /** The id of the visual scene.*/
        const String & mVisualSceneId;

        DocumentExporter * mDocumentExporter;

        /** The praefix used for node ids.*/
        static const String NODE_ID_PRAEFIX;

        /** Exports all the nodes in @a node and all its child nodes.*/
        void doExport ( ExportNode* exportNode );

        /** Exports all the transformations of a node.*/
        void exportTransformations ( ExportNode * exportNode, const COLLADA::Node & colladaNode );


        /** Fills the two dimensional array @a copy with the values contained in @a original.*/
        static void Matrix3ToDouble4x4 ( double copy[][ 4 ], const Matrix3 & original );


    };

}

#endif //__COLLADAMAX_VISUALSCENEEXPORTER_H__
