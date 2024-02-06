#include <vector>
#include <string>
#pragma once

struct Vector4 {
	float x, y, z, w;
};

struct Triangle {
	uint32_t point0, point1, point2;
};

struct StSimMesh {
	uint32_t numTags;
	uint32_t sName; /* Index of name @ string table */
	uint32_t sSimVtxCount;
	uint32_t sIterationCount;

	bool bCalcNormal;
	bool bDispObject;
	bool bSimPattern;

	std::string modelName;
	std::vector<uint32_t> sObjVerts;
	std::vector<uint32_t> simVerts;
	std::vector<Vector4> skinData;
	std::vector<Triangle> linkFaces;
	std::vector<Vector4> skinCalc;
	std::vector<Vector4> skinPaste;
};

struct StHead {
	uint32_t eType;
	uint32_t sTagCount;
	uint32_t sTotalSize;
	uint32_t sSize;
};

struct StTag {
	uint32_t eType;
	uint32_t sSize;
	uint32_t sTotalSize;
	StSimMesh stSimMesh;
	StTag* pParent = nullptr;
};

struct StSimMesh_AssignSubObj {
	uint32_t numTags;
	uint32_t sModelName;
	uint32_t sObjName;
};


namespace yclutils {

	void debugPrintFloats(std::vector<Vector4>* floatArray);

};

