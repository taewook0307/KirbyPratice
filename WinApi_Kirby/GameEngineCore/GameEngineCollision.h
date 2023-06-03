#pragma once
#include "GameEngineActorSubObject.h"
#include <GameEngineBase/GameEngineMath.h>
#include <string>
#include <map>
#include <vector>

enum class CollisionType
{
	Point, // ��
	Rect, // �簢��
	CirCle, // ��
	Max, // ��
};

class CollisionData
{
public:
	float4 Pos;
	float4 Scale;

	float Left() const
	{
		return Pos.X - Scale.hX();
	}
	float Right() const
	{
		return Pos.X + Scale.hX();
	}
	float Top() const
	{
		return Pos.Y - Scale.hY();
	}
	float Bot() const
	{
		return Pos.Y + Scale.hY();
	}

	int iLeft() const
	{
		return static_cast<int>(Left());
	}
	int iRight() const
	{
		return static_cast<int>(Right());
	}
	int iTop() const
	{
		return static_cast<int>(Top());
	}
	int iBot() const
	{
		return static_cast<int>(Bot());
	}

};

// ���� :
class GameEngineActor;
class CollisionInitClass;
class GameEngineCollision : public GameEngineActorSubObject
{
	// �Լ� ������
	static bool (*CollisionFunction[static_cast<int>(CollisionType::Max)][static_cast<int>(CollisionType::Max)])(const CollisionData& _LeftData, const CollisionData& _RightData);

	friend CollisionInitClass;
	friend GameEngineActor;
	friend GameEngineLevel;

public:
	static bool PointToPoint(const CollisionData& _LeftData, const CollisionData& _RightData);
	static bool PointToRect(const CollisionData& _LeftData, const CollisionData& _RightData);
	static bool PointToCirCle(const CollisionData& _LeftData, const CollisionData& _RightData);
	static bool RectToPoint(const CollisionData& _LeftData, const CollisionData& _RightData);
	static bool RectToRect(const CollisionData& _LeftData, const CollisionData& _RightData);
	static bool RectToCirCle(const CollisionData& _LeftData, const CollisionData& _RightData);
	static bool CirCleToPoint(const CollisionData& _LeftData, const CollisionData& _RightData);
	static bool CirCleToRect(const CollisionData& _LeftData, const CollisionData& _RightData);
	static bool CirCleToCirCle(const CollisionData& _LeftData, const CollisionData& _RightData);

public:
	// constrcuter destructer
	GameEngineCollision();
	~GameEngineCollision();

	// delete Function
	GameEngineCollision(const GameEngineCollision& _Other) = delete;
	GameEngineCollision(GameEngineCollision&& _Other) noexcept = delete;
	GameEngineCollision& operator=(const GameEngineCollision& _Other) = delete;
	GameEngineCollision& operator=(GameEngineCollision&& _Other) noexcept = delete;

	void SetCollisionScale(const float4& _Value)
	{
		CollisionScale = _Value;
	}

	// �÷��̾� ��ġ����.
	void SetCollisionPos(const float4& _Value)
	{
		CollisionPos = _Value;
	}

	// ��� ������ �ִµ�. 
	// 1. ���͵� ���Ͷ�

	// ���� �簢��
	// int _Order => ���� _Order �� �浹�Ұž�
	// CollisionType _ThisType ���� ������ ���� �簢������ ����
	// CollisionType _OtherType ���� ������ ���� �簢������ ����
	// std::vector<GameEngineCollision*>& _Result �浹�� �ֵ� ���⿡ �����.

	template<typename EnumType>
	bool Collision(EnumType _Order, std::vector<GameEngineCollision*>& _Result
		, CollisionType _ThisType = CollisionType::CirCle
		, CollisionType _OtherType = CollisionType::CirCle)
	{
		return Collision(static_cast<int>(_Order), _Result, _ThisType, _OtherType);
	}

	bool Collision(int _Order, std::vector<GameEngineCollision*>& _Result
		, CollisionType _ThisType = CollisionType::CirCle
		, CollisionType _OtherType = CollisionType::CirCle);

	void SetOrder(int _Order) override;

	bool CollisonCheck(GameEngineCollision* _Other
		, CollisionType _ThisType
		, CollisionType _OtherType);

	float4 GetActorPivotPos();

	float4 GetActorScale()
	{
		return CollisionScale;
	}

	CollisionData GetCollisionData()
	{
		CollisionData Data;
		Data.Pos = GetActorPivotPos();
		Data.Scale = GetActorScale();
		return Data;
	}

	void SetCollisionType(CollisionType _ColType)
	{
		ColType = _ColType;
	}

	void On() override
	{
		GameEngineActorSubObject::On();
		CollisionRenderValue = true;
	}

	void Off() override
	{
		GameEngineActorSubObject::Off();
		CollisionRenderValue = false;
	}

protected:

private:
	CollisionType ColType = CollisionType::Rect;

	bool CollisionRenderValue = true;

	float4 CollisionPos;
	float4 CollisionScale;

	void DebugRender();
};

