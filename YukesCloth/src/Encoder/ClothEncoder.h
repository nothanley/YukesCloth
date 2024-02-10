#include <fstream>
#pragma once

class StTag;
class CSimObj;
class TagBuffer;

class CClothEncoder
{

public:
	CClothEncoder(std::ofstream* outStream, CSimObj* pSimObj);

private:
	void WriteTagHead(TagBuffer* pNode);
	void WriteTree(TagBuffer* pRootNode);
	void EncodeAllTags(TagBuffer* pRootNode);
	void EncodeTag(TagBuffer* tag);

private:
	void InitTagBuffers(TagBuffer* pTagBf);
	void EncodeRootTag(TagBuffer* pTag);
	void EncodeSimMesh(TagBuffer* pTag);
	void EncodeSimVerts(TagBuffer* pTag);
	void EncodeSubObj(TagBuffer* pTag);
	void EncodeSubObjVerts(TagBuffer* pTag);
	void EncodeRecalcNormals(TagBuffer* pTag);
	void EncodeRCNData(TagBuffer* pTag);

private:
	uint32_t GetTagTotalSize(TagBuffer* pTag);

private:
	CSimObj* m_pSimObj = nullptr;
	std::ofstream* m_pDataStream = nullptr;
	uintptr_t m_iStreamPos = std::ios::beg;
};
