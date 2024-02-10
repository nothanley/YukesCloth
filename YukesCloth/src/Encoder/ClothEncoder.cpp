#include <iostream>
#include <fstream>
#include <sstream>
#include "BinaryIO.h"
#include <Encoder/ClothEncoder.h>
#include <Cloth/ClothStructs.h>
#include <Cloth/SimObj.h>
#include <Cloth/SimMesh.h>

using namespace std;
using namespace BinaryIO;

struct TagBuffer {
	StTag* tag;
	uint32_t sBegin = 0x10;
	uint32_t sSize;
	std::vector<TagBuffer*> children;
	bool bUsePreDef = false;

	std::stringstream binary = 
		std::stringstream(
		std::ios::binary |
		std::ios::out);
};

CClothEncoder::CClothEncoder(std::ofstream* outStream, CSimObj* pSimObj) {
	m_pDataStream = outStream;
	m_pSimObj = pSimObj;

	/* Setup header tag buffer */
	TagBuffer* pRootBuffer = new TagBuffer;
	pRootBuffer->tag = pSimObj->m_pStHead;

	/* Recursively create all child tag buffers */
	this->InitTagBuffers(pRootBuffer);

	/* Serialize all datasets into formatted binaries */
	this->EncodeAllTags(pRootBuffer);

	/* Merge datasets and write tree hierarchy to out file */
	WriteTree(pRootBuffer);
}

uint32_t
CClothEncoder::GetTagTotalSize(TagBuffer* pTag) {
	uint32_t sSize = pTag->binary.str().size() + 0xC;

	for (auto child : pTag->children) {
		sSize += GetTagTotalSize(child);
	}

	return sSize;
}

void
CClothEncoder::WriteTagHead(TagBuffer* pTagBuf) {
	pTagBuf->sSize = GetTagTotalSize(pTagBuf);
	printf("Size of Tag: %d", pTagBuf->sSize);

	/* Write stream metadata */
	WriteUInt32(m_pDataStream, pTagBuf->tag->eType);
	WriteUInt32(m_pDataStream, pTagBuf->sBegin);
	WriteUInt32(m_pDataStream, pTagBuf->sSize);

	/* Write defined binary */
	m_pDataStream->write(pTagBuf->binary.str().data(), pTagBuf->binary.str().size());
}

void
CClothEncoder::WriteTree(TagBuffer* pTagBuf) {
	WriteTagHead(pTagBuf);

	for (auto& tag : pTagBuf->children) {
		WriteTree(tag);
	}
}

void
CClothEncoder::InitTagBuffers(TagBuffer* pParentBuf)
{
	for (auto& tag : pParentBuf->tag->children) {
		TagBuffer* pChildBuf = new TagBuffer;
		pChildBuf->tag = tag;

		InitTagBuffers(pChildBuf);
		pParentBuf->children.push_back(pChildBuf);
	}
}

void
CClothEncoder::EncodeAllTags(TagBuffer* pRootNode) {
	EncodeTag(pRootNode);

	for (auto &tag : pRootNode->children) {
		EncodeAllTags(tag);
	}
}

