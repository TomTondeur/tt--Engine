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

#include "Nxf.h"
#include "NxSimpleTypes.h"
#include "NxStream.h"

class UserStream : public NxStream
{	
public:
	UserStream(const std::string& filename, bool load);
	virtual	~UserStream();

	virtual	NxU8	readByte()							 const;
	virtual	NxU16	readWord()							 const;
	virtual	NxU32	readDword()							 const;
	virtual	float	readFloat()							 const;
	virtual	double	readDouble()						 const;
	virtual	void	readBuffer(void* buffer, NxU32 size) const;

	virtual	NxStream& storeByte(NxU8 b);
	virtual	NxStream& storeWord(NxU16 w);
	virtual	NxStream& storeDword(NxU32 d);
	virtual	NxStream& storeFloat(NxReal f);
	virtual	NxStream& storeDouble(NxF64 f);
	virtual	NxStream& storeBuffer(const void* buffer, NxU32 size);

	std::fstream fileStream;
};

class MemoryWriteBuffer : public NxStream
{
public:
	MemoryWriteBuffer();
	virtual	~MemoryWriteBuffer();
	void clear();

	virtual NxU8	readByte() const;
	virtual NxU16	readWord() const;
	virtual NxU32	readDword() const;
	virtual float	readFloat() const;
	virtual double	readDouble() const;
	virtual void	readBuffer(void* buffer, NxU32 size) const;

	virtual	NxStream& storeByte(NxU8 b);
	virtual	NxStream& storeWord(NxU16 w);
	virtual	NxStream& storeDword(NxU32 d);
	virtual	NxStream& storeFloat(NxReal f);
	virtual	NxStream& storeDouble(NxF64 f);
	virtual	NxStream& storeBuffer(const void* buffer, NxU32 size);

	NxU32 currentSize;
	NxU32 maxSize;
	NxU8* data;
};

class MemoryReadBuffer : public NxStream
{
public:
	MemoryReadBuffer(const NxU8* data);
	virtual	~MemoryReadBuffer();

	virtual	NxU8	readByte()							 const;
	virtual	NxU16	readWord()							 const;
	virtual	NxU32	readDword()							 const;
	virtual	float	readFloat()							 const;
	virtual	double	readDouble()						 const;
	virtual	void	readBuffer(void* buffer, NxU32 size) const;

	virtual	NxStream& storeByte(NxU8 b);
	virtual	NxStream& storeWord(NxU16 w);
	virtual	NxStream& storeDword(NxU32 d);
	virtual	NxStream& storeFloat(NxReal f);
	virtual	NxStream& storeDouble(NxF64 f);
	virtual	NxStream& storeBuffer(const void* buffer, NxU32 size);

	mutable	const NxU8*	buffer;
};
