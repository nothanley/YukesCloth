#include <vector>
#include "CSimObj.h"
#include <BinaryIO.h>
#include "CClothStructs.h"
#include "CSimMeshData.h"
using namespace BinaryIO;

#ifdef DEBUG_CONSOLE
#include <iostream> 
#endif 

CSimObj::CSimObj(std::ifstream* fs) {
	this->m_pDataStream = fs;
}

void 
CSimObj::UpdateStrings() {

    for (auto child : m_pStHead->children) {
        uint32_t type = child->eType;

        if (type == enTagType_SimMesh) {
			child->pSimMesh->modelName = m_sStringTable.at(child->pSimMesh->modelNameIndex);
			child->pSimMesh->sObjName  = m_sStringTable.at(child->pSimMesh->subObjNameIndex);
		}
        else if(type == enTagType_SimLine){
            child->pSimMesh->modelName = m_sStringTable.at(child->pSimMesh->sName);
            child->pSimMesh->sObjName = "Line";
        }

	}

}

void
CSimObj::Create() {
	/* Initialize Tag Header */
	m_pStHead = new StTag{ _U32,_U32,_U32,_U32 };

#ifdef DEBUG_CONSOLE
	printf("\n====== StHead {sTagCount:%d, eType:%d, sSize:%d, sTotalSize:%d} ======\n",
			m_pStHead->sTagCount, m_pStHead->eType, m_pStHead->sSize, m_pStHead->sTotalSize);
#endif

	/* Update stream pointer */
	m_iStreamPos += m_pStHead->sSize;
	
	/* Iterate and collect all child nodes */
	for (int i = 0; i < m_pStHead->sTagCount; i++) {
		GetTag(m_iStreamPos, m_pStHead);
	}

	SetupTags(m_pStHead);
	UpdateStrings();
};

void
CSimObj::SetupTags(StTag* pParentTag) {

	/* Setup parent tag */
	InitTag(*pParentTag);

	/* Setup and LOAD all child nodes */
	for (int i = 0; i < pParentTag->children.size(); i++) {

		if (pParentTag->pSimMesh)
			pParentTag->children.at(i)->pSimMesh = pParentTag->pSimMesh;

		SetupTags( pParentTag->children.at(i) );
	}

}

StTag*
CSimObj::GetTag(uintptr_t& streamBegin, StTag* pParentTag) {
	/* Seek to data begin address*/
	m_pDataStream->seekg(streamBegin);

	/* Init Struct Tag data */
	StTag* stDataTag = new StTag{ _U32,_U32,_U32 };
	stDataTag->streamPointer = streamBegin + 0xC;

#ifdef DEBUG_CONSOLE
	printf("\n[0x%x]\t- StTag {eType:%d, sSize:%d, sTotalSize:%d}\n",
		m_iStreamPos, stDataTag->eType, stDataTag->sSize, stDataTag->sTotalSize);
#endif
	
	/* Get number of child nodes */
	uint32_t numNodes = yclutils::hasIndex(rootNodes,stDataTag->eType) ? _U32 : 0;

	/* Add node to parent vector */
	stDataTag->pParent = pParentTag;
	pParentTag->children.push_back(stDataTag);

	/* Collect all child nodes */
	streamBegin += (numNodes == 0) ? stDataTag->sTotalSize : stDataTag->sSize;

	for (uint32_t i = 0; i < numNodes; i++) {
		StTag* childNode = GetTag(streamBegin, stDataTag);
	}

	return stDataTag;
}


void
CSimObj::InitTag(StTag& tag) {

	if (tag.eType == 0x0) { return; }

	this->m_iStreamPos = tag.streamPointer-0xC;
	this->m_pDataStream->seekg(tag.streamPointer);


#ifdef DEBUG_DISP_BINARY
    m_pDataStream->seekg(m_iStreamPos);
    tag.data.resize(tag.sTotalSize);

    // Read the data from the file into the vector
    m_pDataStream->read(reinterpret_cast<char*>(tag.data.data()), tag.sTotalSize);
    m_pDataStream->seekg(tag.streamPointer);
#endif

	switch (tag.eType) {
		case enTagType_SimMesh:
			CSimMeshData::GetSimMesh(tag,this);
			break;
		case enTagType_SimMesh_AssignSubObj:
			CSimMeshData::AssignSubObj(*tag.pSimMesh,this);
			break;
		case enTagType_SimMesh_AssignSubObjVtx:
			CSimMeshData::AssignSubObjVtx(*tag.pSimMesh,this);
			break;
		case enTagType_SimMesh_AssignSimVtx:
			CSimMeshData::AssignSimVtx(*tag.pSimMesh,this);
			break;
		case enTagType_SimMesh_RCN:
			CSimMeshData::GetRCNData(*tag.pSimMesh,this);
			break;
		case enTagType_SimMesh_RCNSubObj:
			CSimMeshData::GetRecalcNormals(*tag.pSimMesh, this);
			break;
		case enTagType_SimMesh_Skin:
//			printf("\nSkin Data Buffer at %d", uint64_t(m_pDataStream->tellg()));
			CSimMeshData::GetSkinData(*tag.pSimMesh,this);
			break;
		case enTagType_SimMesh_SimLinkSrc:
			CSimMeshData::Link_DefineSourceMesh(*tag.pSimMesh,this);
			break;
		case enTagType_SimMesh_Pattern:
			CSimMeshData::GetSimMeshPattern(*tag.pSimMesh,this);
			break;
		case enTagType_SimMesh_Stacks:
            CSimMeshData::GetSimMeshStacks(tag,this);
			break;
		case enTagType_SimMesh_SkinCalc:
			CSimMeshData::GetSkinCalc(*tag.pSimMesh,this);
			break;
		case enTagType_SimMesh_SkinPaste:
			CSimMeshData::GetSkinPaste(*tag.pSimMesh,this);
			break;
		case enTagType_SimMesh_OldVtxSave:
			CSimMeshData::SaveOldVtxs(*tag.pSimMesh,this);
			break;
		case enTagType_SimMesh_Force:
			CSimMeshData::GetForce(*tag.pSimMesh,this);
			break;
		case enTagType_SimMesh_CtStretchLink:
			CSimMeshData::GetConstraintStretchLink(*tag.pSimMesh,this);
			break;
		case enTagType_SimMesh_CtStdLink:
			CSimMeshData::GetConstraintStandardLink(*tag.pSimMesh,this);
			break;
		case enTagType_SimMesh_CtBendLink:
			CSimMeshData::GetConstraintBendLink(*tag.pSimMesh,this);
			break;
		case enTagType_SimMesh_BendingStiffness:
			CSimMeshData::GetBendStiffness(*tag.pSimMesh,this);
			break;
		case enTagType_SimMesh_ColVtx:
			CSimMeshData::GetCollisionVerts(*tag.pSimMesh,this);
			break;
		case enTagType_SimMesh_CtFixation:
			CSimMeshData::GetConstraintFixation(*tag.pSimMesh,this);
			break;
		case enTagType_SimLine:
            CSimMeshData::GetSimLine(tag,this);
			break;
		case enTagType_SimLine_LineDef:
			CSimMeshData::GetLineDef(*tag.pSimMesh, this);
			break;
        case enTagType_StrTbl:
            CSimMeshData::GetStringTable(this);
            break;
		case enTagType_String:
            tag.sTagName = CSimMeshData::GetString(this);
			break;
		default:
//			printf("No Suitable Operator Found for TagType: %d\n", tag.eType);
			break;

	}
}


















