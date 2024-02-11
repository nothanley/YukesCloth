#include "SimMesh.h"
#include "BinaryIO.h"
#include "SimObj.h"
using namespace BinaryIO;

#define _U32  ReadUInt32(*pSimObj->m_pDataStream)
#define _S32  ReadSInt32(*pSimObj->m_pDataStream)
#define _U16  ReadUShort(*pSimObj->m_pDataStream)
#define _U8  ReadByte(*pSimObj->m_pDataStream)
#define _BOOL ReadBool(*pSimObj->m_pDataStream)
#define _FLOAT ReadFloat(*pSimObj->m_pDataStream)

void
CSimMeshData::GetSimMesh(StTag& parent, const CSimObj* pSimObj) {

	/* Initialize and get Sim Mesh stream */
	StSimMesh* _SimMesh = new StSimMesh{ _U32, _U32, _U32, _U32, (bool)_U32, (bool)_U32 };

#ifdef DEBUG_CONSOLE
	//printf("Pos: %d\n", uint64_t(this->m_pDataStream->tellg()) );
	printf("\n\t\t- StSimMesh {sTags: %d, sName:%d, sSimVtxCount:%d, sIterationCount:%d, bCalcNormal: %s, bDispObject: %s}\n",
		_SimMesh->numTags, _SimMesh->sNameIdx, _SimMesh->sSimVtxCount,
		_SimMesh->sIterationCount, _SimMesh->bCalcNormal ? "true" : "false",
		_SimMesh->bDispObject ? "true" : "false");
#endif

	/* Assign sim mesh pointer */
	parent.pSimMesh = _SimMesh;
}

void
CSimMeshData::AssignSubObj(StSimMesh& sMesh, const CSimObj* pSimObj) {
	uint32_t numTags = _U32;
	sMesh.sMeshIndex = _U32;
	sMesh.sObjIndex = _U32;

#ifdef DEBUG_CONSOLE
	printf("\n\t\t- StSimMesh_AssignSubObj {sTags: %d, sModelName:%d, sObjName:%d }\n",
		numTags, sMesh.sMeshIndex, sMesh.sObjIndex);
#endif
}

void
CSimMeshData::AssignSubObjVtx(StSimMesh& sMesh, const CSimObj* pSimObj) {
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
CSimMeshData::GetRCNData(StSimMesh& sMesh, const CSimObj* pSimObj) {
	uint32_t numNodes = _U32;

	sMesh.rcn.parameters.push_back(_U32);
	sMesh.rcn.parameters.push_back(_U32); /* number of item a */
	sMesh.rcn.parameters.push_back(_U32);
	sMesh.rcn.parameters.push_back(_U32); /* number of item b */
}

void
CSimMeshData::GetRecalcNormals(StSimMesh& sMesh, const CSimObj* pSimObj) {
	uint32_t numNodes = _U32;
	uint32_t numElementsA = _U32;
	uint32_t numElementsB = _U32;
	uint32_t sBufferSizeA = _U32;
	uint32_t sBufferSizeB = _U32;

	/* Gets the total length of each element */
	for (int i = 0; i < numElementsA; i++) {
		uint8_t size = _U8;
		sMesh.rcn.sizesA.push_back(size);
	}

	/* align buffer */
	while (pSimObj->m_pDataStream->tellg() % 4 != 0) {
		ReadByte(*pSimObj->m_pDataStream);
	}

	/* Reads element index values using specified length value */
	for (int i = 0; i < numElementsA; i++) {
		uint8_t totalIndices = sMesh.rcn.sizesA.at(i);
		for (int j = 0; j < totalIndices; j++) {
			sMesh.rcn.values.push_back(_U32);
		}
	}

	/* Gets the total length of each element */
	for (int i = 0; i < numElementsB; i++) {
		uint8_t size = _U8;
		sMesh.rcn.sizesB.push_back(size);
	}

	/* align buffer */
	while (pSimObj->m_pDataStream->tellg() % 4 != 0) {
		ReadByte(*pSimObj->m_pDataStream);
	}

	/* Reads element index values using specified length value */
	for (int i = 0; i < numElementsB; i++) {
		uint8_t totalIndices = sMesh.rcn.sizesB.at(i);
		for (int j = 0; j < totalIndices; j++) {
			sMesh.rcn.elements.push_back(SkinVertex{ _U32 ,_FLOAT });
		}
	}
}



void
CSimMeshData::GetSkinData(StSimMesh& sMesh, const CSimObj* pSimObj) 
{
	/* This buffer is arranged with respect to the simVtx table, 
		correct the output to match the table */
	uint32_t numBones = _U32;
	uint32_t numVerts = _U32;
	uint32_t maxBlendWeights = _U32;
	uint32_t numOther = _U32;
	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x20);

	/* Get NodeSkin matrices */
	for (int i = 0; i < numVerts; i++) {
		Vector4 skinMatrix{ _FLOAT, _FLOAT, _FLOAT, _FLOAT };
		sMesh.skin.matrices.push_back(skinMatrix);
	}

	/* Get NodeSkin weights */
	for (int i = 0; i < numVerts; i++) {
		Vector4 skinInfluence{ _FLOAT, _FLOAT, _FLOAT, _FLOAT };
		sMesh.skin.attributes.push_back(skinInfluence);
	}
	uintptr_t skinBufferAddress = pSimObj->m_pDataStream->tellg();

	/* Get blendweight palette */
	std::vector<SimNode> skinPalette;
	for (int i = 0; i < numBones; i++) {
		int nodeIndex = _U32;
		SimNode node = pSimObj->m_NodeTable.at(nodeIndex);
		skinPalette.push_back(node);
	}

	/* Get blendweights */
	for (int i = 0; i < numVerts; i++) {
		MeshWeight blendWeight;
		for (int i = 0; i < maxBlendWeights; i++) {
			blendWeight.numWeights = _U16;
			blendWeight.bones.push_back( skinPalette.at(_U16).name );
			blendWeight.weights.push_back(_FLOAT);
		}
		sMesh.skin.blendWeights.push_back(blendWeight);
	}
}

