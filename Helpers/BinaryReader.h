#pragma once

#include "stdafx.h"

class BinaryReader final{
public:
	BinaryReader(const std::tstring& filename);
	~BinaryReader(void);

	template<typename T> T Read(void)
	{
		T retVal;
		fileStream.read(reinterpret_cast<char*>(&retVal),sizeof(T));
		return retVal;
	}
	
	template<typename T> void ReadArray(T* outArray, size_t size)
	{
		fileStream.read(reinterpret_cast<char*>(outArray),sizeof(T) * size);
	}

private:
	std::ifstream fileStream;

	BinaryReader(void);
	BinaryReader& operator=(const BinaryReader& src);
	BinaryReader(const BinaryReader& src);
};
