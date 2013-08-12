#include "BinaryReader.h"

BinaryReader::BinaryReader(const std::tstring& filename):fileStream(filename){}

BinaryReader::~BinaryReader(void)
{
	fileStream.close();
}

//Methods

