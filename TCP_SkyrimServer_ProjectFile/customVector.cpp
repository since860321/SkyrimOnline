//for linux
#include "customVector.h"
#include <math.h>

vector::vector()
	: x(0), y(0), z(0)
{
}

vector::vector(int _x, int _y, int _z)
	: x((float)_x), y((float)_y), z((float)_z)
{
}

vector vector::operator-(vector& rhw)
{
	vector  ret(0,0,0);

	ret.x = x - rhw.x;
	ret.y = y - rhw.y;
	ret.z = z - rhw.z;

	return ret; 
}

vector vector::operator*(float _f)
{
	vector  ret(0,0,0);

	ret.x = x * _f;
	ret.y = y * _f;
	ret.z = z * _f;

	return ret; 
}

vector vector::operator+=(vector& rhw)
{
	//자기 자신 입력에 대한 검사 필요..

	x = x + rhw.x;
	y = y + rhw.y;
	z = z + rhw.z;

	return *this; 
}

vector vector::operator-=(vector& rhw)
{
	//자기 자신 입력에 대한 검사 필요..

	x = x - rhw.x;
	y = y - rhw.y;
	z = z - rhw.z;

	return *this; 
}

void vector::normalize(vector *pOut, const vector *pV)
{
	if (pV == nullptr)
		return ;

	int sqrtSum = sqrt(pV->x) + sqrt(pV->y) + sqrt(pV->z);
	pOut->x = (pV->x)/sqrtSum;
	pOut->y = (pV->y)/sqrtSum;
	pOut->z = (pV->z)/sqrtSum;
}

