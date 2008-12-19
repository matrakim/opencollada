/*
    Copyright (c) 2008 NetAllied Systems GmbH

    This file is part of MayaDataModel.

    Licensed under the MIT Open Source License,
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/
#ifndef __MayaDM_POLYUNITE_H__
#define __MayaDM_POLYUNITE_H__
#include "MayaDMTypes.h"
#include "MayaDMConnectables.h"
#include "MayaDMPolyCreator.h"
namespace MayaDM
{
class PolyUnite : public PolyCreator
{
public:
public:
	PolyUnite(FILE* file,const std::string& name,const std::string& parent=""):PolyCreator(file, name, parent, "polyUnite"){}
	virtual ~PolyUnite(){}
	void setUseOldPolyArchitecture(bool uopa)
	{
		if(uopa == false) return;
		fprintf(mFile,"setAttr \".uopa\" %i;\n", uopa);

	}
	void getInputPoly(size_t ip_i)
	{
		fprintf(mFile,"\"%s.ip[%i]\"",mName.c_str(),ip_i);

	}
	void getInputMat(size_t im_i)
	{
		fprintf(mFile,"\"%s.im[%i]\"",mName.c_str(),im_i);

	}
protected:
	PolyUnite(FILE* file,const std::string& name,const std::string& parent,const std::string& nodeType):PolyCreator(file, name, parent, nodeType) {}

};
}//namespace MayaDM
#endif//__MayaDM_POLYUNITE_H__
