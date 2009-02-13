/*
    Copyright (c) 2008-2009 NetAllied Systems GmbH

    This file is part of MayaDataModel.

    Licensed under the MIT Open Source License,
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/
#ifndef __MayaDM_ANIMCURVETT_H__
#define __MayaDM_ANIMCURVETT_H__
#include "MayaDMTypes.h"
#include "MayaDMConnectables.h"
#include "MayaDMAnimCurve.h"
namespace MayaDM
{
class AnimCurveTT : public AnimCurve
{
public:
	struct KeyTimeValue{
		void write(FILE* file) const
		{
		}
	};
public:
	AnimCurveTT():AnimCurve(){}
	AnimCurveTT(FILE* file,const std::string& name,const std::string& parent="",bool create=true)
		:AnimCurve(file, name, parent, "animCurveTT", create){}
	virtual ~AnimCurveTT(){}
	void setKeyTimeValue(size_t ktv_i,const KeyTimeValue& ktv)
	{
		fprintf(mFile,"\tsetAttr \".ktv[%i]\" ",ktv_i);
		ktv.write(mFile);
		fprintf(mFile,";\n");

	}
	void getInput()const
	{
		fprintf(mFile,"\"%s.i\"",mName.c_str());

	}
	void getOutput()const
	{
		fprintf(mFile,"\"%s.o\"",mName.c_str());

	}
protected:
	AnimCurveTT(FILE* file,const std::string& name,const std::string& parent,const std::string& nodeType,bool create=true)
		:AnimCurve(file, name, parent, nodeType, create) {}

};
}//namespace MayaDM
#endif//__MayaDM_ANIMCURVETT_H__