void
CClothEncoder::EncodeTag(TagBuffer* pTagBuf) {

	StTag* pTag = pTagBuf->tag;
	pTag->sTagName = yclutils::GetNodeName(pTag->eType);

	switch (pTag->eType) {
		case enTagType_Root:
			EncodeRootTag(pTagBuf);
			break;
		case enTagType_SimMesh:
			EncodeSimMesh(pTagBuf);
			break;
		case enTagType_SimMesh_AssignSubObj:
			EncodeSubObj(pTagBuf);
			break;
		case enTagType_SimMesh_AssignSubObjVtx:
			EncodeSubObjVerts(pTagBuf);
			break;
		case enTagType_SimMesh_AssignSimVtx:
			EncodeSimVerts(pTagBuf);
			break;
		case enTagType_SimLine_AssignNode:
			break;
		case enTagType_SimMesh_RCN:
			EncodeRecalcNormals(pTagBuf);
			break;
		case enTagType_SimMesh_RCNSubObj:
			EncodeRCNData(pTagBuf);
			break;
		case enTagType_SimMesh_Skin:
			break;
		case enTagType_SimMesh_SimLinkSrc:
			break;
		case enTagType_SimMesh_Pattern:
			break;
		case enTagType_SimMesh_Stacks:
			break;
		case enTagType_SimMesh_SkinCalc:
			break;
		case enTagType_SimMesh_SkinPaste:
			break;
		case enTagType_SimMesh_OldVtxSave:
			break;
		case enTagType_SimMesh_Force:
			break;
		case enTagType_SimMesh_CtStretchLink:
			break;
		case enTagType_SimMesh_CtStdLink:
			break;
		case enTagType_SimMesh_CtBendLink:
			break;
		case enTagType_SimMesh_BendingStiffness:
			break;
		case enTagType_SimMesh_ColVtx:
			break;
		case enTagType_SimMesh_CtFixation:
			break;
		case enTagType_SimLine:
			break;
		case enTagType_SimLine_LineDef:
			break;
		case enTagType_StrTbl:
			break;
		case enTagType_String:
			break;
		case enTagType_NodeTbl:
			break;
		default:
			std::cerr << "Could not resolve encode format for tag type: " << pTag->eType << ". Skipping node..." << endl;
			pTagBuf->bUsePreDef = true;
			break;

	}

}

int
FindStringIndex(const std::string& sTarget, std::vector<std::string> stringTable) {

	for (int i = 0; i < stringTable.size(); i++) {
		if (sTarget == stringTable.at(i)) {
			return i;
		}
	}

	return -1;
}

void
AlignStream16(std::stringstream& stream, const uintptr_t& offset = 0) {
	while ((uint32_t(stream.str().size()) + offset) % 16 != 0) {
		WriteByte(stream, 0x0);
	}
}

void
AlignTagHeader(TagBuffer* pTagBuf) {
	AlignStream16(pTagBuf->binary, 0xC);
	pTagBuf->sBegin = 0xC + pTagBuf->binary.tellp();
}


void 
CClothEncoder::EncodeRootTag(TagBuffer* pTagBuf) {
	uint32_t numChildren = pTagBuf->children.size();

	WriteUInt32(pTagBuf->binary, numChildren);
	WriteUInt32(pTagBuf->binary, 0x10000);
	AlignTagHeader(pTagBuf);
}

void
CClothEncoder::EncodeSimMesh(TagBuffer* pTagBuf) {
	StTag* tag = pTagBuf->tag;
	StSimMesh* sMesh = tag->pSimMesh;
	uint32_t numChildren = pTagBuf->children.size();
	uint32_t nameIdx = FindStringIndex(sMesh->sObjName, m_pSimObj->m_sStringTable);

	WriteUInt32(pTagBuf->binary, numChildren);
	WriteUInt32(pTagBuf->binary, nameIdx);
	WriteUInt32(pTagBuf->binary, sMesh->sSimVtxCount);
	WriteUInt32(pTagBuf->binary, sMesh->sIterationCount);
	WriteUInt32(pTagBuf->binary, sMesh->bCalcNormal);
	WriteUInt32(pTagBuf->binary, sMesh->bDispObject);
	AlignTagHeader(pTagBuf);
}

void
CClothEncoder::EncodeSubObj(TagBuffer* pTagBuf) {
	StTag* tag = pTagBuf->tag;
	uint32_t numChildren = pTagBuf->children.size();
	uint32_t nameIdx = FindStringIndex(tag->pSimMesh->sModelName, m_pSimObj->m_sStringTable);

	WriteUInt32(pTagBuf->binary, numChildren);
	WriteUInt32(pTagBuf->binary, nameIdx);
	AlignTagHeader(pTagBuf);
}

