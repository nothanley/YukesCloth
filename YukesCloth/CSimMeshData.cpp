#include "CSimMeshData.h"
#include "BinaryIO.h"
#include "CSimObj.h"
using namespace BinaryIO;

#define _U32  ReadUInt32(*pSimObj->m_pDataStream)
#define _S32  ReadSInt32(*pSimObj->m_pDataStream)
#define _U16  ReadUShort(*pSimObj->m_pDataStream)
#define _BOOL ReadBool(*pSimObj->m_pDataStream)
#define _FLOAT ReadFloat(*pSimObj->m_pDataStream)

void
CSimMeshData::GetSimMesh(StTag& parent, const CSimObj* pSimObj) {

	/* Initialize and get Sim Mesh stream */
	StSimMesh* _SimMesh = new StSimMesh{ _U32, _U32, _U32, _U32, (bool)_U32, (bool)_U32 };

#ifdef DEBUG_CONSOLE
	//printf("Pos: %d\n", uint64_t(this->m_pDataStream->tellg()) );
	printf("\n\t\t- StSimMesh {sTags: %d, sName:%d, sSimVtxCount:%d, sIterationCount:%d, bCalcNormal: %s, bDispObject: %s}\n",
		_SimMesh->numTags, _SimMesh->sName, _SimMesh->sSimVtxCount,
		_SimMesh->sIterationCount, _SimMesh->bCalcNormal ? "true" : "false",
		_SimMesh->bDispObject ? "true" : "false");
#endif

	/* Assign sim mesh pointer */
	parent.pSimMesh = _SimMesh;
}

void
CSimMeshData::AssignSubObj(StSimMesh& sMesh, const CSimObj* pSimObj) {
	uint32_t numTags = _U32;
	sMesh.modelNameIndex = _U32;
	sMesh.subObjNameIndex = _U32;

#ifdef DEBUG_CONSOLE
	printf("\n\t\t- StSimMesh_AssignSubObj {sTags: %d, sModelName:%d, sObjName:%d }\n",
		numTags, sMesh.modelNameIndex, sMesh.subObjNameIndex);
#endif
}

void
CSimMeshData::AssignSubObjVtx(StSimMesh& sMesh, const CSimObj* pSimObj) {
	/* Reads and assigns an index to each vertex in specified subobject */

	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x10);
	uint32_t numVerts = _U32;

	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x20);
	for (uint32_t i = 0; i < numVerts; i++) {
		sMesh.sObjVerts.push_back(_U32);
	}

}

void
CSimMeshData::AssignSimVtx(StSimMesh& sMesh, const CSimObj* pSimObj) {
	uint32_t unkVal0 = _U32;
	uint32_t unkVal1 = _U32;
	uint32_t numSimVerts = _U32;
	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x20);

	for (uint32_t i = 0; i < numSimVerts; i++) {
		uint32_t nullValue = _U32;
		sMesh.simVerts.push_back(_U32);
	}
}

void
CSimMeshData::GetRecalcNormalData(StSimMesh& sMesh, const CSimObj* pSimObj) {
	uint32_t unkVal0 = _U32;
	uint32_t unkVal1 = _U32;
	uint32_t unkVal2 = _U32;
	uint32_t unkVal3 = _U32;
}


void
CSimMeshData::GetSkinData(StSimMesh& sMesh, const CSimObj* pSimObj) {
	if (!&sMesh) {
		printf("Could not parse skin data - Missing sim mesh destination.\n");
		return;
	}

	uintptr_t skinBufferAddress = pSimObj->m_iStreamPos + 0x20;

	for (int i = 0; i < sMesh.sSimVtxCount; i++) {
		/* Calculate vtx index from sim mesh vtx table */
		uint32_t simVtxIndex = sMesh.simVerts.at(i);
		uint32_t sObjVtxIndex = sMesh.sObjVerts.at(simVtxIndex);

		/* Get weight float data using vtx index */
		uintptr_t skinWeightAddress = skinBufferAddress + (sObjVtxIndex * 0x10);
		pSimObj->m_pDataStream->seekg(skinWeightAddress);

		Vector4 skinWeight{ _FLOAT, _FLOAT, _FLOAT, _FLOAT };
		sMesh.skinData.push_back(skinWeight);
	}
}

void
CSimMeshData::LinkSourceMesh(StSimMesh& sMesh, const CSimObj* pSimObj) {
	uint32_t numTriangles = _U32;

	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x20);
	for (int i = 0; i < numTriangles; i++) {
		sMesh.linkFaces.push_back(Triangle{ _U16, _U16, _U32 });
	}

}

void
CSimMeshData::GetSimMeshPattern(StSimMesh& sMesh, const CSimObj* pSimObj) {
	if (!&sMesh) {
		printf("Could not parse skin pattern - Missing sim mesh destination.\n");
		return;
	}

	uint32_t unkVal0 = _U32;
	sMesh.bSimPattern = _S32;
}

void
CSimMeshData::GetSimMeshStacks(StSimMesh& sMesh, const CSimObj* pSimObj) {
	uint32_t unkVal0 = _U32;
}

void
CSimMeshData::GetSkinCalc(StSimMesh& sMesh, const CSimObj* pSimObj) {
	uint32_t numSkinVerts = _U32;
	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x10);

	for (int i = 0; i < numSkinVerts; i++)
	{
		uint32_t patternIdx = _U32;

		/* Get Node Skin Matrix */
		Vector4 skinWeight = sMesh.skinData.at(patternIdx);
		sMesh.skinCalc.push_back(skinWeight);
	}

}


