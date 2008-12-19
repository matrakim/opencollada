/*
    Copyright (c) 2008 NetAllied Systems GmbH

    This file is part of MayaDataModel.

    Licensed under the MIT Open Source License,
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/
#ifndef __MayaDM_POLYPRIMITIVE_H__
#define __MayaDM_POLYPRIMITIVE_H__
#include "MayaDMTypes.h"
#include "MayaDMConnectables.h"
#include "MayaDMPolyCreator.h"
namespace MayaDM
{
class PolyPrimitive : public PolyCreator
{
public:
public:
	PolyPrimitive(FILE* file,const std::string& name,const std::string& parent=""):PolyCreator(file, name, parent, "polyPrimitive"){}
	virtual ~PolyPrimitive(){}
	void setAxis(const double3& ax)
	{
		fprintf(mFile,"setAttr \".ax\" -type \"double3\" ");
		ax.write(mFile);
		fprintf(mFile,";\n");

	}
	void setAxisX(double axx)
	{
		if(axx == 0) return;
		fprintf(mFile,"setAttr \".ax.axx\" %f;\n", axx);

	}
	void setAxisY(double axy)
	{
		if(axy == 1) return;
		fprintf(mFile,"setAttr \".ax.axy\" %f;\n", axy);

	}
	void setAxisZ(double axz)
	{
		if(axz == 0) return;
		fprintf(mFile,"setAttr \".ax.axz\" %f;\n", axz);

	}
	void setParamWarn(bool pw)
	{
		if(pw == true) return;
		fprintf(mFile,"setAttr \".pw\" %i;\n", pw);

	}
	void setUvSetName(const string& uvs)
	{
		if(uvs == "NULL") return;
		fprintf(mFile,"setAttr \".uvs\" -type \"string\" ");
		uvs.write(mFile);
		fprintf(mFile,";\n");

	}
	void getAxis()
	{
		fprintf(mFile,"\"%s.ax\"",mName.c_str());

	}
	void getAxisX()
	{
		fprintf(mFile,"\"%s.ax.axx\"",mName.c_str());

	}
	void getAxisY()
	{
		fprintf(mFile,"\"%s.ax.axy\"",mName.c_str());

	}
	void getAxisZ()
	{
		fprintf(mFile,"\"%s.ax.axz\"",mName.c_str());

	}
	void getParamWarn()
	{
		fprintf(mFile,"\"%s.pw\"",mName.c_str());

	}
	void getUvSetName()
	{
		fprintf(mFile,"\"%s.uvs\"",mName.c_str());

	}
protected:
	PolyPrimitive(FILE* file,const std::string& name,const std::string& parent,const std::string& nodeType):PolyCreator(file, name, parent, nodeType) {}

};
}//namespace MayaDM
#endif//__MayaDM_POLYPRIMITIVE_H__
