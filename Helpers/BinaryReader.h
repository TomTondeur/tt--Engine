// Copyright © 2013 Tom Tondeur
// 
// This file is part of tt::Engine.
// 
// tt::Engine is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// tt::Engine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with tt::Engine.  If not, see <http://www.gnu.org/licenses/>.

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

	void Advance(size_t nrOfBytesToSkip);
	
	std::tstring ReadString(void);	
	std::tstring ReadNullTerminatedString(void);

private:
	std::ifstream fileStream;

	BinaryReader(void);
	BinaryReader& operator=(const BinaryReader& src);
	BinaryReader(const BinaryReader& src);
};
