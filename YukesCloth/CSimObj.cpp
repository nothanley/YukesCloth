#include <vector>
#include "CSimObj.h"
#include <BinaryIO.h>

using namespace BinaryIO;
#define _U32  ReadUInt32(*m_pDataStream)
#define _U16  ReadUInt16(*m_pDataStream)
#define _BOOL ReadBool(*m_pDataStream)

#ifdef DEBUG_CONSOLE
#include <iostream> 
#endif 

struct StHead {
	uint32_t eType;
	uint32_t sTagCount;
	uint32_t sTotalSize;
	uint32_t sSize;
};

struct StTag {
	uint32_t eType;
	uint32_t sSize;
	uint32_t sTotalSize;
};

struct StSimMesh {
	uint32_t numTags;
	uint32_t sName; /* Index of name @ string table */
	uint32_t sSimVtxCount;
	uint32_t sIterationCount;
	bool bCalcNormal;
	bool bDispObject;
};

struct StSimMesh_AssignSubObj /* rename this?? */ {
	uint32_t numTags;
	uint32_t sModelName;
	uint32_t sObjName;
};

CSimObj::CSimObj(std::ifstream* fs) {
	this->m_pDataStream = fs;
}

void
CSimObj::Create() {
	/* Initialize Struct Header Data */
	StHead stClothHead{ _U32,_U32,_U32,_U32 };

#ifdef DEBUG_CONSOLE
	printf("\n====== StHead {sTagCount:%d, eType:%d, sSize:%d, sTotalSize:%d} ======\n",
			stClothHead.sTagCount, stClothHead.eType, stClothHead.sSize, stClothHead.sTotalSize);
#endif

	/* Update stream pointer */
	m_iStreamPos += stClothHead.sSize;
	GetAllTags();
};

void
CSimObj::AssignSubObjVtx() {
	printf("\n\t\t* SimMesh_AssignSubObjVtx - TODO: do something with this data! \n");
}

void 
CSimObj::AssignSubObj() {
	StSimMesh_AssignSubObj _SubObj{ _U32, _U32, _U32, };

	#ifdef DEBUG_CONSOLE
	printf("\n\t\t- StSimMesh_AssignSubObj {sTags: %d, sModelName:%d, sObjName:%d }\n",
		_SubObj.numTags, _SubObj.sModelName, _SubObj.sObjName);
	#endif
}

void
CSimObj::GetSimMesh() {

	/* Initialize and get Sim Mesh stream */
	StSimMesh _SimMesh{ _U32, _U32, _U32, _U32, _U32, _U32 };

#ifdef DEBUG_CONSOLE
	printf("\n\t\t- StSimMesh {sTags: %d, sName:%d, sSimVtxCount:%d, sIterationCount:%d, bCalcNormal: %s, bDispObject: %s}\n",
		_SimMesh.numTags, _SimMesh.sName, _SimMesh.sSimVtxCount,
		_SimMesh.sIterationCount, _SimMesh.bCalcNormal ? "true" : "false",
		_SimMesh.bDispObject ? "true" : "false");
#endif

}

void 
CSimObj::InitTag( uint32_t eType ) {
	switch (eType) {
		case enTagType_SimMesh:
			GetSimMesh();
			break;
		case enTagType_SimMesh_AssignSubObj:
			AssignSubObj();
			break;
		case enTagType_SimMesh_AssignSubObjVtx:
			AssignSubObjVtx();
			break;
		default:
			break;
	}
}

StTag
CSimObj::GetNextTag() {
	/* Seek to data begin address*/
	m_pDataStream->seekg(m_iStreamPos);

	/* Init Struct Tag data */
	StTag stDataTag{ _U32,_U32,_U32 };

#ifdef DEBUG_CONSOLE
	printf("\n\t- StTag {eType:%d, sSize:%d, sTotalSize:%d}\n",
		stDataTag.eType, stDataTag.sSize, stDataTag.sTotalSize);
#endif

	InitTag(stDataTag.eType);
	m_iStreamPos += stDataTag.sSize;

	return stDataTag;
}

void 
CSimObj::GetAllTags() {
	
	std::vector<StTag> yclNodes;
	uint32_t numNodes = 3; /* Placeholder number for debugging */

	for (int i = 0; i < numNodes; i++) {
		StTag node = GetNextTag();
		yclNodes.push_back(node);
	}

}