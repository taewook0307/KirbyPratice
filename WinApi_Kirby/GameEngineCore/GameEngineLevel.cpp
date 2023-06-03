#include "GameEngineLevel.h"
#include "GameEngineCamera.h"
#include <GameEngineBase/GameEngineDebug.h>
#include "GameEngineCollision.h"

bool GameEngineLevel::IsCollisionDebugRender = false;

GameEngineLevel::GameEngineLevel()
{
	MainCamera = new GameEngineCamera();
	UICamera = new GameEngineCamera();
}

GameEngineLevel::~GameEngineLevel()
{
	if (nullptr != MainCamera)
	{
		delete MainCamera;
		MainCamera = nullptr;
	}

	if (nullptr != UICamera)
	{
		delete UICamera;
		UICamera = nullptr;
	}

	for (const std::pair<int, std::list<GameEngineActor*>>& _Pair : AllActors)
	{
		const std::list<GameEngineActor*>& Group = _Pair.second;

		for (GameEngineActor* _Actor : Group)
		{
			if (nullptr != _Actor)
			{
				delete _Actor;
				_Actor = nullptr;
			}
		}
	}
}


void GameEngineLevel::ActorInit(GameEngineActor* _Actor, int _Order)
{
	_Actor->Level = this;
	_Actor->SetOrder(_Order);
	_Actor->Start();
}

void GameEngineLevel::ActorUpdate(float _Delta)
{
	for (const std::pair<int, std::list<GameEngineActor*>>& _Pair : AllActors)
	{
		const std::list<GameEngineActor*>& Group = _Pair.second;

		for (GameEngineActor* _Actor : Group)
		{
			if (false == _Actor->IsUpdate())
			{
				continue;
			}

			_Actor->AddLiveTime(_Delta);
			_Actor->Update(_Delta);
		}
	}
}
void GameEngineLevel::ActorRender(float _Delta)
{
	MainCamera->Render(_Delta);

	UICamera->Render(_Delta);


	// for���� ������ �ֽ��ϴ�.
	for (const std::pair<int, std::list<GameEngineActor*>>& _Pair : AllActors)
	{
		const std::list<GameEngineActor*>& Group = _Pair.second;

		for (GameEngineActor* _Actor : Group)
		{
			if (false == _Actor->IsUpdate())
			{
				continue;
			}

			_Actor->Render(_Delta);
		}
	}


	if (true == IsCollisionDebugRender)
	{
		for (const std::pair<int, std::list<GameEngineCollision*>>& Pair : AllCollision)
		{
			const std::list < GameEngineCollision*>& Group = Pair.second;

			for (GameEngineCollision* Collision : Group)
			{
				Collision->DebugRender();
			}

		}
	}
}

void GameEngineLevel::CollisionOverRelease()
{
	{
		std::map<int, std::list<GameEngineCollision*>>::iterator GroupStartIter = AllCollision.begin();
		std::map<int, std::list<GameEngineCollision*>>::iterator GroupEndIter = AllCollision.end();

		// ���� ��ŭ�̶� ������ ���̷��� ����.

		for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
		{
			std::list<GameEngineCollision*>& Group = GroupStartIter->second;

			std::list<GameEngineCollision*>::iterator ObjectStartIter = Group.begin();
			std::list<GameEngineCollision*>::iterator ObjectEndIter = Group.end();

			for (; ObjectStartIter != ObjectEndIter; )
			{
				GameEngineCollision* Object = *ObjectStartIter;
				if (false == Object->GetActor()->IsLevelOver())
				{
					++ObjectStartIter;
					continue;
				}

				ObjectStartIter = Group.erase(ObjectStartIter);
			}
		}
	}
}

