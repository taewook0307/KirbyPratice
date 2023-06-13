#pragma once
#include "GameEngineDebug.h"

// ���� :
class GameEngineMath
{
public:
	static const float PI;
	static const float PI2;

	static const float D2R;
	static const float R2D;

	// 3.14 => 180��
	// float R = 3.14;
	// float Result = R * (180.0f / 3.14);

};

class float4
{
public:
	static const float4 ZERO;
	static const float4 LEFT;
	static const float4 RIGHT;
	static const float4 UP;
	static const float4 DOWN;

	// �Ǽ��� �⺻������ == �� ���� �Ұ����ϴ�. 
	// �ص� ��Ȯ���� �ʴ´�. �Ǽ��� ó���ϴ� ����� ���ʿ� ��Ȯ���� �ʱ� �����̴�.
	// �ε��Ҽ��� ������� �⺻������ ������ ������ �ְ�
	// + - ���� �Ҷ� �����е��� �������� �ٸ� ���� ������ ���ɼ��� ����. 
	float X = 0.0f;
	float Y = 0.0f;
	float Z = 0.0f;
	float W = 1.0f;

	inline int iX() const
	{
		return static_cast<int>(X);
	}

	inline int iY() const
	{
		return static_cast<int>(Y);
	}

	inline float hX() const
	{
		return X * 0.5f;
	}

	inline float hY() const
	{
		return Y * 0.5f;
	}


	inline int ihX() const
	{
		return static_cast<int>(hX());
	}

	inline int ihY() const
	{
		return static_cast<int>(hY());
	}

	inline float4 Half() const
	{
		return { hX(), hY(), Z, W };
	}

	float4 operator-() const
	{
		float4 ReturnValue = *this;

		ReturnValue.X = -ReturnValue.X;
		ReturnValue.Y = -ReturnValue.Y;
		ReturnValue.Z = -ReturnValue.Z;
		return ReturnValue;
	}

	float4 operator-(const float4& _Other) const
	{
		float4 ReturnValue;

		ReturnValue.X = X - _Other.X;
		ReturnValue.Y = Y - _Other.Y;
		ReturnValue.Z = Z - _Other.Z;

		return ReturnValue;
	}



	float4 operator+(const float4& _Other) const
	{
		float4 ReturnValue;

		ReturnValue.X = X + _Other.X;
		ReturnValue.Y = Y + _Other.Y;
		ReturnValue.Z = Z + _Other.Z;

		return ReturnValue;
	}

	float4 operator*(const float4& _Other) const
	{
		float4 ReturnValue;

		ReturnValue.X = X * _Other.X;
		ReturnValue.Y = Y * _Other.Y;
		ReturnValue.Z = Z * _Other.Z;

		return ReturnValue;
	}


	float4 operator*(const float _Value) const
	{
		float4 ReturnValue;

		ReturnValue.X = X * _Value;
		ReturnValue.Y = Y * _Value;
		ReturnValue.Z = Z * _Value;

		return ReturnValue;
	}

	float4& operator+=(const float4& _Other)
	{
		X += _Other.X;
		Y += _Other.Y;
		Z += _Other.Z;

		return *this;
	}


	float4& operator-=(const float4& _Other)
	{
		X -= _Other.X;
		Y -= _Other.Y;
		Z -= _Other.Z;

		return *this;
	}

	float4& operator*=(const float4& _Other)
	{
		X *= _Other.X;
		Y *= _Other.Y;
		Z *= _Other.Z;

		return *this;
	}

	float4& operator*=(const float _Value)
	{
		X *= _Value;
		Y *= _Value;
		Z *= _Value;

		return *this;
	}

	bool operator==(const float4 _Value) const
	{
		return X == _Value.X &&
			Y == _Value.Y &&
			Z == _Value.Z;
	}

	inline void Normalize()
	{
		// ���̸� 1�� ����� �Լ��Դϴ�.
		float Len = Size();

		if (0.0f == Len)
		{
			// MsgBoxAssert("0���� �������� �߽��ϴ�.");
			return;
		}

		X /= Len;
		Y /= Len;
		Z /= Len;
	}

	inline float4 NormalizeReturn()
	{
		float4 Result = *this;
		Result.Normalize();
		return Result;
	}

	inline float Size()
	{
		float Value = X * X + Y * Y; // == ���� * ����

		// �������̴�.
		// ������ Ǯ� �������̶�� �մϴ�.
		Value; // ���� * ���� => ����

		return sqrtf(Value);
	}

	float Max2D() const
	{
		return X > Y ? X : Y;
	}


public:
	inline float GetVectorToDeg(const float4& _Vector)
	{



		return 0.0f;
	}

	inline float GetVectorToRad(const float4& _Vector)
	{
		float4 CalVector = _Vector;
		CalVector.Normalize();
		// acos�Լ��� �̳༮�� 180���� �Ѿ�� �Ǹ� 
		// �Ȱ��� ���� �����ϱ� �����մϴ�.
		float Rad = acosf(CalVector.X);

		//if (_Vector.Y < 0)
		//{

		//}

		// 2         => 1;                 
		// 
		// CalVector.X = > cosf(����);
		// CalVector.Y = > sinf(����);
		return 0.0f;
	}

	// GetUnitVectorFromDeg(45)

	inline float4 GetUnitVectorFromDeg(const float _Degree)
	{
		// 90 => 1.57
		return GetUnitVectorFromRad(_Degree * GameEngineMath::D2R);
	}

	//                                       90.0f
	inline float4 GetUnitVectorFromRad(const float _Rad)
	{
		// cosf(_Rad)�������� ���� 1�϶��� cosf���� �������ϴ�.
		// sinf(_Rad)�������� ���� 1�϶��� sinf���� �������ϴ�.
		// => ������ ���̰� 1�϶��� ����� ���´�.

		// 1.57
		return { cosf(_Rad) , sinf(_Rad) };
	}
};

