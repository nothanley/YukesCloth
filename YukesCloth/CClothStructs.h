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
	SkinVertex x, y;
};

struct NodeLink {
	uint32_t x;
	uint32_t y;
};

struct LineDef {
	uint32_t sSize;
	uint32_t index;
	std::vector<NodeLink> vec;
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

struct StSimMesh {
	uint32_t numTags;
	uint32_t sName;
	uint32_t sSimVtxCount;
	uint32_t sIterationCount;

	bool bCalcNormal;
	bool bDispObject;
	bool bSimPattern;

	std::string modelName;
	std::string sObjName;

	std::vector<uint32_t> sObjVerts;
	std::vector<uint32_t> simVerts;
	std::vector<uint32_t> saveVerts;
	std::vector<Vector4> skinData;
	std::vector<Triangle> sourceEdges;
	std::vector<Vector4> skinCalc;
	std::vector<uint32_t> skinPaste;
    std::vector<SimConstraint> constraints;
	CollisionVerts colVtx;
	RCNData rcn;
	ForceField force;

	int modelNameIndex;
	int subObjNameIndex;
    bool isSimLine;
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











