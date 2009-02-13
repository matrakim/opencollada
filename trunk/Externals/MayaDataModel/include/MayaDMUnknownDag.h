/*
    Copyright (c) 2008-2009 NetAllied Systems GmbH

    This file is part of MayaDataModel.

    Licensed under the MIT Open Source License,
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/
#ifndef __MayaDM_UNKNOWNDAG_H__
#define __MayaDM_UNKNOWNDAG_H__
#include "MayaDMTypes.h"
#include "MayaDMConnectables.h"
#include "MayaDMDagNode.h"
namespace MayaDM
{
class UnknownDag : public DagNode
{
public:
	UnknownDag():DagNode(){}
	UnknownDag(FILE* file,const std::string& name,const std::string& parent="",bool create=true)
		:DagNode(file, name, parent, "unknownDag", create){}
	virtual ~UnknownDag(){}
protected:
	UnknownDag(FILE* file,const std::string& name,const std::string& parent,const std::string& nodeType,bool create=true)
		:DagNode(file, name, parent, nodeType, create) {}

};
}//namespace MayaDM
#endif//__MayaDM_UNKNOWNDAG_H__
