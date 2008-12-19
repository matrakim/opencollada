/*
    Copyright (c) 2008 NetAllied Systems GmbH

    This file is part of MayaDataModel.

    Licensed under the MIT Open Source License,
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/
#ifndef __MayaDM_DIRECTEDDISC_H__
#define __MayaDM_DIRECTEDDISC_H__
#include "MayaDMTypes.h"
#include "MayaDMConnectables.h"
#include "MayaDMGeometryShape.h"
namespace MayaDM
{
class DirectedDisc : public GeometryShape
{
public:
	DirectedDisc(FILE* file,const std::string& name,const std::string& parent=""):GeometryShape(file, name, parent, "directedDisc"){}
	virtual ~DirectedDisc(){}
protected:
	DirectedDisc(FILE* file,const std::string& name,const std::string& parent,const std::string& nodeType):GeometryShape(file, name, parent, nodeType) {}

};
}//namespace MayaDM
#endif//__MayaDM_DIRECTEDDISC_H__
