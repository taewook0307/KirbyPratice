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

	float Left() 
	{ 
		return Pos.X - Scale.hX();
	}
	float Right() 
	{
		return Pos.X + Scale.hX();
	}
	float Top() 
	{ 
		return Pos.Y - Scale.hY();
	}
	float Bot() 
	{ 
		return Pos.Y + Scale.hY();
	}

	int iLeft()
	{
		return static_cast<int>(Left());
	}
	int iRight()
	{
		return static_cast<int>(Right());
	}
	int iTop()
	{
		return static_cast<int>(Top());
	}
	int iBot()
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
	static bool (*CollisionFunction[static_cast<int>(CollisionType::Max)][static_cast<int>(CollisionType::Max)])(GameEngineCollision* _Left, GameEngineCollision* _Right);
	
	friend CollisionInitClass;
	friend GameEngineActor;
	friend GameEngineLevel;


public:
	static bool PointToPoint(GameEngineCollision* _Left, GameEngineCollision* _Right);
	static bool PointToRect(GameEngineCollision* _Left, GameEngineCollision* _Right);
	static bool PointToCirCle(GameEngineCollision* _Left, GameEngineCollision* _Right);
	static bool RectToPoint(GameEngineCollision* _Left, GameEngineCollision* _Right);
	static bool RectToRect(GameEngineCollision* _Left, GameEngineCollision* _Right);
	static bool RectToCirCle(GameEngineCollision* _Left, GameEngineCollision* _Right);
	static bool CirCleToPoint(GameEngineCollision* _Left, GameEngineCollision* _Right);
	static bool CirCleToRect(GameEngineCollision* _Left, GameEngineCollision* _Right);
	static bool CirCleToCirCle(GameEngineCollision* _Left, GameEngineCollision* _Right);

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

protected:

private:
	CollisionType ColType = CollisionType::Rect;

	float4 CollisionPos;
	float4 CollisionScale;

	void DebugRender();
};