void
CClothEncoder::EncodeSubObjVerts(TagBuffer* pTagBuf) {
	StTag* tag = pTagBuf->tag;
	StSimMesh* sMesh = tag->pSimMesh;

	uint32_t numChildren = pTagBuf->children.size();
	WriteUInt32(pTagBuf->binary, numChildren);
	WriteUInt32(pTagBuf->binary, sMesh->sSimVtxCount);
	AlignTagHeader(pTagBuf);

	for (auto& vertIdx : sMesh->sObjVerts) {
		WriteUInt32(pTagBuf->binary, vertIdx);
	}

	AlignStream16(pTagBuf->binary, 0xC);
}

void
CClothEncoder::EncodeSimVerts(TagBuffer* pTagBuf) {
	StTag* tag = pTagBuf->tag;
	StSimMesh* sMesh = tag->pSimMesh;

	uint32_t unkVal0 = 0x1;
	uint32_t unkVal1 = 0x0;
	uint32_t numSimVerts = tag->pSimMesh->simVerts.size();

	WriteUInt32(pTagBuf->binary, unkVal0);
	WriteUInt32(pTagBuf->binary, unkVal1);
	WriteUInt32(pTagBuf->binary, numSimVerts);
	AlignTagHeader(pTagBuf);

	for (auto& vertIdx : sMesh->simVerts) {
		WriteUInt32(pTagBuf->binary, 0x0);
		WriteUInt32(pTagBuf->binary, vertIdx);
	}
	AlignStream16(pTagBuf->binary, 0xC);
}

void
CClothEncoder::EncodeRecalcNormals(TagBuffer* pTagBuf) {
	StTag* tag = pTagBuf->tag;
	StSimMesh* sMesh = tag->pSimMesh;

	uint32_t numChildren = pTagBuf->children.size();
	WriteUInt32(pTagBuf->binary, numChildren);

	for (auto& param : sMesh->rcn.parameters) {
		WriteUInt32(pTagBuf->binary, param);
	}

	AlignTagHeader(pTagBuf);
}

void
CClothEncoder::EncodeRCNData(TagBuffer* pTagBuf) {
	StTag* tag = pTagBuf->tag;
	StSimMesh* sMesh = tag->pSimMesh;

	uint32_t numChildren = pTagBuf->children.size();
	WriteUInt32(pTagBuf->binary, numChildren);

	for (auto& param : sMesh->rcn.parameters) {
		WriteUInt32(pTagBuf->binary, param);
	}

	AlignTagHeader(pTagBuf);
}


//void
//CSimMeshData::GetRecalcNormals(StSimMesh& sMesh, const CSimObj* pSimObj) {
//	uint32_t numNodes = _U32;
//	uint32_t numElementsA = _U32;
//	uint32_t numElementsB = _U32;
//	uint32_t sBufferSizeA = _U32;
//	uint32_t sBufferSizeB = _U32;
//
//	/* Gets the total length of each element */
//	for (int i = 0; i < numElementsA; i++) {
//		uint8_t size = _U8;
//		sMesh.rcn.sizesA.push_back(size);
//	}
//
//	/* align buffer */
//	while (pSimObj->m_pDataStream->tellg() % 8 != 0) {
//		ReadByte(*pSimObj->m_pDataStream);
//	}
//
//	/* Reads element index values using specified length value */
//	for (int i = 0; i < numElementsA; i++) {
//		uint8_t totalIndices = sMesh.rcn.sizesA.at(i);
//		for (int j = 0; j < totalIndices; j++) {
//			sMesh.rcn.values.push_back(_U32);
//		}
//	}
//
//	/* Gets the total length of each element */
//	for (int i = 0; i < numElementsB; i++) {
//		uint8_t size = _U8;
//		sMesh.rcn.sizesB.push_back(size);
//	}
//
//	/* align buffer */
//	while (pSimObj->m_pDataStream->tellg() % 4 != 0) {
//		ReadByte(*pSimObj->m_pDataStream);
//	}
//
//	/* Reads element index values using specified length value */
//	for (int i = 0; i < numElementsB; i++) {
//		uint8_t totalIndices = sMesh.rcn.sizesB.at(i);
//		for (int j = 0; j < totalIndices; j++) {
//			sMesh.rcn.elements.push_back(SkinVertex{ _U32 ,_FLOAT });
//		}
//	}
//}