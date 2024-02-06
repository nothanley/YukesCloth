#include "CClothStructs.h"
#include <string>
#include <fstream>
#include <BinaryIO.h>
#include <iostream>

using namespace BinaryIO;

void
yclutils::debugPrintFloats(std::vector<Vector4>* floatArray) {
	std::string filePath = "C:\\Users\\wauke\\Desktop\\gen_skin_data.bin";
	std::ofstream outputFile(filePath.c_str(), ios_base::binary);

	if (!outputFile.is_open()) {
		std::cerr << "Error: Unable to open the file for writing." << std::endl;
		return;
	}

	for (const auto& vec : *floatArray) {
		WriteFloat(&outputFile, vec.x);
		WriteFloat(&outputFile, vec.y);
		WriteFloat(&outputFile, vec.z);
		WriteFloat(&outputFile, vec.w);
	}

	outputFile.close();
}