void
CSimMeshData::GetSkinPaste(StSimMesh& sMesh, const CSimObj* pSimObj) {
	uint32_t numPasteVerts = _U32;
	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x10);

	for (int i = 0; i < numPasteVerts; i++)
	{
		uint32_t patternIdx = _U32;
		/* todo: interpret index values */
	}

}

void
CSimMeshData::SaveOldVtxs(StSimMesh& sMesh, const CSimObj* pSimObj) {
	uint32_t numVerts = _U32;
	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x10);

	for (int i = 0; i < numVerts; i++)
	{
		uint32_t vertIdx = _U32;
		/* todo: interpret index values */
	}
}

void
CSimMeshData::GetForce(StSimMesh& sMesh, const CSimObj* pSimObj) {
	uint32_t numVerts = _U32;
	float unkVal0 = _FLOAT;
	float unkVal1 = _FLOAT;

	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x30);
	for (int i = 0; i < numVerts; i++)
	{
		uint32_t vertIdx = _U32;
		float unkFloatVal = _FLOAT;
	}
}


void
CSimMeshData::GetConstraintStretchLink(StSimMesh& sMesh, const CSimObj* pSimObj) {
	uint32_t numLinks = _U32;
	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x10);

	for (int i = 0; i < numLinks; i++)
	{
		uint32_t indexA = _U32;
		uint32_t indexB = _U32;

		float unkFloatValA = _FLOAT;
		float unkFloatValB = _FLOAT;
	}
}

void
CSimMeshData::GetConstraintStandardLink(StSimMesh& sMesh, const CSimObj* pSimObj) {
	uint32_t numLinks = _U32;
	uint32_t unkVal = _U32;
	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x20);

	for (int i = 0; i < numLinks; i++)
	{
		uint16_t indexA = _U16;
		uint16_t indexB = _U16;

		float unkFloatValA = _FLOAT;
		float unkFloatValB = _FLOAT;
	}
}

void
CSimMeshData::GetConstraintBendLink(StSimMesh& sMesh, const CSimObj* pSimObj) {
	uint32_t numLinks = _U32;
	uint32_t unkVal = _U32;
	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x20);

	for (int i = 0; i < numLinks; i++)
	{
		uint16_t indexA = _U16;
		uint16_t indexB = _U16;

		float unkFloatValA = _FLOAT;
		float unkFloatValB = _FLOAT;
	}
}

void
CSimMeshData::GetBendStiffness(StSimMesh& sMesh, const CSimObj* pSimObj) {
	uint32_t numTris = _U32;
	uint32_t unkVal = _U32;
	float unkFloat = _FLOAT;
	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x20);

	for (int i = 0; i < numTris; i++){
		uint16_t indexA = _U16;
		uint16_t indexB = _U16;
		uint16_t indexC = _U32;

		float unkFloatValA = _FLOAT;
		float unkFloatValB = _FLOAT;
		float unkFloatValC = _FLOAT;
		float unkFloatValD = _FLOAT;
	}
}

void
CSimMeshData::GetCollisionVerts(StSimMesh& sMesh, const CSimObj* pSimObj) {
	int32_t unkSValue0 = _S32;
	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x30);

	uint32_t unkVal = _U32;
	uint32_t numUnks = _U32;
	uint32_t numVerts = _U32;
	int32_t unkSValue1 = _S32;
	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x50);

	for (int i = 0; i < numUnks; i++)
	{
		uint32_t indexA = _U32;
		uint32_t indexB = _U32;

		float unkFloatValA = _FLOAT;
		float unkFloatValB = _FLOAT;
	}

	for (int i = 0; i < numVerts; i++)
	{
		uint32_t vertIndex = _U32;
	}

}

void
CSimMeshData::GetConstraintFixation(StSimMesh& sMesh, const CSimObj* pSimObj) {
	uint32_t numVerts = _U32;
	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x10);

	for (int i = 0; i < numVerts; i++)
	{
		uint32_t vertIdx = _U32;
		float unkFloatVal = _FLOAT;
	}
}

void
CSimMeshData::GetSimLines(StSimMesh& sMesh, const CSimObj* pSimObj) {
	uint32_t unkVal0 = _U32;
	uint32_t unkVal1 = _U32;
	uint32_t unkVal2 = _U32;
	uint32_t unkVal3 = _U32;
}

void
CSimMeshData::GetStringTable(CSimObj* pSimObj){
    pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos+0x8);
    uint32_t numNodes = _U32;
    uint32_t numStrings = _U32;

    printf("Total Strings: %d\n", numStrings);
	pSimObj->m_iStreamPos += 0x20;
	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos);

    for (int i = 0; i < numStrings; i++){
		std::string item = GetString(pSimObj);
        pSimObj->m_sStringTable.push_back(item);
    }
}

std::string
CSimMeshData::GetString(CSimObj* pSimObj) {
	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos);
	uint32_t eTagType = _U32;
	uint32_t sSize = _U32;
	uint32_t sTotalSize = _U32;
	uint32_t numChildNodes = _U32;

	int charBufferSize = sTotalSize - 0x10;
	std::string item = ReadString(*pSimObj->m_pDataStream, charBufferSize);

	pSimObj->m_iStreamPos += sTotalSize;
	return item;
}








