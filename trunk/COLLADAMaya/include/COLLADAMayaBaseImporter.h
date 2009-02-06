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

#ifndef __COLLADA_MAYA_BASE_IMPORTER_H__
#define __COLLADA_MAYA_BASE_IMPORTER_H__

#include "COLLADAMayaStableHeaders.h"
#include "COLLADAMayaDocumentImporter.h"

#include "math/COLLADABUMathVector3.h"

#include "MayaDMTypes.h"


namespace COLLADAMaya
{

    /** Declares the base import functionality. 
    This is the base class for all importers. */
    class BaseImporter
    {

    private:

        /** Pointer to the current document importer. */
        DocumentImporter* mDocumentImporter;

    public:

        /** Constructor. */
        BaseImporter ( DocumentImporter* documentImporter )
        : mDocumentImporter ( documentImporter )
        {}

        /** Destructor. */
        virtual ~BaseImporter () {}

        /**
        * Converts the given value to the linear unit in meters.
        * How many real-world meters in one distance unit as a floating-point number.
        * For example, 1.0 for the name "meter"; 1000 for the name "kilometer";
        * 0.3048 for the name "foot".
        */
        MayaDM::double3 toLinearUnit ( const MayaDM::double3& val )
        {
            if ( mDocumentImporter->getUpAxisType () == COLLADAFW::FileInfo::X_UP )
                return ( MayaDM::double3 ( toLinearUnit ( val[1] ), toLinearUnit ( val[0]*(-1) ), toLinearUnit ( val[2] ) ) );
            else if ( mDocumentImporter->getUpAxisType () == COLLADAFW::FileInfo::Z_UP )
                return ( MayaDM::double3 ( toLinearUnit ( val[0] ), toLinearUnit ( val[2] ), toLinearUnit ( val[1]*(-1) ) ) );
            else return ( MayaDM::double3 ( toLinearUnit ( val[0] ), toLinearUnit ( val[1] ), toLinearUnit ( val[2] ) ) );
        }

        /**
        * Converts the given value to the linear unit in meters.
        * How many real-world meters in one distance unit as a floating-point number.
        * For example, 1.0 for the name "meter"; 1000 for the name "kilometer";
        * 0.3048 for the name "foot".
        */
        MVector toLinearUnit ( const MVector& val )
        {
            if ( mDocumentImporter->getUpAxisType () == COLLADAFW::FileInfo::X_UP )
                return ( MVector ( toLinearUnit ( val[1] ), toLinearUnit ( val[0]*(-1) ), toLinearUnit ( val[2] ) ) );
            else if ( mDocumentImporter->getUpAxisType () == COLLADAFW::FileInfo::Z_UP )
                return ( MVector ( toLinearUnit ( val[0] ), toLinearUnit ( val[2] ), toLinearUnit ( val[1]*(-1) ) ) );
            else return ( MVector ( toLinearUnit ( val[0] ), toLinearUnit ( val[1] ), toLinearUnit ( val[2] ) ) );
        }

        /**
        * Converts the given value to the linear unit in meters.
        * How many real-world meters in one distance unit as a floating-point number.
        * For example, 1.0 for the name "meter"; 1000 for the name "kilometer";
        * 0.3048 for the name "foot".
        */
        void toLinearUnit ( const double val0, const double val1, const double val2, COLLADABU::Math::Vector3& retVal )
        {
            if ( mDocumentImporter->getUpAxisType () == COLLADAFW::FileInfo::X_UP )
            {
                retVal [0] = toLinearUnit ( val1 );
                retVal [1] = toLinearUnit ( val0*(-1) );
                retVal [2] = toLinearUnit ( val2 );
            }
            else if ( mDocumentImporter->getUpAxisType () == COLLADAFW::FileInfo::Z_UP )
            {
                retVal [0] = toLinearUnit ( val0 );
                retVal [1] = toLinearUnit ( val2 );
                retVal [2] = toLinearUnit ( val1*(-1) );
            }
            else 
            {
                retVal [0] = toLinearUnit ( val0 );
                retVal [1] = toLinearUnit ( val1 );
                retVal [2] = toLinearUnit ( val2 );
            }
        }

    protected:

        /** Returns a pointer to the current document importer. */
        DocumentImporter* getDocumentImporter () { return mDocumentImporter; }

        /** Returns a pointer to the current document importer. */
        const DocumentImporter* getDocumentImporter () const { return mDocumentImporter; }

//         /** Pointer to the current dae document. */
//         daeDocument* getDaeDocument () 
//         { 
//             return mDocumentImporter->getDocumentLoader ().getDaeDocument (); 
//         }
// 
//         /** Pointer to the current dae document. */
//         const daeDocument* getDaeDocument () const 
//         { 
//             return mDocumentImporter->getDocumentLoader ().getDaeDocument (); 
//         }
// 
//         /** Returns the current document loader. */
//         COLLADADH::Loader& getDocumentLoader ()
//         {
//             return mDocumentImporter->getDocumentLoader ();
//         }
// 
//         /** Returns the current document loader. */
//         const COLLADADH::Loader& getDocumentLoader () const
//         {
//             return mDocumentImporter->getDocumentLoader ();
//         }

    private:

        /**
        * Converts the given value to the linear unit in meters.
        * How many real-world meters in one distance unit as a floating-point number.
        * For example, 1.0 for the name "meter"; 1000 for the name "kilometer";
        * 0.3048 for the name "foot".
        */
        double toLinearUnit ( const double val )
        {
            return ( val * mDocumentImporter->getLinearUnitMeter () );
        }

        /**
        * Converts the given value to the linear unit in meters.
        * How many real-world meters in one distance unit as a floating-point number.
        * For example, 1.0 for the name "meter"; 1000 for the name "kilometer";
        * 0.3048 for the name "foot".
        */
        float toLinearUnit ( const float val )
        {
            return ( val * (float) mDocumentImporter->getLinearUnitMeter () );
        }

    };
}

#endif // __COLLADA_MAYA_BASE_IMPORTER_H__