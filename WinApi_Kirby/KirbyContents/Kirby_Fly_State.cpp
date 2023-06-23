#include "Kirby.h"

#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineRenderer.h>

void Kirby::BreatheStart()
{
	ChangeAnimationState("Breathe");
	SetGravityVector(float4::UP * FlyPower * 0.5f);
}

void Kirby::FlyStart()
{	
	ChangeAnimationState("Fly");
	SetGravityVector(float4::UP * FlyPower);
}

void Kirby::BreatheOutStart()
{
	ChangeAnimationState("BreatheOut");
}

void Kirby::DropStart()
{
	ChangeAnimationState("Drop");
	DropStartPos = GetPos();
}

void Kirby::FlyToLandStart()
{
	ChangeAnimationState("FlyToLand");
}

void Kirby::FlyToTurnUpStart()
{
	ChangeAnimationState("FlyToTurnUp");
	SetGravityVector(float4::UP * FlyPower);
}

void Kirby::FlyToTurnLandStart()
{
	ChangeAnimationState("FlyToTurnLand");
}

// ���� ���̸��ô� ����
void Kirby::BreatheUpdate(float _Delta)
{
	unsigned int Color = GetGroundColor(EMPTYCOLOR);

	if (EMPTYCOLOR == Color || DOORCOLOR == Color)
	{
		FlyGravity(_Delta);
	}
	else
	{
		GravityReset();
	}

	if (true == MainRenderer->IsAnimationEnd())
	{
		ChangeState(KirbyState::Fly);
		return;
	}
}

// ���� ����
void Kirby::FlyUpdate(float _Delta)
{
	DirCheck();

	unsigned int Color = GetGroundColor(EMPTYCOLOR);

	if (GetGravityVector().Y > 0.0f)
	{
		if (EMPTYCOLOR == Color || DOORCOLOR == Color)
		{
			FlyGravity(_Delta);
		}
		else
		{
			GravityReset();
		}
	}
	else
	{
		FlyGravity(_Delta);
	}

	float4 MovePos = float4::ZERO;
	float4 CheckPos = float4::ZERO;

	if (true == GameEngineInput::IsPress('A'))
	{
		MovePos = float4::LEFT * Speed * _Delta;
		CheckPos = LEFTCHECKPOS;
	}
	
	if (true == GameEngineInput::IsPress('D'))
	{
		MovePos = float4::RIGHT * Speed * _Delta;
		CheckPos = RIGHTCHECKPOS;
	}

	ObstacleCheck(MovePos);

	unsigned int XColor = GetGroundColor(EMPTYCOLOR, CheckPos);

	if (EMPTYCOLOR == XColor || DOORCOLOR == XColor)
	{
		AddPos(MovePos);
		CameraMove(MovePos);
	}

	if (true == GameEngineInput::IsDown(VK_SPACE))
	{
		SetGravityVector(float4::UP * FlyPower);
	}

	if (true == GameEngineInput::IsPress('C'))
	{
		ChangeState(KirbyState::BreatheOut);
		return;
	}
}

// ���߿��� �����ϱ� ���� ���⸦ ����� ����
void Kirby::BreatheOutUpdate(float _Delta)
{
	unsigned int Color = GetGroundColor(EMPTYCOLOR);

	if (EMPTYCOLOR == Color || DOORCOLOR == Color)
	{
		Gravity(_Delta);
	}
	else
	{
		GravityReset();
	}

	if (true == MainRenderer->IsAnimationEnd())
	{
		ChangeState(KirbyState::Drop);
		return;
	}
}

