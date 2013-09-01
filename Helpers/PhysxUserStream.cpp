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

#include "stdafx.h"
#include <stdio.h>
#include "NxPhysics.h"
#include "PhysxUserStream.h"

UserStream::UserStream(const std::string& filename, bool load)
{
	fileStream.open(filename, std::ios::binary | (load ? std::ios::in : std::ios::out) );
}

UserStream::~UserStream()
{
	if(fileStream)
		fileStream.close();
}

// Loading API
NxU8 UserStream::readByte() const
{
	ASSERT(!fileStream.eof(),_T("Error reading PhysX stream: reached EOF"));

	NxU8 oByte;
	const_cast<UserStream*>(this)->fileStream.read(reinterpret_cast<char*>(&oByte), sizeof(NxU8));
	return oByte;
}

NxU16 UserStream::readWord() const
{
	ASSERT(!fileStream.eof(),_T("Error reading PhysX stream: reached EOF"));

	NxU16 oWord;
	const_cast<UserStream*>(this)->fileStream.read(reinterpret_cast<char*>(&oWord), sizeof(NxU16));
	return oWord;
}

NxU32 UserStream::readDword() const
{
	ASSERT(!fileStream.eof(),_T("Error reading PhysX stream: reached EOF"));

	NxU32 oDword;
	const_cast<UserStream*>(this)->fileStream.read(reinterpret_cast<char*>(&oDword), sizeof(NxU32));
	return oDword;
}

float UserStream::readFloat() const
{
	ASSERT(!fileStream.eof(),_T("Error reading PhysX stream: reached EOF"));

	NxReal oFloat;
	const_cast<UserStream*>(this)->fileStream.read(reinterpret_cast<char*>(&oFloat), sizeof(NxReal));
	return oFloat;
}

double UserStream::readDouble() const
{
	ASSERT(!fileStream.eof(),_T("Error reading PhysX stream: reached EOF"));

	NxF64 oDouble;
	const_cast<UserStream*>(this)->fileStream.read(reinterpret_cast<char*>(&oDouble), sizeof(NxF64));
	return oDouble;
}

void UserStream::readBuffer(void* buffer, NxU32 size) const
{
	ASSERT(!fileStream.eof(),_T("Error reading PhysX stream: reached EOF"));

	const_cast<UserStream*>(this)->fileStream.read(static_cast<char*>(buffer), size);
}

// Saving API
NxStream& UserStream::storeByte(NxU8 b)
{
	fileStream.write(reinterpret_cast<char*>(&b), sizeof(NxU8));
	return *this;
}

NxStream& UserStream::storeWord(NxU16 w)
{
	fileStream.write(reinterpret_cast<char*>(&w), sizeof(NxU16));
	return *this;
}

NxStream& UserStream::storeDword(NxU32 d)
{
	fileStream.write(reinterpret_cast<char*>(&d), sizeof(NxU32));
	return *this;
}

NxStream& UserStream::storeFloat(NxReal f)
{
	fileStream.write(reinterpret_cast<char*>(&f), sizeof(NxReal));
	return *this;
}

NxStream& UserStream::storeDouble(NxF64 f)
{
	fileStream.write(reinterpret_cast<char*>(&f), sizeof(NxF64));
	return *this;
}

NxStream& UserStream::storeBuffer(const void* buffer, NxU32 size)
{
	fileStream.write(static_cast<const char*>(buffer), size);
	return *this;
}


MemoryWriteBuffer::MemoryWriteBuffer() : currentSize(0), maxSize(0), data(NULL)
{}

MemoryWriteBuffer::~MemoryWriteBuffer()
{
	NX_DELETE_ARRAY(data);
}

void MemoryWriteBuffer::clear()
{
	currentSize = 0;
}

NxStream& MemoryWriteBuffer::storeByte(NxU8 b)
{
	storeBuffer(&b, sizeof(NxU8));
	return *this;
}

