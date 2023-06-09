#include "FireKirby.h"
#include "Monster.h"

#include <GameEngineBase/GameEngineTime.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>

void FireKirby::JumpToDropUpdate(float _Delta)
{
	Kirby::JumpToDropUpdate(_Delta);

	if (true == GameEngineInput::IsPress('C'))
	{
		ChangeState(KirbyState::Attack);
		return;
	}
}

void FireKirby::AttackUpdate(float _Delta)
{
	DirCheck();

	unsigned int Color = GetGroundColor(EMPTYCOLOR);

	if (EMPTYCOLOR == Color || DOORCOLOR == Color)
	{
		Gravity(_Delta);

		float4 MovePos = float4::ZERO;
		float4 CheckPos = float4::ZERO;

		if (true == GameEngineInput::IsPress('A') && Dir == ActorDir::Left)
		{
			MovePos = { -Speed * _Delta, 0.0f };
			CheckPos = LEFTBOTCHECKPOS;

			unsigned int Color = GetGroundColor(EMPTYCOLOR, CheckPos);

			if (EMPTYCOLOR == Color || DOORCOLOR == Color)
			{
				AddPos(MovePos);
				CameraMove(MovePos);
			}
		}

		if (true == GameEngineInput::IsPress('D') && Dir == ActorDir::Right)
		{
			MovePos = { Speed * _Delta, 0.0f };
			CheckPos = RIGHTBOTCHECKPOS;

			unsigned int Color = GetGroundColor(EMPTYCOLOR, CheckPos);

			if (EMPTYCOLOR == Color || DOORCOLOR == Color)
			{
				AddPos(MovePos);
				CameraMove(MovePos);
			}
		}

		if (true == GameEngineInput::IsUp('C'))
		{
			ChangeState(KirbyState::JumpToDrop);
			return;
		}
	}
	else
	{
		GravityReset();
	}

	if (true == GameEngineInput::IsPress('C'))
	{
		if (ActorDir::Left == Dir)
		{
			AttackCollision->SetCollisionPos(LEFTATTACKCOLLISIONPOS);
			AttackCollision->On();
		}

		else
		{
			AttackCollision->SetCollisionPos(RIGHTTATTACKCOLLISIONPOS);
			AttackCollision->On();
		}
	}

	if (true == AttackCollision->Collision(CollisionOrder::MonsterBody, Col, CollisionType::Rect, CollisionType::Rect))
	{
		float Timer = 0.0f;
		GameEngineCollision* MonsterCollision = Col[Col.size() - 1];
		GameEngineActor* MonsterPtr = MonsterCollision->GetActor();
		Monster* KeepMonster = dynamic_cast<Monster*>(MonsterPtr);

		if (Timer > 3.0f)
		{
			KeepMonster->Death();
			Timer = 0.0f;
		}

		Timer += _Delta;
	}

	if (true == GameEngineInput::IsUp('C'))
	{
		AttackCollision->Off();
		ChangeState(KirbyState::AttackToIdle);
		return;
	}
}