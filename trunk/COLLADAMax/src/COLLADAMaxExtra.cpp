/*
    Copyright (c) 2008 NetAllied Systems GmbH

	This file is part of COLLADAMax.

    Portions of the code are:
    Copyright (c) 2005-2007 Feeling Software Inc.
    Copyright (c) 2005-2007 Sony Computer Entertainment America
    
    Based on the 3dsMax COLLADA Tools:
    Copyright (c) 2005-2006 Autodesk Media Entertainment
	
    Licensed under the MIT Open Source License, 
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/


#include "ColladaMaxStableHeaders.h"

#include "COLLADAMaxExtra.h"



namespace COLLADAMax
{


    const String Extra::TECHNIQUE_PROFILE = "MAX3D";


    //---------------------------------------------------------------
    Extra::Extra ( COLLADA::StreamWriter * streamWriter )
            : COLLADA::ElementWriter ( streamWriter ),
            mExtra ( streamWriter ),
            mTechnique ( streamWriter )
    {}


    //---------------------------------------------------------------
    void Extra::openTechnique()
    {
        mExtra.openExtra();
        mTechnique.openTechnique ( TECHNIQUE_PROFILE );
    }

    //---------------------------------------------------------------
    void Extra::closeExtra()
    {
        mExtra.closeExtra();
    }
}