#include <Windows.h>

bool DrawLine(HDC hDC, DWORD dwBrushColor, int iSize, int iType, float fLeft, float fTop, float fRigth, float fDown)
{
	bool bRes = false;
	HPEN hNPen = 0;
	HPEN hOPen = 0;

	do
	{
		hNPen = CreatePen(iType, iSize, dwBrushColor);
		if (!hNPen)
			break;

		hOPen = (HPEN)SelectObject(hDC, hNPen);
		if (!hOPen)
			break;

		if (!MoveToEx(hDC, fLeft, fTop, 0))
			break;

		if (!LineTo(hDC, fRigth, fDown))
			break;

		if (!DeleteObject((HGDIOBJ)hOPen))
			break;

		bRes = true;
	} while (false);

	return bRes;
}

bool DrawElipse(HDC hDC, DWORD dwBrushColor, int iSize, float fLeft, float fTop)
{
	return DrawLine(hDC, dwBrushColor, iSize, PS_SOLID, fLeft, fTop, fLeft, fTop);
}