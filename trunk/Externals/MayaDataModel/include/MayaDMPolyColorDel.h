/*
    Copyright (c) 2008 NetAllied Systems GmbH

    This file is part of MayaDataModel.

    Licensed under the MIT Open Source License,
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/
#ifndef __MayaDM_POLYCOLORDEL_H__
#define __MayaDM_POLYCOLORDEL_H__
#include "MayaDMTypes.h"
#include "MayaDMConnectables.h"
#include "MayaDMPolyModifier.h"
namespace MayaDM
{
class PolyColorDel : public PolyModifier
{
public:
public:
	PolyColorDel(FILE* file,const std::string& name,const std::string& parent=""):PolyModifier(file, name, parent, "polyColorDel"){}
	virtual ~PolyColorDel(){}
	void setColorSetName(const string& cls)
	{
		if(cls == "NULL") return;
		fprintf(mFile,"setAttr \".cls\" -type \"string\" ");
		cls.write(mFile);
		fprintf(mFile,";\n");

	}
	void getColorSetName()
	{
		fprintf(mFile,"\"%s.cls\"",mName.c_str());

	}
protected:
	PolyColorDel(FILE* file,const std::string& name,const std::string& parent,const std::string& nodeType):PolyModifier(file, name, parent, nodeType) {}

};
}//namespace MayaDM
#endif//__MayaDM_POLYCOLORDEL_H__
