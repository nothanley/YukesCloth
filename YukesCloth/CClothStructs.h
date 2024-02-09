#include <vector>
#include <string>
#pragma once

struct Vector4 {
	float x, y, z, w;
};

struct Triangle {
	uint32_t point0, point1, point2;
};

struct Points {
	uint32_t x, y;
};

struct LineDef {
	uint32_t sSize;
	uint32_t index;
	std::vector<Points> vec;
};

struct SimProperty {
	uint32_t item;
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
	std::vector<Vector4> skinData;
	std::vector<Triangle> linkFaces;
	std::vector<Vector4> skinCalc;
	std::vector<Vector4> skinPaste;
    std::vector<SimProperty> properties;

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