void GameEngineLevel::CollisionRelease()
{
	{
		std::map<int, std::list<GameEngineCollision*>>::iterator GroupStartIter = AllCollision.begin();
		std::map<int, std::list<GameEngineCollision*>>::iterator GroupEndIter = AllCollision.end();

		// ���� ��ŭ�̶� ������ ���̷��� ����.

		for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
		{
			std::list<GameEngineCollision*>& Group = GroupStartIter->second;

			std::list<GameEngineCollision*>::iterator ObjectStartIter = Group.begin();
			std::list<GameEngineCollision*>::iterator ObjectEndIter = Group.end();

			for (; ObjectStartIter != ObjectEndIter; )
			{
				GameEngineCollision* Object = *ObjectStartIter;
				if (false == Object->IsDeath())
				{
					++ObjectStartIter;
					continue;
				}

				ObjectStartIter = Group.erase(ObjectStartIter);

			}
		}
	}
}


void GameEngineLevel::ActorRelease()
{
	MainCamera->Release();

	CollisionRelease();


	// ������ �ٲ�̴ϴ�.
	{
		std::map<int, std::list<GameEngineActor*>>::iterator GroupStartIter = AllActors.begin();
		std::map<int, std::list<GameEngineActor*>>::iterator GroupEndIter = AllActors.end();

		// ���� ��ŭ�̶� ������ ���̷��� ����.

		for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
		{
			std::list<GameEngineActor*>& Group = GroupStartIter->second;

			std::list<GameEngineActor*>::iterator ObjectStartIter = Group.begin();
			std::list<GameEngineActor*>::iterator ObjectEndIter = Group.end();

			for (; ObjectStartIter != ObjectEndIter; )
			{
				GameEngineActor* Actor = *ObjectStartIter;
				if (false == Actor->IsDeath())
				{
					Actor->ActorRelease();
					++ObjectStartIter;
					continue;
				}

				if (nullptr == Actor)
				{
					MsgBoxAssert("nullptr�� ���Ͱ� ������ ����Ʈ�� �� �־����ϴ�.");
					continue;
				}

				delete Actor;
				Actor = nullptr;

				//                      i
				// [s] [a] [a]     [a] [e]
				ObjectStartIter = Group.erase(ObjectStartIter);

			}
		}
	}

}

void GameEngineLevel::ActorLevelEnd()
{
	for (const std::pair<int, std::list<GameEngineActor*>>& _Pair : AllActors)
	{
		const std::list<GameEngineActor*>& Group = _Pair.second;

		for (GameEngineActor* _Actor : Group)
		{
			_Actor->LevelEnd();
		}
	}
}
void GameEngineLevel::ActorLevelStart()
{
	for (const std::pair<int, std::list<GameEngineActor*>>& _Pair : AllActors)
	{
		const std::list<GameEngineActor*>& Group = _Pair.second;

		for (GameEngineActor* _Actor : Group)
		{
			_Actor->LevelStart();
		}
	}
}

void GameEngineLevel::OverCheck(GameEngineLevel* _PrevLevel)
{
	if (nullptr == _PrevLevel)
	{
		return;
	}

	// �̷������� �����ϴ� �� ���Ϳ� ���õ� ��������
	_PrevLevel->MainCamera->OverRelease();
	// �̷������� �����ϴ� �� ���Ϳ� ���õ� �浹ü��
	_PrevLevel->CollisionOverRelease();

	for (std::pair<const int, std::list<GameEngineActor*>>& _Pair : _PrevLevel->AllActors)
	{
		std::list<GameEngineActor*>& Group = _Pair.second;

		std::list<GameEngineActor*>::iterator StartIter = Group.begin();
		std::list<GameEngineActor*>::iterator EndIter = Group.end();

		for (; StartIter != EndIter;)
		{
			GameEngineActor* Actor = *StartIter;

			if (false == Actor->IsLevelOver())
			{
				++StartIter;
				continue;
			}

			Actor->Level = this;

			for (GameEngineRenderer* Render : Actor->AllRenderer)
			{
				if (Render->GetCameraType() == CameraType::MAIN)
				{
					Render->MainCameraSetting();
				}
				else
				{
					Render->UICameraSetting();
				}

				Render->SetOrder(Render->GetOrder());
			}

			for (GameEngineCollision* Collision : Actor->AllCollision)
			{
				Collision->SetOrder(Collision->GetOrder());
			}




			AllActors[Actor->GetOrder()].push_back(Actor);

			StartIter = Group.erase(StartIter);
		}
	}

}