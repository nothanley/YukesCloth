#pragma once
#include <fstream>

class CSimObj
{

	enum {
		enTagType_SimMesh = 0x5,
		enTagType_SimMesh_AssignSubObj = 0x6,
		enTagType_SimMesh_AssignSubObjVtx = 0x6,
		enTagType_SimMesh_AssignSimVtx = 0x6,
		enTagType_SimMesh_RCN = 0x6,
		enTagType_SimMesh_Skin = 0x6,
		enTagType_SimMesh_SimLinkSrc = 0x6,
		enTagType_SimMesh_Pattern = 0x6,
		enTagType_SimMesh_Stacks = 0x6
		/* ... */
		/* todo: complete enums */
	};

public:

	CSimObj(std::ifstream* fs);
	void Create();

private:

	void GetAllTags();
	void InitTag(uint32_t eTyoe);
	void GetSimMesh();

	std::ifstream* m_pDataStream;
	uintptr_t m_iStreamPos = std::ios::beg;
};

