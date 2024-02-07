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
};

void
CSimObj::SetupTags(StTag* pParentTag) {

	/* Setup parent tag */
	InitTag(*pParentTag);

	/* Setup and LOAD all child nodes */
	for (int i = 0; i < pParentTag->children.size(); i++) {
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
		GetTag(streamBegin, stDataTag);
	}

	return stDataTag;
}


void
CSimObj::InitTag(StTag& tag) {

	this->m_iStreamPos = tag.streamPointer-0xC;
	m_pDataStream->seekg(m_iStreamPos);

	switch (tag.eType) {
		case enTagType_SimMesh:
			CSimMeshData::GetSimMesh(tag,this);
			break;
		case enTagType_SimMesh_AssignSubObj:
			CSimMeshData::AssignSubObj(tag.pParent->stSimMesh,this);
			break;
		case enTagType_SimMesh_AssignSubObjVtx:
			CSimMeshData::AssignSubObjVtx(tag.pParent->stSimMesh,this);
			break;
		case enTagType_SimMesh_AssignSimVtx:
			CSimMeshData::AssignSimVtx(tag.pParent->stSimMesh,this);
			break;
		case enTagType_SimMesh_RCN:
			CSimMeshData::GetRecalcNormalData(tag.pParent->stSimMesh,this);
			break;
		case enTagType_SimMesh_Skin:
			CSimMeshData::GetSkinData(tag.pParent->stSimMesh,this);
			break;
		case enTagType_SimMesh_SimLinkSrc:
			CSimMeshData::LinkSourceMesh(tag.pParent->stSimMesh,this);
			break;
		case enTagType_SimMesh_Pattern:
			CSimMeshData::GetSimMeshPattern(tag.pParent->stSimMesh,this);
			break;
		case enTagType_SimMesh_Stacks:
			CSimMeshData::GetSimMeshStacks(tag.pParent->stSimMesh,this);
			break;
		case enTagType_SimMesh_SkinCalc:
			CSimMeshData::GetSkinCalc(tag.pParent->stSimMesh,this);
			break;
		case enTagType_SimMesh_SkinPaste:
			CSimMeshData::GetSkinPaste(tag.pParent->stSimMesh,this);
			break;
		case enTagType_SimMesh_OldVtxSave:
			CSimMeshData::SaveOldVtxs(tag.pParent->stSimMesh,this);
			break;
		case enTagType_SimMesh_Force:
			CSimMeshData::GetForce(tag.pParent->stSimMesh,this);
			break;
		case enTagType_SimMesh_CtStretchLink:
			CSimMeshData::GetConstraintStretchLink(tag.pParent->stSimMesh,this);
			break;
		case enTagType_SimMesh_CtStdLink:
			CSimMeshData::GetConstraintStandardLink(tag.pParent->stSimMesh,this);
			break;
		case enTagType_SimMesh_CtBendLink:
			CSimMeshData::GetConstraintBendLink(tag.pParent->stSimMesh,this);
			break;
		case enTagType_SimMesh_BendingStiffness:
			CSimMeshData::GetBendStiffness(tag.pParent->stSimMesh,this);
			break;
		case enTagType_SimMesh_ColVtx:
			CSimMeshData::GetCollisionVerts(tag.pParent->stSimMesh,this);
			break;
		case enTagType_SimMesh_CtFixation:
			CSimMeshData::GetConstraintFixation(tag.pParent->stSimMesh,this);
			break;
		case enTagType_SimLine:
			CSimMeshData::GetSimLines(tag.pParent->stSimMesh,this);
			break;
		default:
			printf("No Suitable Operator Found for TagType: %d\n", tag.eType);
			break;

	}
}