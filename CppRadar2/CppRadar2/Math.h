#pragma once
#include <Windows.h>

struct Vector3
{
    float x, y, z;

    void operator *=(float fSome)
    {
        this->x *= fSome;
        this->y *= fSome;
        this->z *= fSome;
    }
};

float _sqrt(float fNumber);

struct Vector2
{
    float x, y;

    void operator =(Vector2 vSome)
    {
        this->x = vSome.x;
        this->y = vSome.y;
    }

    void operator *=(float fSome)
    {
        this->x *= fSome;
        this->y *= fSome;
    }

    Vector2 operator -(Vector2 vSome)
    {
        return { this->x - vSome.x, this->y - vSome.y };
    }

    void ToVector2(Vector3 veSome)
    {
        this->x = veSome.x;
        this->y = veSome.y;
    }

    float Length()
    {
        return _sqrt(this->x * this->x + this->y * this->y);
    }

    void Add(Vector2 vSome)
    {
        this->x += vSome.x;
        this->y += vSome.y;
    }

    void Normalize()
    {
        float fLength = this->Length();
        if (fLength != 0)
        {
            this->x /= fLength;
            this->y /= fLength;
        }
    }
};

Vector2 RotatePoint(Vector2 vPos, Vector2 vCenter, float iAngle);

float _min(float fA, float fB);