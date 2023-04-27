#pragma once
#include "GameEngineObject.h"

// ���� : ȭ��ȿ� �����ϴ� �÷��̾� ���� �Ѿ� ���� �����Ѵٰ� ġ�� ��ġ�� �ִٸ� �� �༮���� ǥ��
class GameEngineActor : public GameEngineObject
{
public:
	// constrcuter destructer
	GameEngineActor();
	~GameEngineActor();

	// delete Function
	GameEngineActor(const GameEngineActor& _Other) = delete;
	GameEngineActor(GameEngineActor&& _Other) noexcept = delete;
	GameEngineActor& operator=(const GameEngineActor& _Other) = delete;
	GameEngineActor& operator=(GameEngineActor&& _Other) noexcept = delete;

protected:

private:

};

