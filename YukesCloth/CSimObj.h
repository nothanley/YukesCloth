/* Interprets Yukes Cloth container data and stores sim data in a Sim Object */
#pragma once
#include <fstream>

class StTag;
class CSimObj
{

	enum {
		enTagType_SimMesh = 0x5,
		enTagType_SimMesh_AssignSubObj = 0x6,
		enTagType_SimMesh_AssignSubObjVtx = 0x7,
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

	void InitTag(uint32_t eTyoe);
	void GetAllTags();
	StTag GetNextTag();

private:
	void GetSimMesh();
	void AssignSubObj();
	void AssignSubObjVtx();

	std::ifstream* m_pDataStream;
	uintptr_t m_iStreamPos = std::ios::beg;
};

