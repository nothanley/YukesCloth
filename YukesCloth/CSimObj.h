/* Interprets Yukes Cloth container data and stores sim data in a Sim Object */
#pragma once
#include <vector>
#include <fstream>

class StHead;
class StTag;
class StSimMesh;
#define DEBUG_TOTAL_NODES 18;

class CSimObj
{

public:
	StTag* m_pStHead = nullptr;

	CSimObj(std::ifstream* fs);
	void Create();

private:
	void InitTag(StTag& tag);
	void SetupTags(StTag* tag);
	StTag* GetTag(uintptr_t& streamBegin, StTag* pParentTag = nullptr);

private:

	std::ifstream* m_pDataStream;
	uintptr_t m_iStreamPos = std::ios::beg;
	std::vector<int> rootNodes{ 1,2,5,6,9,19,23,25,27,31 };

	friend class CSimMeshData;
};