#include "Kirby.h"

#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineRenderer.h>

void Kirby::BreatheStart()
{
	ChangeAnimationState("Breathe");
	SetGravityVector(float4::UP * FlyPower * 0.8f);
}

void Kirby::FlyStart()
{
	ChangeAnimationState("Fly");
	SetGravityVector(float4::UP * FlyPower);
}

void Kirby::BreatheOutLandStart()
{
	ChangeAnimationState("BreatheOutLand");
}

void Kirby::BreatheOutStart()
{
	ChangeAnimationState("BreatheOut");
}

void Kirby::DropStart()
{
	ChangeAnimationState("Drop");
}

void Kirby::FlyToLandStart()
{
	ChangeAnimationState("FlyToLand");
}

void Kirby::FlyToTurnUpStart()
{
	ChangeAnimationState("FlyToTurnUp");
	SetGravityVector(float4::UP * FlyPower * 0.8f);
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

	unsigned int TopColor = GetGroundColor(EMPTYCOLOR, TOPCHECKPOS);

	if (EMPTYCOLOR == TopColor || DOORCOLOR == TopColor)
	{
		FlyGravity(_Delta);
	}
	else
	{
		SetGravityVector(float4::DOWN);
		FlyGravity(_Delta);
	}

	float4 MovePos = float4::ZERO;
	float4 CheckPos = float4::ZERO;

	if (true == GameEngineInput::IsPress('A') && Dir == ActorDir::Left)
	{
		MovePos = { -Speed * _Delta, 0.0f };
		CheckPos = LEFTCHECKPOS;

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
		CheckPos = RIGHTCHECKPOS;

		unsigned int Color = GetGroundColor(EMPTYCOLOR, CheckPos);

		if (EMPTYCOLOR == Color || DOORCOLOR == Color)
		{
			AddPos(MovePos);
			CameraMove(MovePos);
		}
	}

	if (true == GameEngineInput::IsPress(VK_SPACE))
	{
		SetGravityVector(float4::UP * FlyPower);
	}

	if (true == GameEngineInput::IsPress('C'))
	{
		ChangeState(KirbyState::BreatheOut);
		return;
	}

	unsigned int BotColor = GetGroundColor(EMPTYCOLOR);

	if (EMPTYCOLOR != BotColor && DOORCOLOR != BotColor)
	{
		ChangeState(KirbyState::BreatheOutLand);
		return;
	}
}

// ���� ���¸� ��ġ�� �ʰ� ������ ����
void Kirby::BreatheOutLandUpdate(float _Delta)
{
	if (true == MainRenderer->IsAnimationEnd())
	{
		if (true == GameEngineInput::IsPress('A') || true == GameEngineInput::IsPress('D'))
		{
			ChangeState(KirbyState::Walk);
			return;
		}

		ChangeState(KirbyState::Idle);
		return;
	}
}

// ���߿��� �����ϱ� ���� ���⸦ ����� ����
void Kirby::BreatheOutUpdate(float _Delta)
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
		ChangeState(KirbyState::Drop);
		return;
	}
}

// ���� ����
void Kirby::DropUpdate(float _Delta)
{
	DirCheck();

	static float DropTimer = 0.0f;
	unsigned int Color = GetGroundColor(EMPTYCOLOR);

	if (EMPTYCOLOR == Color || DOORCOLOR == Color)
	{
		FlyGravity(_Delta);
		DropTimer += 2.0f;
	}

	if (EMPTYCOLOR != Color && DOORCOLOR != Color)
	{
		GravityReset();
		if (DropTimer > 2.0f)
		{
			DropTimer = 0.0f;
			ChangeState(KirbyState::FlyToTurnUp);
			return;
		}
		else
		{
			DropTimer = 0.0f;
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
		CheckPos = RIGHTCHECKPOS;

		unsigned int Color = GetGroundColor(EMPTYCOLOR, CheckPos);

		if (EMPTYCOLOR == Color || DOORCOLOR == Color)
		{
			AddPos(MovePos);
			CameraMove(MovePos);
		}
	}
}

// �ٴڿ� �ε��� �� ���ϸ鼭 ��¦ ���� �ö󰡴� ����
void Kirby::FlyToTurnUpUpdate(float _Delta)
{
	unsigned int TopColor = GetGroundColor(EMPTYCOLOR, TOPCHECKPOS);

	if (EMPTYCOLOR == TopColor || DOORCOLOR == TopColor)
	{
		FlyGravity(_Delta);
	}
	else
	{
		SetGravityVector(float4::DOWN);
		FlyGravity(_Delta);
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
	unsigned int Color = GetGroundColor(EMPTYCOLOR);

	if (EMPTYCOLOR == Color || DOORCOLOR == Color)
	{
		FlyGravity(_Delta);
	}
	else
	{
		GravityReset();
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
		if (true == GameEngineInput::IsPress('A') || true == GameEngineInput::IsPress('D'))
		{
			ChangeState(KirbyState::Walk);
			return;
		}

		ChangeState(KirbyState::Idle);
		return;
	}
}