NxStream& MemoryWriteBuffer::storeWord(NxU16 w)
{
	storeBuffer(&w, sizeof(NxU16));
	return *this;
}

NxStream& MemoryWriteBuffer::storeDword(NxU32 d)
{
	storeBuffer(&d, sizeof(NxU32));
	return *this;
}

NxStream& MemoryWriteBuffer::storeFloat(NxReal f)
{
	storeBuffer(&f, sizeof(NxReal));
	return *this;
}

NxStream& MemoryWriteBuffer::storeDouble(NxF64 f)
{
	storeBuffer(&f, sizeof(NxF64));
	return *this;
}

NxStream& MemoryWriteBuffer::storeBuffer(const void* buffer, NxU32 size)
{
	NxU32 expectedSize = currentSize + size;
	if(expectedSize > maxSize){
		maxSize = expectedSize + 4096;

		NxU8* newData = new NxU8[maxSize];
		NX_ASSERT(newData!=NULL);

		if(data){
			memcpy(newData, data, currentSize);
			delete[] data;
		}
		data = newData;
	}
	memcpy(data+currentSize, buffer, size);
	currentSize += size;
	return *this;
}


MemoryReadBuffer::MemoryReadBuffer(const NxU8* data) : buffer(data)
{}

MemoryReadBuffer::~MemoryReadBuffer() // We don't own the data => no delete
{}

NxU8 MemoryReadBuffer::readByte() const
{
	NxU8 b;
	memcpy(&b, buffer, sizeof(NxU8));
	buffer += sizeof(NxU8);
	return b;
}

NxU16 MemoryReadBuffer::readWord() const
{
	NxU16 w;
	memcpy(&w, buffer, sizeof(NxU16));
	buffer += sizeof(NxU16);
	return w;
}

NxU32 MemoryReadBuffer::readDword() const
{
	NxU32 d;
	memcpy(&d, buffer, sizeof(NxU32));
	buffer += sizeof(NxU32);
	return d;
}

float MemoryReadBuffer::readFloat() const
{
	float f;
	memcpy(&f, buffer, sizeof(float));
	buffer += sizeof(float);
	return f;
}

double MemoryReadBuffer::readDouble() const
{
	double f;
	memcpy(&f, buffer, sizeof(double));
	buffer += sizeof(double);
	return f;
}

void MemoryReadBuffer::readBuffer(void* dest, NxU32 size) const
{
	memcpy(dest, buffer, size);
	buffer += size;
}


// 'DISABLED' METHODS

NxU8	MemoryWriteBuffer::readByte()							const { NX_ASSERT(0); return 0;		}
NxU16	MemoryWriteBuffer::readWord()							const { NX_ASSERT(0); return 0;		}
NxU32	MemoryWriteBuffer::readDword()							const { NX_ASSERT(0); return 0;		}
float	MemoryWriteBuffer::readFloat()							const { NX_ASSERT(0); return 0.0f;	}
double	MemoryWriteBuffer::readDouble()							const { NX_ASSERT(0); return 0.0;	}
void	MemoryWriteBuffer::readBuffer(void* buffer, NxU32 size)	const { NX_ASSERT(0); 				}

NxStream& MemoryReadBuffer::storeByte(NxU8 b)							{ NX_ASSERT(0);	return *this; }
NxStream& MemoryReadBuffer::storeWord(NxU16 w)							{ NX_ASSERT(0);	return *this; }
NxStream& MemoryReadBuffer::storeDword(NxU32 d)							{ NX_ASSERT(0);	return *this; }
NxStream& MemoryReadBuffer::storeFloat(NxReal f)						{ NX_ASSERT(0);	return *this; }
NxStream& MemoryReadBuffer::storeDouble(NxF64 f)						{ NX_ASSERT(0);	return *this; }
NxStream& MemoryReadBuffer::storeBuffer(const void* buffer, NxU32 size)	{ NX_ASSERT(0);	return *this; }
