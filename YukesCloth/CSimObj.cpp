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
	/* Initialize Struct Header Data */
	m_pStHead = new StHead{ _U32,_U32,_U32,_U32 };

#ifdef DEBUG_CONSOLE
	printf("\n====== StHead {sTagCount:%d, eType:%d, sSize:%d, sTotalSize:%d} ======\n",
			m_pStHead->sTagCount, m_pStHead->eType, m_pStHead->sSize, m_pStHead->sTotalSize);
#endif

	/* Update stream pointer */
	m_iStreamPos += m_pStHead->sSize;
	GetAllTags();
};

void
CSimObj::GetAllTags() {

	std::vector<StTag> yclNodes;
	uint32_t numNodes = DEBUG_TOTAL_NODES; /* Placeholder number for debugging */

	/* Get first tag */
	StTag parent = GetTag();
	yclNodes.push_back(parent);

	/* Iterate and collect all child nodes */
	for (int i = 0; i < numNodes; i++) {
		StTag node = GetTag(&parent);
		yclNodes.push_back(node);
	}

}

StTag
CSimObj::GetTag(StTag* pParentTag) {
	/* Seek to data begin address*/
	m_pDataStream->seekg(m_iStreamPos);

	/* Init Struct Tag data */
	StTag stDataTag{ _U32,_U32,_U32 };
	stDataTag.pParent = pParentTag;

#ifdef DEBUG_CONSOLE
	printf("\n[0x%x]\t- StTag {eType:%d, sSize:%d, sTotalSize:%d}\n",
		m_iStreamPos, stDataTag.eType, stDataTag.sSize, stDataTag.sTotalSize);
#endif

	InitTag(stDataTag);
	return stDataTag;
}


void
CSimObj::InitTag(StTag& tag) {

	switch (tag.eType) {

		case enTagType_SimMesh:
			CSimMeshData::GetSimMesh(tag,this);
			m_iStreamPos += tag.sSize;
			break;
		case enTagType_SimMesh_AssignSubObj:
			CSimMeshData::AssignSubObj(tag.pParent->stSimMesh,this);
			m_iStreamPos += tag.sSize;
			break;
		case enTagType_SimMesh_AssignSubObjVtx:
			CSimMeshData::AssignSubObjVtx(tag.pParent->stSimMesh,this);
			m_iStreamPos += tag.sTotalSize;
			break;
		case enTagType_SimMesh_AssignSimVtx:
			CSimMeshData::AssignSimVtx(tag.pParent->stSimMesh,this);
			m_iStreamPos += tag.sTotalSize;
			break;
		case enTagType_SimMesh_RCN:
			CSimMeshData::GetRecalcNormalData(tag.pParent->stSimMesh,this);
			m_iStreamPos += tag.sTotalSize;
			break;
		case enTagType_SimMesh_Skin:
			CSimMeshData::GetSkinData(tag.pParent->stSimMesh,this);
			m_iStreamPos += tag.sTotalSize;
			break;
		case enTagType_SimMesh_SimLinkSrc:
			CSimMeshData::LinkSourceMesh(tag.pParent->stSimMesh,this);
			m_iStreamPos += tag.sTotalSize;
			break;
		case enTagType_SimMesh_Pattern:
			CSimMeshData::GetSimMeshPattern(tag.pParent->stSimMesh,this);
			m_iStreamPos += tag.sSize;
			break;
		case enTagType_SimMesh_Stacks:
			CSimMeshData::GetSimMeshStacks(tag.pParent->stSimMesh,this);
			m_iStreamPos += tag.sSize;
			break;
		case enTagType_SimMesh_SkinCalc:
			CSimMeshData::GetSkinCalc(tag.pParent->stSimMesh,this);
			m_iStreamPos += tag.sTotalSize;
			break;
		case enTagType_SimMesh_SkinPaste:
			CSimMeshData::GetSkinPaste(tag.pParent->stSimMesh,this);
			m_iStreamPos += tag.sTotalSize;
			break;
		case enTagType_SimMesh_OldVtxSave:
			CSimMeshData::SaveOldVtxs(tag.pParent->stSimMesh,this);
			m_iStreamPos += tag.sTotalSize;
			break;
		case enTagType_SimMesh_Force:
			CSimMeshData::GetForce(tag.pParent->stSimMesh,this);
			m_iStreamPos += tag.sTotalSize;
			break;
		case enTagType_SimMesh_CtStretchLink:
			CSimMeshData::GetConstraintStretchLink(tag.pParent->stSimMesh,this);
			m_iStreamPos += tag.sTotalSize;
			break;
		case enTagType_SimMesh_CtStdLink:
			CSimMeshData::GetConstraintStandardLink(tag.pParent->stSimMesh,this);
			m_iStreamPos += tag.sTotalSize;
			break;
		case enTagType_SimMesh_CtBendLink:
			CSimMeshData::GetConstraintBendLink(tag.pParent->stSimMesh,this);
			m_iStreamPos += tag.sTotalSize;
			break;
		case enTagType_SimMesh_BendingStiffness:
			CSimMeshData::GetBendStiffness(tag.pParent->stSimMesh,this);
			m_iStreamPos += tag.sTotalSize;
			break;
		case enTagType_SimMesh_ColVtx:
			CSimMeshData::GetCollisionVerts(tag.pParent->stSimMesh,this);
			m_iStreamPos += tag.sTotalSize;
			break;
		case enTagType_SimMesh_CtFixation:
			CSimMeshData::GetConstraintFixation(tag.pParent->stSimMesh,this);
			m_iStreamPos += tag.sTotalSize;
			break;
		case enTagType_SimLine:
			CSimMeshData::GetSimLines(tag.pParent->stSimMesh,this);
			m_iStreamPos += tag.sTotalSize;
			break;
		default:
			printf("No Suitable Operator Found.\n");
			break;

	}
}