void
CSimMeshData::Link_DefineSourceMesh(StSimMesh& sMesh, const CSimObj* pSimObj) {
	uint32_t numTriangles = _U32;

	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x20);

	/* Define source edges */
	for (int i = 0; i < numTriangles; i++) {
		sMesh.sourceEdges.push_back(Triangle{ _U16, _U16, _U32 });
	}

}

void
CSimMeshData::GetSimMeshPattern(StSimMesh& sMesh, const CSimObj* pSimObj) {
    if (!&sMesh || sMesh.bIsSimLine) {
		printf("Could not parse skin pattern - Missing sim mesh destination.\n");
		return;
	}

	uint32_t numNodes = _U32;
	sMesh.bSimPattern = _S32;
}

void
CSimMeshData::GetSimMeshStacks(StSimMesh& sMesh, const CSimObj* pSimObj) {
    uint32_t numProperties = _U32;
}

void
CSimMeshData::GetSkinCalc(StSimMesh& sMesh, const CSimObj* pSimObj) {
	uint32_t numSkinVerts = _U32;
	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x10);
	
	for (int i = 0; i < numSkinVerts; i++)
	{
		uint32_t vertIdx = _U32;
		Vector4 skinMatrix = sMesh.skin.matrices.at(vertIdx);
		sMesh.skinCalc.push_back(skinMatrix);
		/* calls nodeskinmatrix */
	}
}

void
CSimMeshData::GetSkinPaste(StSimMesh& sMesh, const CSimObj* pSimObj) {
	uint32_t numPasteVerts = _U32;
	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x10);

	for (int i = 0; i < numPasteVerts; i++)
	{
		uint32_t skinIdx = _U32;
		sMesh.skinPaste.push_back(skinIdx);
	}
}

void
CSimMeshData::SaveOldVtxs(StSimMesh& sMesh, const CSimObj* pSimObj) {
	uint32_t numVerts = _U32;
	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x10);

	for (int i = 0; i < numVerts; i++)
	{
		sMesh.saveVerts.push_back(_U32);
	}
}

void
CSimMeshData::GetForce(StSimMesh& sMesh, const CSimObj* pSimObj) {
	uint32_t numVerts = _U32;
    sMesh.force.parameters = std::vector<float>{ _FLOAT, _FLOAT, _FLOAT, _FLOAT };
	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x30);

	for (int i = 0; i < numVerts; i++)
	{
		sMesh.force.data.push_back(SkinVertex{_U32,_FLOAT});
	}
}