// ���� ����
void Kirby::DropUpdate(float _Delta)
{
	DirCheck();

	if (true == GameEngineInput::IsDown(VK_SPACE))
	{
		ChangeState(KirbyState::Breathe);
		return;
	}

	if (true == GameEngineInput::IsPress('C'))
	{
		ChangeState(KirbyState::AttackReady);
		return;
	}

	unsigned int Color = GetGroundColor(EMPTYCOLOR);

	if (EMPTYCOLOR == Color || DOORCOLOR == Color)
	{
		Gravity(_Delta);
	}

	if (EMPTYCOLOR != Color && DOORCOLOR != Color)
	{
		LandPos = GetPos();

		GravityReset();

		float DropDistance = static_cast<float>(fabs(DropStartPos.Y - LandPos.Y));

		if (DropDistance > 400.0f)
		{
			GravityReset();
			ChangeState(KirbyState::FlyToTurnUp);
			return;
		}
		else
		{
			if (true == GameEngineInput::IsPress('A') || true == GameEngineInput::IsPress('D'))
			{
				ChangeState(KirbyState::Walk);
				return;
			}

			ChangeState(KirbyState::FlyToLand);
			return;
		}
	}

	float4 MovePos = float4::ZERO;
	float4 CheckPos = float4::ZERO;

	if (true == GameEngineInput::IsPress('A') && Dir == ActorDir::Left)
	{
		MovePos = { -Speed * _Delta, 0.0f };
		CheckPos = LEFTCHECKPOS;
	}

	if (true == GameEngineInput::IsPress('D') && Dir == ActorDir::Right)
	{
		MovePos = { Speed * _Delta, 0.0f };
		CheckPos = RIGHTCHECKPOS;
	}

	ObstacleCheck(MovePos);

	unsigned int XColor = GetGroundColor(EMPTYCOLOR, CheckPos);

	if (EMPTYCOLOR == XColor || DOORCOLOR == XColor)
	{
		AddPos(MovePos);
		CameraMove(MovePos);
	}
}

// �ٴڿ� �ε��� �� ���ϸ鼭 ��¦ ���� �ö󰡴� ����
void Kirby::FlyToTurnUpUpdate(float _Delta)
{
	DirCheck();

	unsigned int TopColor = GetGroundColor(EMPTYCOLOR, TOPCHECKPOS);

	if (EMPTYCOLOR == TopColor || DOORCOLOR == TopColor)
	{
		Gravity(_Delta);
	}
	else
	{
		SetGravityVector(float4::DOWN);
		Gravity(_Delta);
	}

	if (GetGravityVector().Y >= 0.0f)
	{
		ChangeState(KirbyState::FlyToTurnLand);
		return;
	}

	float4 MovePos = float4::ZERO;
	float4 CheckPos = float4::ZERO;

	if (true == GameEngineInput::IsPress('A') && ActorDir::Left == Dir)
	{
		MovePos = float4::LEFT * Speed * 0.5f * _Delta;
		CheckPos = LEFTCHECKPOS;
	}
	if (true == GameEngineInput::IsPress('D') && ActorDir::Right == Dir)
	{
		MovePos = float4::RIGHT * Speed * 0.5f * _Delta;
		CheckPos = RIGHTCHECKPOS;
	}

	ObstacleCheck(MovePos);

	unsigned int XColor = GetGroundColor(EMPTYCOLOR, CheckPos);

	if (EMPTYCOLOR == XColor || DOORCOLOR == XColor)
	{
		AddPos(MovePos);
		CameraMove(MovePos);
	}
}

// �Ʒ� �������� ����
void Kirby::FlyToTurnLandUpdate(float _Delta)
{
	DirCheck();

	unsigned int Color = GetGroundColor(EMPTYCOLOR);

	if (EMPTYCOLOR == Color || DOORCOLOR == Color)
	{
		Gravity(_Delta);
	}
	else
	{
		GravityReset();

		if (true == GameEngineInput::IsPress('A') || true == GameEngineInput::IsPress('D'))
		{
			ChangeState(KirbyState::Walk);
			return;
		}

		ChangeState(KirbyState::FlyToLand);
		return;
	}

	float4 MovePos = float4::ZERO;
	float4 CheckPos = float4::ZERO;

	if (true == GameEngineInput::IsPress('A') && ActorDir::Left == Dir)
	{
		MovePos = float4::LEFT * Speed * 0.5f * _Delta;
		CheckPos = LEFTCHECKPOS;
	}
	if (true == GameEngineInput::IsPress('D') && ActorDir::Right == Dir)
	{
		MovePos = float4::RIGHT * Speed * 0.5f * _Delta;
		CheckPos = RIGHTCHECKPOS;
	}

	ObstacleCheck(MovePos);

	unsigned int XColor = GetGroundColor(EMPTYCOLOR, CheckPos);

	if (EMPTYCOLOR == XColor || DOORCOLOR == XColor)
	{
		AddPos(MovePos);
		CameraMove(MovePos);
	}
}

// ���� ����
void Kirby::FlyToLandUpdate(float _Delta)
{
	if (true == MainRenderer->IsAnimationEnd())
	{
		ChangeState(KirbyState::Idle);
		return;
	}
}