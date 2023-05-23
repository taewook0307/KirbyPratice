#pragma once
#include "GameEngineObject.h"

// ���� :
class GameEngineActor;
class GameEngineActorSubObject : public GameEngineObject
{
	friend class GameEngineLevel;
	friend class GameEngineActor;

public:
	// constrcuter destructer
	GameEngineActorSubObject();
	~GameEngineActorSubObject();

	// delete Function
	GameEngineActorSubObject(const GameEngineActorSubObject& _Other) = delete;
	GameEngineActorSubObject(GameEngineActorSubObject&& _Other) noexcept = delete;
	GameEngineActorSubObject& operator=(const GameEngineActorSubObject& _Other) = delete;
	GameEngineActorSubObject& operator=(GameEngineActorSubObject&& _Other) noexcept = delete;

	bool IsDeath() override;

	GameEngineActor* GetActor() 
	{
		return Master;
	}

protected:

private:
	GameEngineActor* Master = nullptr;
};

