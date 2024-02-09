/* Validates YANM file and initializes a stream upon validation success  */
#pragma once
#include <BinaryIO.h>
#include <fstream>

using namespace std;
using namespace BinaryIO;

class CSimObj;
class ClothContainer {
	enum {
		YCLHEAD = 0x0
	};

public:
	ClothContainer(const char* FilePath) {
		this->m_sFilePath = FilePath;
		Load();
		fs->close();
	}

	CSimObj* m_pClothSimObj = nullptr;

private:
	void Load();
	void ReadContents();
	void ValidateContainer();

	std::ifstream* fs = nullptr;
	std::string m_sFilePath;
	uint64_t m_iFileSize;
	float m_fVersion;
	bool isOk = false;

};
