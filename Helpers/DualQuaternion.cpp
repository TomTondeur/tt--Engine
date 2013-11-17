#include "Namespace.h"

using namespace tt;

//--------------
//DualQuaternion
//--------------
const DualQuaternion DualQuaternion::Identity = DualQuaternion(Quaternion(0,0,0,1), Vector3(0,0,0));

DualQuaternion::DualQuaternion(void)
{
	*this = DualQuaternion::Identity;
}

// convert unit quaternion and translation to unit dual quaternion
DualQuaternion::DualQuaternion(const Quaternion& rotation, const Vector3& translation) 
{
	Data[0] = rotation;
	//Data[1] = rotation * tt::Quaternion(translation.x, translation.y, translation.z, 0) * .5f; //Optimized version below
	Data[1].x = 0.5f * ( translation.x * rotation.w + translation.y * rotation.z - translation.z * rotation.y ) ; 
	Data[1].y = 0.5f * (-translation.x * rotation.z + translation.y * rotation.w + translation.z * rotation.x ) ; 
	Data[1].z = 0.5f * ( translation.x * rotation.y - translation.y * rotation.x + translation.z * rotation.w ) ; 
	Data[1].w = -.5f * ( translation.x * rotation.x + translation.y * rotation.y + translation.z * rotation.z ) ; 
}

DualQuaternion::DualQuaternion(const DualQuaternion& src)
{
	Data[0].x = src.Data[0].x;
	Data[0].y = src.Data[0].y;
	Data[0].z = src.Data[0].z;
	Data[0].w = src.Data[0].w;
	
	Data[1].x = src.Data[1].x;
	Data[1].y = src.Data[1].y;
	Data[1].z = src.Data[1].z;
	Data[1].w = src.Data[1].w;
}

DualQuaternion& DualQuaternion::operator=(const DualQuaternion& src)
{
	Data[0].x = src.Data[0].x;
	Data[0].y = src.Data[0].y;
	Data[0].z = src.Data[0].z;
	Data[0].w = src.Data[0].w;
	
	Data[1].x = src.Data[1].x;
	Data[1].y = src.Data[1].y;
	Data[1].z = src.Data[1].z;
	Data[1].w = src.Data[1].w;

	return *this;
}

DualQuaternion DualQuaternion::operator+(const DualQuaternion& dualQuat) const
{
	DualQuaternion newDQ;
	newDQ.Data[0] = Data[0] + dualQuat.Data[0];
	newDQ.Data[1] = Data[1] + dualQuat.Data[1];
	return newDQ;
}

DualQuaternion& DualQuaternion::operator+=(const DualQuaternion& dualQuat)
{
	Data[0] += dualQuat.Data[0];
	Data[1] += dualQuat.Data[1];
	return *this;
}

DualQuaternion DualQuaternion::operator*(const DualQuaternion& dualQuat) const
{
	DualQuaternion newDQ;
	newDQ.Data[0] = tt::Quaternion(0,0,0,0)-Data[0] * dualQuat.Data[0];
	newDQ.Data[1] = (Data[0] * dualQuat.Data[1]) + (Data[1] * dualQuat.Data[0]);
	return newDQ;
}

DualQuaternion& DualQuaternion::operator*=(const DualQuaternion& dualQuat)
{
	Data[0] *= dualQuat.Data[0];
	Data[1] = (Data[0] * dualQuat.Data[1]) + (Data[1] * dualQuat.Data[0]);
	return *this;
}

DualQuaternion DualQuaternion::operator*(float f) const
{
	DualQuaternion newDQ;
	newDQ.Data[0] = Data[0] * f;
	newDQ.Data[1] = Data[1] * f;
	return newDQ;
}

DualQuaternion& DualQuaternion::operator*=(float f)
{
	Data[0] *= f;
	Data[1] *= f;
	return *this;
}

DualQuaternion DualQuaternion::operator/(const DualNumber& dualNum) const
{
	if(dualNum.a == 0)
		return *this;

	DualQuaternion newDQ;
	newDQ.Data[0] = Data[0] / dualNum.a;
	newDQ.Data[1] = ((Data[1] * dualNum.a) - (Data[0] * dualNum.b)) / (dualNum.a * dualNum.b);
	return newDQ;
}

DualQuaternion& DualQuaternion::operator/=(const DualNumber& dualNum)
{
	if(dualNum.a == 0)
		return *this;

	Data[0] /= dualNum.a;
	Data[1] = ((Data[1] * dualNum.a) - (Data[0] * dualNum.b)) / (dualNum.a * dualNum.a);
	return *this = *this;
}

DualQuaternion DualQuaternion::Conjugate(const DualQuaternion& dualQuat)
{
	DualQuaternion newDQ;
	newDQ.Data[0] = Quaternion::Conjugate(dualQuat.Data[0]);
	newDQ.Data[1] = Quaternion::Conjugate(dualQuat.Data[1]);
	return newDQ;
}

//http://en.wikipedia.org/wiki/Dual_quaternion#Inverse
DualQuaternion DualQuaternion::Inverse(const DualQuaternion& dualQuat)
{
	// (p + e q)^-1 = p^-1 ( 1 - e(q * p^-1) )
	// => p2 = p^-1
	// => q2 = -q * p^-1)
	auto rotInv = Quaternion::Inverse(dualQuat.Data[0]);
	
	DualQuaternion newDQ;
	newDQ.Data[0] = rotInv;
	newDQ.Data[1] = rotInv * (dualQuat.Data[1] * -1) * rotInv;
	return newDQ;
}

//http://www.xbdev.net/misc_demos/demos/dual_quaternions_beyond/paper.pdf
DualNumber DualQuaternion::Norm(void) const
{
	//Change representation of dual quaternion into 4 dual numbers
	DualNumber a(Data[0].x, Data[1].x);
	DualNumber b(Data[0].y, Data[1].y);
	DualNumber c(Data[0].z, Data[1].z);
	DualNumber d(Data[0].w, Data[1].w);

	return DualNumber::Sqrt(a*a + b*b + c*c + d*d);
}

DualQuaternion& DualQuaternion::Normalize(void)
{
	return *this /= Norm();
}

//normalize((1 - t)p + tq)
DualQuaternion DualQuaternion::DLB(const DualQuaternion& q1, const DualQuaternion& q2, float t)
{
	return (q1*(1-t) + q2*t	).Normalize();
}
