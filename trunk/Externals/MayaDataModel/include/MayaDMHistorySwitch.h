/*
    Copyright (c) 2008 NetAllied Systems GmbH

    This file is part of MayaDataModel.

    Licensed under the MIT Open Source License,
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/
#ifndef __MayaDM_HISTORYSWITCH_H__
#define __MayaDM_HISTORYSWITCH_H__
#include "MayaDMTypes.h"
#include "MayaDMConnectables.h"
#include "MayaDMGeometryFilter.h"
namespace MayaDM
{
class HistorySwitch : public GeometryFilter
{
public:
public:
	HistorySwitch(FILE* file,const std::string& name,const std::string& parent=""):GeometryFilter(file, name, parent, "historySwitch"){}
	virtual ~HistorySwitch(){}
	void setPlayFromCache(bool pfc)
	{
		if(pfc == false) return;
		fprintf(mFile,"setAttr \".pfc\" %i;\n", pfc);

	}
	void getUndeformedGeometry(size_t ug_i)
	{
		fprintf(mFile,"\"%s.ug[%i]\"",mName.c_str(),ug_i);

	}
	void getPlayFromCache()
	{
		fprintf(mFile,"\"%s.pfc\"",mName.c_str());

	}
	void getInPositions(size_t inp_i)
	{
		fprintf(mFile,"\"%s.inp[%i]\"",mName.c_str(),inp_i);

	}
	void getOutPositions(size_t op_i)
	{
		fprintf(mFile,"\"%s.op[%i]\"",mName.c_str(),op_i);

	}
protected:
	HistorySwitch(FILE* file,const std::string& name,const std::string& parent,const std::string& nodeType):GeometryFilter(file, name, parent, nodeType) {}

};
}//namespace MayaDM
#endif//__MayaDM_HISTORYSWITCH_H__