void
CSimMeshData::GetConstraintStretchLink(StSimMesh& sMesh, const CSimObj* pSimObj) {
	uint32_t numLinks = _U32;
	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x10);
	SimConstraint stretchConstraint{ "Stretch", enTagType_SimMesh_CtStretchLink };

	for (int i = 0; i < numLinks; i++)
	{
		EdgeConstraint edgeLink;
		edgeLink.vertices.x.index = _U32;
		edgeLink.vertices.y.index = _U32;

		edgeLink.vertices.x.weight = _FLOAT;
		edgeLink.vertices.y.weight = _FLOAT;;
		stretchConstraint.data.push_back(edgeLink);
	}

	sMesh.constraints.push_back(stretchConstraint);
}

void
CSimMeshData::GetConstraintStandardLink(StSimMesh& sMesh, const CSimObj* pSimObj) {
	uint32_t numLinks = _U32;
	uint32_t unkVal = _U32;
	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x20);
	SimConstraint constraint{ "Standard", enTagType_SimMesh_CtStdLink };

	for (int i = 0; i < numLinks; i++)
	{
		EdgeConstraint edgeLink;
		edgeLink.vertices.x.index = _U16;
		edgeLink.vertices.y.index = _U16;

		edgeLink.vertices.x.weight = _FLOAT;
		edgeLink.vertices.y.weight = _FLOAT;
		edgeLink.vertices.z.weight = _FLOAT;;
		constraint.data.push_back(edgeLink);
	}

	sMesh.constraints.push_back(constraint);
}

void
CSimMeshData::GetConstraintBendLink(StSimMesh& sMesh, const CSimObj* pSimObj) {
	uint32_t numLinks = _U32;
	uint32_t unkVal = _U32;
	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x20);
	SimConstraint constraint{ "Bend", enTagType_SimMesh_CtBendLink };

	for (int i = 0; i < numLinks; i++)
	{
		EdgeConstraint edgeLink;
		edgeLink.vertices.x.index = _U16;
		edgeLink.vertices.y.index = _U16;

		edgeLink.vertices.x.weight = _FLOAT;
		edgeLink.vertices.y.weight = _FLOAT;;
		edgeLink.vertices.z.weight = _FLOAT;;
		constraint.data.push_back(edgeLink);
	}

	sMesh.constraints.push_back(constraint);
}

void
CSimMeshData::GetBendStiffness(StSimMesh& sMesh, const CSimObj* pSimObj) {
	uint32_t numTris = _U32;
	uint32_t unkVal = _U32;
	float unkFloat = _FLOAT;
	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x20);
	SimConstraint constraint{ "BendStiffness", enTagType_SimMesh_BendingStiffness };

	for (int i = 0; i < numTris; i++){
		FaceConstraint face;

		face.x.index = _U16;
		face.y.index = _U16;
		face.z.index = _U32;

		face.weights.push_back(_FLOAT);
		face.weights.push_back(_FLOAT);
		face.weights.push_back(_FLOAT);
		face.weights.push_back(_FLOAT);

		constraint.faceData.push_back(face);
	}

	sMesh.constraints.push_back(constraint);
}

void
CSimMeshData::GetCollisionVerts(StSimMesh& sMesh, const CSimObj* pSimObj) {
	sMesh.colVtx.unkFlag = _S32;
	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x30);

	sMesh.colVtx.unkVal = _S32;
	sMesh.colVtx.numItems = _S32;
	sMesh.colVtx.numVerts = _S32;
	sMesh.colVtx.unkFlagB = _S32;
	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x60);

	for (int i = 0; i < sMesh.colVtx.numItems; i++)
	{
		Points segment;
		segment.x.index = _U32;
		segment.y.index = _U32;

		segment.x.weight = _FLOAT;
		segment.y.weight = _FLOAT;
		sMesh.colVtx.items.push_back(segment);
	}

	for (int i = 0; i < sMesh.colVtx.numVerts; i++)
	{
		sMesh.colVtx.indices.push_back(_U32);
	}

}

