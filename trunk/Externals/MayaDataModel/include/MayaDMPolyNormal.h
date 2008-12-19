/*
    Copyright (c) 2008 NetAllied Systems GmbH

    This file is part of MayaDataModel.

    Licensed under the MIT Open Source License,
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/
#ifndef __MayaDM_POLYNORMAL_H__
#define __MayaDM_POLYNORMAL_H__
#include "MayaDMTypes.h"
#include "MayaDMConnectables.h"
#include "MayaDMPolyModifier.h"
namespace MayaDM
{
class PolyNormal : public PolyModifier
{
public:
public:
	PolyNormal(FILE* file,const std::string& name,const std::string& parent=""):PolyModifier(file, name, parent, "polyNormal"){}
	virtual ~PolyNormal(){}
	void setNormalMode(unsigned int nm)
	{
		if(nm == 0) return;
		fprintf(mFile,"setAttr \".nm\" %i;\n", nm);

	}
	void setUserNormalMode(bool unm)
	{
		if(unm == true) return;
		fprintf(mFile,"setAttr \".unm\" %i;\n", unm);

	}
	void getNormalMode()
	{
		fprintf(mFile,"\"%s.nm\"",mName.c_str());

	}
	void getUserNormalMode()
	{
		fprintf(mFile,"\"%s.unm\"",mName.c_str());

	}
protected:
	PolyNormal(FILE* file,const std::string& name,const std::string& parent,const std::string& nodeType):PolyModifier(file, name, parent, nodeType) {}

};
}//namespace MayaDM
#endif//__MayaDM_POLYNORMAL_H__
