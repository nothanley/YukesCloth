/* Interprets Yukes Cloth container data and stores sim data in a Sim Object */
#pragma once
#include <fstream>

class StHead;
class StTag;
class StSimMesh;
#define DEBUG_TOTAL_NODES 18;

class CSimObj
{

public:

	CSimObj(std::ifstream* fs);
	void Create();

private:
	void InitTag(StTag& tag);
	void GetAllTags();
	StTag GetTag(StTag* pParentTag = nullptr);

private:

	StHead* m_pStHead = nullptr;
	std::ifstream* m_pDataStream;
	uintptr_t m_iStreamPos = std::ios::beg;

	friend class CSimMeshData;
};