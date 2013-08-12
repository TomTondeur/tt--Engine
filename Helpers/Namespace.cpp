//    __  __      ______            _          
//   / /_/ /__ _ / ____/___  ____ _(_)___  ___ 
//  / __/ __(_|_) __/ / __ \/ __ `/ / __ \/ _ \
// / /_/ /__ _ / /___/ / / / /_/ / / / / /  __/
// \__/\__(_|_)_____/_/ /_/\__, /_/_/ /_/\___/ 
//                        /____/               
//
// Namespace.cpp - Copyright © 2013 Tom Tondeur
//

#include "Namespace.h"

using namespace std;
using namespace tt;


//------------------------------------------
//Converting tt datatypes to string(stream)s
//------------------------------------------

tstring std::to_tstring(const Vector2& vec)
{
	return tstring(tstring(_T("[")) + to_tstring(vec.x) + _T(",") + to_tstring(vec.y) + _T("]"));
}

tstring std::to_tstring(const Vector3& vec)
{
	return tstring(tstring(_T("[")) + to_tstring(vec.x) + _T(",") + to_tstring(vec.y) + _T(",") + to_tstring(vec.z) + _T("]"));
}

tstring std::to_tstring(const Vector4& vec)
{
	return tstring(tstring(_T("[")) + to_tstring(vec.x) + _T(",") + to_tstring(vec.y) + _T(",") + to_tstring(vec.z) + _T(",") + to_tstring(vec.w) + _T("]"));
}

tstring std::to_tstring(const Matrix3x3& mat)
{
	return tstring(tstring(_T("[")) + to_tstring(mat._11) + _T(",") + to_tstring(mat._12) + _T(",") + to_tstring(mat._13) + _T("]")
							+ _T("[") + to_tstring(mat._21) + _T(",") + to_tstring(mat._22) + _T(",") + to_tstring(mat._23) + _T("]")
							+ _T("[") + to_tstring(mat._31) + _T(",") + to_tstring(mat._32) + _T(",") + to_tstring(mat._33) + _T("]"));
}

tstring std::to_tstring(const Matrix4x4& mat)
{
	return tstring(tstring(_T("[")) + to_tstring(mat._11) + _T(",") + to_tstring(mat._12) + _T(",") + to_tstring(mat._13) + _T(",") + to_tstring(mat._14) + _T("]\n")
							+ _T("[") + to_tstring(mat._21) + _T(",") + to_tstring(mat._22) + _T(",") + to_tstring(mat._23) + _T(",") + to_tstring(mat._24) + _T("]\n")
							+ _T("[") + to_tstring(mat._31) + _T(",") + to_tstring(mat._32) + _T(",") + to_tstring(mat._33) + _T(",") + to_tstring(mat._34) + _T("]\n")
							+ _T("[") + to_tstring(mat._41) + _T(",") + to_tstring(mat._42) + _T(",") + to_tstring(mat._43) + _T(",") + to_tstring(mat._44) + _T("]\n"));
}

tstringstream& operator<<(std::tstringstream& stream, const Vector2& vec)
{
	stream << to_tstring(vec);
	return stream;
}

tstringstream& operator<<(std::tstringstream& stream, const Vector3& vec)
{
	stream << to_tstring(vec);
	return stream;
}

tstringstream& operator<<(std::tstringstream& stream, const Vector4& vec)
{
	stream << to_tstring(vec);
	return stream;
}

tstringstream& operator<<(std::tstringstream& stream, const Matrix3x3& mat)
{
	stream << to_tstring(mat);
	return stream;
}

tstringstream& operator<<(std::tstringstream& stream, const Matrix4x4& mat)
{
	stream << to_tstring(mat);
	return stream;
}

//--------
// Vector2
//--------
Vector2::Vector2(void)
{
	x = 0;
	y = 0;
}
Vector2::Vector2(float f)
{
	x = f;
	y = f;
}

Vector2::Vector2(float _x, float _y)
{
	x = _x;
	y = _y;
}

