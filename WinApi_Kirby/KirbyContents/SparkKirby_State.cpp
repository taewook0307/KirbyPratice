#include "SparkKirby.h"
#include "Monster.h"

#include <GameEngineBase/GameEngineTime.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>

void SparkKirby::JumpUpdate(float _Delta)
{
	Kirby::JumpUpdate(_Delta);

	if (true == GameEngineInput::IsPress('C'))
	{
		ChangeState(KirbyState::Attack);
		return;
	}
}

void SparkKirby::JumpToDropUpdate(float _Delta)
{
	Kirby::JumpToDropUpdate(_Delta);

	if (true == GameEngineInput::IsPress('C'))
	{
		ChangeState(KirbyState::Attack);
		return;
	}
}

void SparkKirby::DropUpdate(float _Delta)
{
	Kirby::DropUpdate(_Delta);

	if (true == GameEngineInput::IsPress('C'))
	{
		ChangeState(KirbyState::Attack);
		return;
	}
}

void SparkKirby::AttackUpdate(float _Delta)
{
	DirCheck();

	AttackCollision->On();

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

	if (true == GameEngineInput::IsUp('C'))
	{
		AttackCollision->Off();

		if (true == GameEngineInput::IsPress('A') || true == GameEngineInput::IsPress('D'))
		{
			ChangeState(KirbyState::Walk);
			return;
		}

		ChangeState(KirbyState::AttackToIdle);
		return;
	}
}