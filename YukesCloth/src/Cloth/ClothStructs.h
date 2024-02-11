#include <vector>
#include <string>
#pragma once

struct Vector4 {
	float x, y, z, w;
};

struct Triangle {
	uint32_t point0, point1, point2;
};

struct SkinVertex {
	uint32_t index;
	float weight;
};

struct Points {
	SkinVertex x, y, z;
};

struct SimNode {
    uint32_t index;
    std::string name;
    Vector4 vecf;
};

struct NodeLink {
    uint32_t begin_index;
    uint32_t end_index;
    std::vector<SimNode> link;
};

struct LineDef {
	uint32_t sSize;
	uint32_t index;
	std::vector<NodeLink> links;
};

struct EdgeConstraint {
	Points vertices;
};

struct FaceConstraint {
	SkinVertex x, y, z;
	std::vector<float> weights;
};

struct RCNData {
	std::vector<uint32_t> parameters;
	std::vector<uint8_t> sizesA;
	std::vector<uint8_t> sizesB;
	std::vector<uint32_t> values;
	std::vector<SkinVertex> elements;
};

struct SimConstraint {
	std::string name;
	uint32_t type;
	std::vector<EdgeConstraint> data;
	std::vector<FaceConstraint> faceData;
};

struct ForceField {
	std::vector<float> parameters;
	std::vector<SkinVertex> data;
};

struct CollisionVerts {
	int32_t unkFlag;
	uint32_t unkVal;
	uint32_t numItems;
	uint32_t numVerts;
	int32_t unkFlagB;

	std::vector<Points> items;
	std::vector<uint32_t> indices;
};

struct MeshWeight {
	int numWeights;
	std::vector<int> indices;
	std::vector<std::string> bones;
	std::vector<float> weights;
};

struct MeshSkin {
	std::vector<Vector4> matrices;
	std::vector<Vector4> attributes;
	std::vector<MeshWeight> blendWeights;
};

struct LinkTarget {
	std::string source;
	int totalWeights;
	std::vector<uint32_t> indices;
	std::vector<uint32_t> palette;
	std::vector<MeshWeight> weights;
};


struct StSimMesh {
	uint32_t numTags;
	uint32_t sNameIdx;
	uint32_t sSimVtxCount;
	uint32_t sIterationCount;

	bool bCalcNormal;
	bool bDispObject;
	bool bSimPattern;

	bool bIsSimLine;
    int sMeshIndex;
    int sObjIndex;

	std::string sModelName;
	std::string sObjName;

	std::vector<uint32_t> sObjVerts;
	std::vector<uint32_t> simVerts;
	std::vector<uint32_t> saveVerts;
	std::vector<uint32_t> skinPaste;
    std::vector<Vector4> skinCalc;
	std::vector<Triangle> sourceEdges;
    std::vector<SimConstraint> constraints;
	std::vector<SimNode> nodePalette;

	MeshSkin skin;
	CollisionVerts colVtx;
	RCNData rcn;
	ForceField force;
	LineDef lines;
	LinkTarget target;
};

struct StTag {
	uint32_t eType;
	uint32_t sSize;
	uint32_t sTotalSize;
	uint32_t sTagCount;

	StSimMesh* pSimMesh = nullptr;
	StTag* pParent = nullptr;
	std::vector<StTag*> children;
	uintptr_t streamPointer;
    std::string sTagName;

#ifdef DEBUG_DISP_BINARY
    std::vector<uint8_t> data;
#endif
};


namespace yclutils {
	void debugPrintFloats(std::vector<Vector4>* floatArray);
	bool hasIndex(const std::vector<int>& vec, int target);
    std::string GetNodeName(const uint32_t &tagEnum);
};