Vector2::Vector2(const D3DXVECTOR2& v)
{
	x = v.x;
	y = v.y;
}
		
Vector2 Vector2::operator+(const Vector2& v) const
{
	return Vector2() += v;
}

Vector2& Vector2::operator+=(const Vector2& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

Vector2 Vector2::operator-(const Vector2& v) const
{
	return Vector2(*this) -= v;
}

Vector2& Vector2::operator-=(const Vector2& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

Vector2 Vector2::operator*(float f) const
{
	return Vector2() *= f;
}

Vector2& Vector2::operator*=(float f)
{
	x *= f;
	y *= f;
	return *this;
}

Vector2 Vector2::operator/(float f) const
{
	return Vector2() /= f;
}

Vector2& Vector2::operator/=(float f)
{
	x /= f;
	y /= f;
	return *this;
}

Vector2::operator D3DXVECTOR2(void) const
{
	return D3DXVECTOR2(x, y);
}

Vector2& Vector2::Normalize(void)
{
	float len = Length();
	x /= len;
	y /= len;
	return (*this);
}

Vector2 Vector2::Normalize(const Vector2& vec)
{
	Vector2 ret = vec;
	return ret.Normalize();
}

float Vector2::Length(void) const
{
	return sqrtf(x*x + y*y);
}

float Vector2::LengthSq(void) const
{
	return x*x + y*y;
}

//--------
// Vector3
//--------
Vector3::Vector3(void)
{
	x = 0;
	y = 0;
	z = 0;
}
Vector3::Vector3(float f)
{
	x = f;
	y = f;
	z = f;
}

Vector3::Vector3(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

Vector3::Vector3(const D3DXVECTOR3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}

Vector3::Vector3(const D3DXVECTOR4& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}
		
Vector3 Vector3::operator+(const Vector3& v) const
{
	return Vector3(*this) += v;
}

Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3 Vector3::operator-(const Vector3& v) const
{
	return Vector3(x-v.x,y-v.y,z-v.z);
}

Vector3 Vector3::operator-(void) const
{
	return Vector3(-x,-y,-z);
}

Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3 Vector3::operator*(float f) const
{
	return Vector3(x*f,y*f,z*f);
}

Vector3& Vector3::operator*=(float f)
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

Vector3 Vector3::operator*(const Vector3& v) const
{
	return Vector3(x*v.x, y*v.y, z*v.z);
}

Vector3& Vector3::operator*=(const Vector3& v)
{
	x*=v.x;
	y*=v.y;
	z*=v.z;
	return *this;
}

Vector3 operator*(float f,const Vector3& v)
{
	return v*f;
}

Vector3 Vector3::operator/(float f) const
{
	return Vector3(x/f, y/f, z/f);
}

Vector3& Vector3::operator/=(float f)
{
	x /= f;
	y /= f;
	z /= f;
	return *this;
}

bool Vector3::operator==(const Vector3& v) const
{
	return x==v.x && y==v.y && z==v.z;
}

bool Vector3::operator!=(const Vector3& v) const
{
	return !(*this == v);
}

Vector3::operator D3DXVECTOR3(void) const
{
	return D3DXVECTOR3(x,y,z);
}

Vector3::operator NxVec3(void) const
{
	return NxVec3(x,y,z);
}

Vector3& Vector3::Normalize(void)
{
	float len = Length();

	if(len != 0){
		x /= len;
		y /= len;
		z /= len;
	}

	return (*this);
}

Vector3 Vector3::Normalize(const Vector3& vec)
{
	Vector3 ret = vec;
	return ret.Normalize();
}

float Vector3::Length(void) const
{
	return sqrtf(x*x + y*y + z*z);
}

float Vector3::LengthSq(void) const
{
	return x*x + y*y + z*z;
}

Vector3 Vector3::TransformCoord(Matrix4x4 matTransform) const
{
	Vector3 out;
	float w = x*matTransform._14 + y*matTransform._24 + z*matTransform._34 + matTransform._44;
	out.x = (x*matTransform._11 + y*matTransform._21 + z*matTransform._31 + matTransform._41) / w;
	out.y = (x*matTransform._12 + y*matTransform._22 + z*matTransform._32 + matTransform._42) / w;
	out.z = (x*matTransform._13 + y*matTransform._23 + z*matTransform._33 + matTransform._43) / w;
	return out;
}

Vector3 Vector3::Cross(const Vector3& v) const
{
	//Cross product calculation: http://en.wikipedia.org/wiki/Cross_product#Matrix_notation
	//(v1.y * v2*z - v1.z * v2.y) * i + (v1.x * v2.z + v1.z * v2.x) * j + (v1.x * v2.y + v1.y * v2.x) * k
	Vector3 i(1,0,0);
	Vector3 j(0,1,0);
	Vector3 k(0,0,1);
	i *= y*v.z - z*v.y;
	j *= x*v.z - z*v.x;
	k *= x*v.y - y*v.x;
	return i-j+k;
}

//--------
// Vector4
//--------
Vector4::Vector4(void)
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}
Vector4::Vector4(float f)
{
	x = f;
	y = f;
	z = f;
	w = f;
}

Vector4::Vector4(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

Vector4::Vector4(const D3DXVECTOR4& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}
		
Vector4 Vector4::operator+(const Vector4& v) const
{
	return Vector4() += v;
}

Vector4& Vector4::operator+=(const Vector4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

Vector4 Vector4::operator-(const Vector4& v) const
{
	return Vector4() -= v;
}

Vector4& Vector4::operator-=(const Vector4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

Vector4 Vector4::operator*(float f) const
{
	return Vector4() *= f;
}

Vector4& Vector4::operator*=(float f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

Vector4 Vector4::operator/(float f) const
{
	return Vector4() /= f;
}

Vector4& Vector4::operator/=(float f)
{
	x /= f;
	y /= f;
	z /= f;
	w /= f;
	return *this;
}

Vector4::operator D3DXVECTOR4(void) const
{
	return D3DXVECTOR4(x,y,z,w);
}

Vector4& Vector4::Normalize(void)
{
	float len = Length();
	x /= len;
	y /= len;
	z /= len;
	w /= len;
	return (*this);
}

Vector4 Vector4::Normalize(const Vector4& vec)
{
	Vector4 ret = vec;
	return ret.Normalize();
}

float Vector4::Length(void) const
{
	return sqrtf(x*x + y*y + z*z + w*w);
}

float Vector4::LengthSq(void) const
{
	return x*x + y*y + z*z + w*w;
}

Matrix4x4::Matrix4x4(void)
{
	_11 = 0; _12 = 0; _13 = 0; _14 = 0;
	_21 = 0; _22 = 0; _23 = 0; _24 = 0;
	_31 = 0; _32 = 0; _33 = 0; _34 = 0;
	_41 = 0; _42 = 0; _43 = 0; _44 = 0;
}

Matrix4x4::Matrix4x4(const D3DXMATRIX& mat)
{
	_11 = mat._11; _12 = mat._12; _13 = mat._13; _14 = mat._14;
	_21 = mat._21; _22 = mat._22; _23 = mat._23; _24 = mat._24;
	_31 = mat._31; _32 = mat._32; _33 = mat._33; _34 = mat._34;
	_41 = mat._41; _42 = mat._42; _43 = mat._43; _44 = mat._44;
}

Matrix4x4::Matrix4x4(float __11, float __12, float __13, float __14
					,float __21, float __22, float __23, float __24
					,float __31, float __32, float __33, float __34
					,float __41, float __42, float __43, float __44)
{
	_11 = __11; _12 = __12; _13 = __13; _14 = __14;
	_21 = __21; _22 = __22; _23 = __23; _24 = __24;
	_31 = __31; _32 = __32; _33 = __33; _34 = __34;
	_41 = __41; _42 = __42; _43 = __43; _44 = __44;
}

Matrix4x4 Matrix4x4::Identity(void)
{
	return Matrix4x4(1,0,0,0
					,0,1,0,0
					,0,0,1,0
					,0,0,0,1);
}

Matrix4x4 Matrix4x4::Translation(Vector3 displacement)
{
	auto out = Matrix4x4::Identity();
	out._41 = displacement.x;
	out._42 = displacement.y;
	out._43 = displacement.z;
	return out;
}

Matrix4x4 Matrix4x4::Rotation(Quaternion rot)
{
	//Calculations found at http://renderfeather.googlecode.com/hg-history/034a1900d6e8b6c92440382658d2b01fc732c5de/Doc/optimized%2520Matrix%2520quaternion%2520conversion.pdf
	auto out = Matrix4x4::Identity();
	out._11 = 1 - 2*rot.y*rot.y - 2*rot.z*rot.z;
	out._12 = 2*rot.x*rot.y + 2*rot.w*rot.z;
	out._13 = 2*rot.x*rot.z - 2*rot.w*rot.y;

	out._21 = 2*rot.x*rot.y - 2*rot.w*rot.z;
	out._22 = 1 - 2*rot.x*rot.x - 2*rot.z*rot.z;
	out._23 = 2*rot.y*rot.z + 2*rot.w*rot.x;

	out._31 = 2*rot.x*rot.z + 2*rot.w*rot.y;
	out._32 = 2*rot.y*rot.z - 2*rot.w*rot.x;
	out._33 = 1 - 2*rot.x*rot.w - 2*rot.y*rot.y;
	return out;
}

Matrix4x4 Matrix4x4::Scale(Vector3 scale)
{
	auto out = Matrix4x4::Identity();
	out._11 = scale.x;
	out._22 = scale.y;
	out._33 = scale.z;
	return out;
}

Matrix4x4 Matrix4x4::Scale(float scale)
{
	auto out = Matrix4x4::Identity();
	out._11 = scale;
	out._22 = scale;
	out._33 = scale;
	return out;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& mat) const
{/*
	Matrix4x4 out;
	
	out._11 = _11 * mat._11 + _12 * mat._21 + _13 * mat._31 + _14 * mat._41;
	out._12 = _11 * mat._12 + _12 * mat._22 + _13 * mat._32 + _14 * mat._42;
	out._13 = _11 * mat._13 + _12 * mat._23 + _13 * mat._33 + _14 * mat._43;
	out._14 = _11 * mat._14 + _12 * mat._24 + _13 * mat._34 + _14 * mat._44;
	
	out._21 = _21 * mat._21 + _22 * mat._21 + _23 * mat._31 + _24 * mat._41;
	out._22 = _21 * mat._22 + _22 * mat._22 + _23 * mat._32 + _24 * mat._42;
	out._23 = _21 * mat._23 + _22 * mat._23 + _23 * mat._33 + _24 * mat._43;
	out._24 = _21 * mat._24 + _22 * mat._24 + _23 * mat._34 + _24 * mat._44;
	
	out._31 = _31 * mat._31 + _32 * mat._21 + _33 * mat._31 + _34 * mat._41;
	out._32 = _31 * mat._32 + _32 * mat._22 + _33 * mat._32 + _34 * mat._42;
	out._33 = _31 * mat._33 + _32 * mat._23 + _33 * mat._33 + _34 * mat._43;
	out._34 = _31 * mat._34 + _32 * mat._24 + _33 * mat._34 + _34 * mat._44;
	
	out._41 = _41 * mat._41 + _42 * mat._21 + _43 * mat._31 + _44 * mat._41;
	out._42 = _41 * mat._42 + _42 * mat._22 + _43 * mat._32 + _44 * mat._42;
	out._43 = _41 * mat._43 + _42 * mat._23 + _43 * mat._33 + _44 * mat._43;
	out._44 = _41 * mat._44 + _42 * mat._24 + _43 * mat._34 + _44 * mat._44;
	*/
	
    D3DXMATRIX matT;
    D3DXMatrixMultiply(&matT, &this->To_DxMatrix(), &mat.To_DxMatrix());
    
	return Matrix4x4(matT);
}

Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& mat)
{
	return *this = *this * mat;
}

void Matrix4x4::Decompose(Vector3& pos, Quaternion& rot, Vector3& scale) const
{
	D3DXMATRIX transform = To_DxMatrix();
	D3DXVECTOR3 _pos, _scale;
	D3DXQUATERNION _rot;

	D3DXMatrixDecompose(&_scale,&_rot,&_pos,&transform); //Decompose matrix

	pos = _pos;
	rot = _rot;
	scale = _scale;
}

Matrix4x4 Matrix4x4::Inverse(void) const
{
	D3DXMATRIX temp;
	D3DXMatrixInverse(&temp, nullptr, &To_DxMatrix());
	return static_cast<Matrix4x4>(temp);
}

D3DXMATRIX Matrix4x4::To_DxMatrix(void) const
{
	D3DXMATRIX out;
	out._11 = _11; out._12 = _12; out._13 = _13; out._14 = _14;
	out._21 = _21; out._22 = _22; out._23 = _23; out._24 = _24;
	out._31 = _31; out._32 = _32; out._33 = _33; out._34 = _34;
	out._41 = _41; out._42 = _42; out._43 = _43; out._44 = _44;
	return out;
}

Quaternion::Quaternion(void)
{
	x=0;
	y=0;
	z=0;
	w=0;
}

Quaternion::Quaternion(const Quaternion& quat)
{
	x=quat.x;
	y=quat.y;
	z=quat.z;
	w=quat.w;
}

Quaternion::Quaternion(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

Quaternion::Quaternion(D3DXQUATERNION quat)
{
	x = quat.x;
	y = quat.y;
	z = quat.z;
	w = quat.w;
}

Quaternion Quaternion::operator*(const Quaternion& quat) const
{
	Quaternion out;
	
	//Quaternion multiplication: http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/code/index.htm#mul
	out.x =  x * quat.w + y * quat.z - z * quat.y + w * quat.x;
	out.y = -x * quat.z + y * quat.w + z * quat.x + w * quat.y;
	out.z =  x * quat.y - y * quat.x + z * quat.w + w * quat.z;
	out.w = -x * quat.x - y * quat.y - z * quat.z + w * quat.w;

	return out;
}

Quaternion& Quaternion::operator*=(const Quaternion& quat)
{
	return *this = *this * quat;
}

bool Quaternion::operator==(const Quaternion& quat)
{
	return x==quat.x && y==quat.y && z==quat.z && w==quat.w;
}

bool Quaternion::operator!=(const Quaternion& quat)
{
	return !(*this == quat);
}

Quaternion Quaternion::Identity(void)
{
	return Quaternion(0,0,0,1);
}

Quaternion Quaternion::FromEuler(const Vector3& eulerAngles)
{
	float yawCos = cosf(eulerAngles.x * .5f);
	float pitchCos = cosf(eulerAngles.y * .5f);
	float rollCos = cosf(eulerAngles.z * .5f);

	float yawSin = sinf(eulerAngles.x * .5f);
	float pitchSin = sinf(eulerAngles.y * .5f);
	float rollSin = sinf(eulerAngles.z * .5f);

	return Quaternion(yawSin * pitchCos * rollCos - yawCos * pitchSin * rollSin
					 ,yawCos * pitchSin * rollCos + yawSin * pitchCos * rollSin
					 , yawCos * pitchCos * rollCos + yawSin * pitchSin * rollSin
					 , yawCos * pitchCos * rollSin - yawSin * pitchSin * rollCos);
}

Quaternion Quaternion::FromEuler(float yaw, float pitch, float roll)
{
	return Quaternion::FromEuler(Vector3(yaw,pitch,roll));
}

Quaternion::operator D3DXQUATERNION(void) const
{
	return D3DXQUATERNION(x,y,z,w);
}

//------------
//ViewportInfo
//------------

ViewportInfo::ViewportInfo(void)
{
	width = 0;
	height = 0;
}

ViewportInfo::ViewportInfo(unsigned short _width, unsigned short _height)
{
	width = _width;
	height = _height;
}
