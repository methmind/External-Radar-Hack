#include "Math.h"

float _min(float fA, float fB)
{
	if (fA > fB)
		return fA;
	else
		return fB;

	return fA;
}

float _abs(float fNumber)
{
	if (fNumber < 0)
		fNumber *= -1;

	return fNumber;
}

float _cos(float fNumber)
{
	double dVal, dTemp;
	unsigned nStep = 1;
	for (dVal = 1.0, dTemp = 1.0; _abs(dTemp) >= 1e-6; ++nStep) {
		dTemp *= -fNumber * fNumber / ((2.0 * nStep) * (2 * nStep - 1));
		dVal += dTemp;
	}
	return dVal;
}

float _sin(float fNumber)
{
	float t = fNumber;
	float sine = t;
	for (int a = 1; a < 5; ++a)
	{
		float mult = -fNumber * fNumber / ((2 * a + 1) * (2 * a));
		t *= mult;
		sine += t;
	}
	return sine;
}

float _sqrt(float fNumber)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = fNumber * 0.5F;
	y = fNumber;
	i = *(long*)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y));
	return 1 / y;
}

Vector2 RotatePoint(Vector2 vPos, Vector2 vCenter, float iAngle)
{
	iAngle *= 3.14 / 180;

	float fCos = _cos(iAngle);
	float fSin = _sin(iAngle);

	Vector2 vRes = { fSin * (vPos.x - vCenter.x) + fCos * (vPos.y - vCenter.y),
		fCos * (vPos.x - vCenter.x) - fSin * (vPos.y - vCenter.y) };

	vRes.Add(vCenter);

	return vRes;
}