void
CSimMeshData::GetConstraintFixation(StSimMesh& sMesh, const CSimObj* pSimObj) {
	uint32_t numVerts = _U32;
	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x10);
	SimConstraint constraint{ "Fixation", enTagType_SimMesh_CtFixation };

	for (int i = 0; i < numVerts; i++)
	{
		EdgeConstraint edge;
		edge.vertices.x.index = _U32;
		edge.vertices.x.weight = _FLOAT;
		constraint.data.push_back(edge);
	}

	sMesh.constraints.push_back(constraint);
}

void
CSimMeshData::GetSimLine(StTag& pTag, const CSimObj* pSimObj) {
    StSimMesh* pSimLine = new StSimMesh;
    pSimLine->numTags = _U32;
    pSimLine->sNameIdx = _U32;
    pSimLine->sSimVtxCount = _U32;
    pSimLine->sIterationCount = _U32;
    pSimLine->bIsSimLine = true;
    pTag.pSimMesh = pSimLine;

#ifdef DEBUG_CONSOLE
    printf("\n\t\t- StSimLine {sTags: %d, sName:%d, sSimVtxCount:%d, sIterationCount:%d}\n",
           pSimLine->numTags, pSimLine->sNameIdx,
           pSimLine->sSimVtxCount, pSimLine->sIterationCount);
#endif
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


void
CSimMeshData::GetLineDef(StSimMesh& sMesh, CSimObj* pSimObj) {
	uint32_t numLines = _U32;
	sMesh.lines.sSize = numLines;
	sMesh.lines.links.resize(numLines);

	for (int i = 0; i < numLines; i++) {
		uint32_t index = _U32;
		uint32_t nodeBegin  = _U32;
		uint32_t nodeEnd = _U32;
		
		/* Collect all linked nodes and append to line def*/
		NodeLink nodeLink = NodeLink{ nodeBegin, nodeEnd };
		for (int j = nodeBegin; j <= nodeEnd; j++) {
			SimNode& node  = sMesh.nodePalette.at(j);
			nodeLink.link.push_back(node);
		}

		sMesh.lines.links.at(index) = nodeLink;
	}
}

void
CSimMeshData::AssignNode(StSimMesh& sLine, CSimObj* pSimObj)
{
    uint32_t numNodes = _U32;

    for (int i = 0; i < numNodes; i++)
    {
        Vector4 matrixf{_FLOAT,_FLOAT,_FLOAT};
        uint32_t index = _U32;

		SimNode sNode = pSimObj->m_NodeTable.at(index);
		sNode.vecf = matrixf;
		sLine.nodePalette.push_back(sNode);
    }
}

void 
CSimMeshData::GetNodeTable(CSimObj* pSimObj) {
	uint32_t numChildren = _U32;
	uint32_t numNodes = _U32;
	pSimObj->m_NodeTable.resize(numNodes);

	printf("Total Nodes: %d\n", numNodes);
	pSimObj->m_iStreamPos += 0x20;
	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos);

    for (uint32_t i = 0; i < numNodes; i++) {
        SimNode node{ i , GetString(pSimObj) };
		pSimObj->m_NodeTable.at(i) = node;
	}
}




void
CSimMeshData::GetLinkTar(StSimMesh& sLine, CSimObj* pSimObj) 
{
	LinkTarget meshTarget;
	meshTarget.source = ReadString(*pSimObj->m_pDataStream, 0x80);
	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0x8C);

	uint32_t numIdxs = _U32;
	uint32_t paletteSize = _U32;
	meshTarget.totalWeights = _U32;
	pSimObj->m_pDataStream->seekg(pSimObj->m_iStreamPos + 0xA0);

	for (uint32_t i = 0; i < numIdxs; i++) {
		meshTarget.indices.push_back(_U32);
	}

	for (uint32_t i = 0; i < paletteSize; i++) {
		meshTarget.palette.push_back(_U32);
	}

	for (uint32_t i = 0; i < paletteSize; i++) {
		MeshWeight blendWeight;
		for (int i = 0; i < meshTarget.totalWeights; i++) {
			blendWeight.numWeights = _U16;
			blendWeight.indices.push_back(_U16);
			blendWeight.weights.push_back(_FLOAT);
		}

		meshTarget.weights.push_back(blendWeight);
	}

	sLine.target = meshTarget;
}












