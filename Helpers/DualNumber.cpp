#include "Namespace.h"

using namespace tt;

//Static members
const DualNumber DualNumber::Identity = DualNumber(1,0);

//Ctors

DualNumber::DualNumber(void)
{
	*this = DualNumber::Identity;
}

DualNumber::DualNumber(float _a, float _b) : a(_a), b(_b) { }

DualNumber::DualNumber(const DualNumber& src) : a(src.a), b(src.b) { }

//Operators
DualNumber DualNumber::operator+(const DualNumber& rhs)
{
	return DualNumber(a+rhs.a, b+rhs.b);
}

DualNumber& DualNumber::operator+=(const DualNumber& rhs)
{
	a += rhs.a;
	b += rhs.b;
	return *this;
}

DualNumber DualNumber::operator-(const DualNumber& rhs)
{
	return DualNumber(a-rhs.a, b-rhs.b);
}

DualNumber& DualNumber::operator-=(const DualNumber& rhs)
{
	a -= rhs.a;
	b -= rhs.b;
	return *this;
}
		
DualNumber DualNumber::operator*(const DualNumber& rhs)
{
	return DualNumber(a * rhs.a, a * rhs.b + rhs.a * b);
}

DualNumber& DualNumber::operator*=(const DualNumber& rhs)
{
	a *= rhs.a;
	b = a * rhs.b + rhs.a * b;
	return *this;
}
		
DualNumber DualNumber::operator/(const DualNumber& rhs)
{
	return *this * Inverse(rhs);
}

DualNumber& DualNumber::operator/=(const DualNumber& rhs)
{
	*this *= Inverse(rhs);
	return *this;
}

DualNumber DualNumber::operator*(float f)
{
	return DualNumber(a*f,b*f);
}

DualNumber& DualNumber::operator*=(float f)
{
	a *= f;
	b *= f;
	return *this;
}
		
DualNumber DualNumber::operator/(float f)
{
	return DualNumber(a/f, b/f);
}

DualNumber& DualNumber::operator/=(float f)
{
	a /= f;
	b /= f;
	return *this;
}

//Member functions
DualNumber& DualNumber::Invert(void)
{
	a = 1 / a;
	b *= -a*a;
	return *this;
}

//Static member functions
DualNumber DualNumber::Inverse(const DualNumber& n)
{
	auto _a = 1 / n.a;
	return DualNumber(_a, n.b * _a * _a);
}

DualNumber DualNumber::Sqrt(const DualNumber& n)
{
	auto sqrtA = sqrtf(n.a);
	return DualNumber(sqrtA, 1 / (2*sqrtA));
}