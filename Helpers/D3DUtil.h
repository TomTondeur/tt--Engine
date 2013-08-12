//    __  __      ______            _          
//   / /_/ /__ _ / ____/___  ____ _(_)___  ___ 
//  / __/ __(_|_) __/ / __ \/ __ `/ / __ \/ _ \
// / /_/ /__ _ / /___/ / / / /_/ / / / / /  __/
// \__/\__(_|_)_____/_/ /_/\__, /_/_/ /_/\___/ 
//                        /____/               
//
// D3DUtil.h : file containing includes and various utilities regarding DirectX
// Copyright © 2013 Tom Tondeur
//

#pragma once
#include "stdafx.h"
// Let VC++ know we are compiling for WinXP and up.  This let's us use
// API functions specific to WinXP (e.g., WM_INPUT API).
#ifndef _WIN32_WINNT
#define _WIN32_WINNT   0x0600 // Vista
#endif

using namespace std;

// Enable extra D3D debugging in debug builds if using the debug DirectX runtime.  
// This makes D3D objects work well in the debugger watch window, but slows down 
// performance slightly.
#if defined(DEBUG) || defined(_DEBUG)
	#ifndef D3D_DEBUG_INFO
	#define D3D_DEBUG_INFO
	#endif
#endif

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <d3d10.h>
#pragma comment(lib, "d3d10.lib")
#include <dxgi.h>
#pragma comment(lib, "dxgi.lib")
#include <dxerr.h>
#pragma comment(lib, "dxerr.lib")

#include <d3dx10.h>
#include <D3DX10Core.h>
#include <D3DX10Math.h>
#pragma comment(lib, "d3dx10.lib")

#include <cassert>

//*****************************************************************************
// Math Constants
//*****************************************************************************
const float INFINITY = FLT_MAX;
const float EPSILON  = 0.001f;
const double ELOG2_INV = 1.4426950408889634073599246810019;

//*****************************************************************************
// Bounding Volumes
//*****************************************************************************
struct AABB 
{
	// Initialize to an infinitely small bounding box.
	AABB()
		: minPt(INFINITY, INFINITY, INFINITY),
		  maxPt(-INFINITY, -INFINITY, -INFINITY){}

    D3DXVECTOR3 center()
	{
		return 0.5f*(minPt+maxPt);
	}
	D3DXVECTOR3 size()
	{
		return D3DXVECTOR3(maxPt.x-minPt.x,maxPt.y-minPt.y,maxPt.z-minPt.z);
	}
	D3DXVECTOR3 minPt;
	D3DXVECTOR3 maxPt;
};

struct BoundingSphere 
{
	BoundingSphere()
		: pos(0.0f, 0.0f, 0.0f), radius(0.0f){}

	D3DXVECTOR3 pos;
	float radius;
};

//*****************************************************************************
//Declare templates for releasing interfaces and deleting objects 
//*****************************************************************************
template<class Interface>
void ReleaseAndNull(Interface& pInterfaceToRelease)
{
    if (pInterfaceToRelease != 0)
    {
        pInterfaceToRelease->Release();
        pInterfaceToRelease = 0;
    }
}
template<class T>
void DeleteAndNull(T& pObjectToDelete)
{
	delete pObjectToDelete;
	pObjectToDelete = nullptr;
}

//*****************************************************************************
// Convenience functions.
//*****************************************************************************

// Converts ARGB 32-bit color format to ABGR 32-bit color format.
D3DX10INLINE UINT ARGB2ABGR(UINT argb)
{
	BYTE A = (argb >> 24) & 0xff;
	BYTE R = (argb >> 16) & 0xff;
	BYTE G = (argb >>  8) & 0xff;
	BYTE B = (argb >>  0) & 0xff;

	return (A << 24) | (B << 16) | (G << 8) | (R << 0);
}

// Returns random float in [0, 1).
D3DX10INLINE float RandF()
{
	return (float)(rand()) / (float)RAND_MAX;
}

// Returns random float in [a, b).
D3DX10INLINE float RandF(float a, float b)
{
	return a + RandF()*(b-a);
}

// Returns random vector on the unit sphere.
D3DX10INLINE D3DXVECTOR3 RandUnitVec3()
{
	D3DXVECTOR3 v(RandF(), RandF(), RandF());
	D3DXVec3Normalize(&v, &v);
	return v;
}
 
template<typename T>
D3DX10INLINE T Min(const T& a, const T& b)
{
	return a < b ? a : b;
}

template<typename T>
D3DX10INLINE T Max(const T& a, const T& b)
{
	return a > b ? a : b;
}
 
template<typename T>
D3DX10INLINE T Lerp(const T& a, const T& b, float t)
{
	return a + (b-a)*t;
}

template<typename T>
D3DX10INLINE T Clamp(const T& x, const T& low, const T& high)
{
	return x < low ? low : (x > high ? high : x); 
}

struct VertexPosTex
{
	VertexPosTex() : Position(D3DXVECTOR3()), TexCoord(D3DXVECTOR2()){}

	VertexPosTex(D3DXVECTOR3 pos, D3DXVECTOR2 texCoord):
		Position(pos), TexCoord(texCoord){}

	D3DXVECTOR3 Position;
	D3DXVECTOR2 TexCoord;
};

//Initializes argument to zero
D3DX10INLINE void ZeroInit(LARGE_INTEGER& integer)
{
	integer.QuadPart = 0;
	integer.LowPart = 0;
	integer.HighPart = 0;
}