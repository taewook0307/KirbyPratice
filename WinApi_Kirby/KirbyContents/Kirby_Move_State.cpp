#include "Kirby.h"

#include <GameEngineBase/GameEngineTime.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>

#define EMPTYCOLOR RGB(255,255,255)

void Kirby::SlideStart()
{
	ChangeAnimationState("Slide");
}

void Kirby::JumpStart()
{
	ChangeAnimationState("Jump");
}

void Kirby::JumpToDownStart()
{
	ChangeAnimationState("JumpToDown");
}

void Kirby::JumpToLandStart()
{
	ChangeAnimationState("JumpToLand");
}

void Kirby::WalkStart()
{
	ChangeAnimationState("Walk");
}


void Kirby::SlideUpdate(float _Delta)
{
	float4 MovePos = float4::ZERO;
	float4 CheckPos = float4::ZERO;
	float CopySpeed = Speed / 1.5f;

	static float SlideTimer = 0.0f;

	// �������� �����̵�
	if (Dir == ActorDir::Left && SlideTimer <= 1.0f)
	{
		MovePos = { -CopySpeed * _Delta, 0.0f };
		CheckPos = { -50.0f, -30.0f };

		unsigned int Color = GetGroundColor(EMPTYCOLOR, CheckPos);

		if (EMPTYCOLOR == Color)
		{
			AddPos(MovePos);
			CameraMove(MovePos);
		}
		SlideTimer += _Delta;
		return;
	}

	// ���������� �����̵�
	if (Dir == ActorDir::Right && SlideTimer <= 1.0f)
	{
		MovePos = { CopySpeed * _Delta, 0.0f };
		CheckPos = { 50.0f, -30.0f };

		unsigned int Color = GetGroundColor(EMPTYCOLOR, CheckPos);

		if (EMPTYCOLOR == Color)
		{
			AddPos(MovePos);
			CameraMove(MovePos);
		}

		SlideTimer += _Delta;
		return;
	}

	// �����̵� ���� �ִϸ��̼�
	if (SlideTimer <= 1.2f || SlideTimer >= 1.0f)
	{
		SlideTimer += _Delta;
	}

	// �����̵� ���� �� ��� ���� ��ȯ
	if (SlideTimer >= 1.2f)
	{
		SlideTimer = 0.0f;
		ChangeState(KirbyState::Idle);
		return;
	}
}

void Kirby::JumpUpdate(float _Delta)
{
	DirCheck();

	static float JumpTimer = 0.0f;

	float4 JumpPos = float4::UP * _Delta * JumpPower;

	AddPos(JumpPos);

	float4 MovePos = float4::ZERO;
	float4 CheckPos = float4::ZERO;

	if (true == GameEngineInput::IsPress('A') && Dir == ActorDir::Left)
	{
		MovePos = { -Speed * _Delta, 0.0f };
		CheckPos = { 50.0f, -30.0f };

		unsigned int Color = GetGroundColor(EMPTYCOLOR, CheckPos);

		if (EMPTYCOLOR == Color)
		{
			AddPos(MovePos);
			CameraMove(MovePos);
		}
	}

	if (true == GameEngineInput::IsPress('D') && Dir == ActorDir::Right)
	{
		MovePos = { Speed * _Delta, 0.0f };
		CheckPos = { 50.0f, -30.0f };

		unsigned int Color = GetGroundColor(EMPTYCOLOR, CheckPos);

		if (EMPTYCOLOR == Color)
		{
			AddPos(MovePos);
			CameraMove(MovePos);
		}
	}

	if (JumpTimer >= 1.0f)
	{
		JumpTimer = 0.0f;
		ChangeState(KirbyState::JumpToDown);
		return;
	}
	JumpTimer += _Delta;
}

void Kirby::JumpToDownUpdate(float _Delta)
{
	DirCheck();

	unsigned int Color = GetGroundColor(EMPTYCOLOR);

	if (RGB(255, 255, 255) == Color)
	{
		Gravity(_Delta);

		float4 MovePos = float4::ZERO;
		float4 CheckPos = float4::ZERO;

		if (true == GameEngineInput::IsPress('A') && Dir == ActorDir::Left)
		{
			MovePos = { -Speed * _Delta, 0.0f };
			CheckPos = { 50.0f, -30.0f };

			unsigned int Color = GetGroundColor(EMPTYCOLOR, CheckPos);

			if (EMPTYCOLOR == Color)
			{
				AddPos(MovePos);
				CameraMove(MovePos);
			}
		}

		if (true == GameEngineInput::IsPress('D') && Dir == ActorDir::Right)
		{
			MovePos = { Speed * _Delta, 0.0f };
			CheckPos = { 50.0f, -30.0f };

			unsigned int Color = GetGroundColor(EMPTYCOLOR, CheckPos);

			if (EMPTYCOLOR == Color)
			{
				AddPos(MovePos);
				CameraMove(MovePos);
			}
		}
	}
	else
	{
		GravityReset();
		ChangeState(KirbyState::JumpToLand);
		return;
	}
}

void Kirby::JumpToLandUpdate(float _Delta)
{
	DirCheck();

	static float LandTimer = 0.0f;

	LandTimer += _Delta;

	if (LandTimer >= 0.5f)
	{
		LandTimer = 0.0f;
		ChangeState(KirbyState::Idle);
		return;
	}
}

void Kirby::WalkUpdate(float _Delta)
{
	DirCheck();
	float4 MovePos = float4::ZERO;
	float4 CheckPos = float4::ZERO;

	// ���� �̵�
	if (true == GameEngineInput::IsPress('A') && Dir == ActorDir::Left)
	{
		CheckPos = { -30.0f, -50.0f };
		MovePos = { -Speed * _Delta, 0.0f };
	}

	// ������ �̵�
	if (true == GameEngineInput::IsPress('D') && Dir == ActorDir::Right)
	{
		CheckPos = { 30.0f, -50.0f };
		MovePos = { Speed * _Delta, 0.0f };
	}

	// �̵� ���� �տ� ��ֹ� ���� Ȯ�� �� �̵�
	{
		unsigned int Color = GetGroundColor(EMPTYCOLOR, CheckPos);

		if (EMPTYCOLOR == Color)
		{
			AddPos(MovePos);
			CameraMove(MovePos);
		}
	}

	if (true == GameEngineInput::IsDown('S'))
	{
		ChangeState(KirbyState::Down);
		return;
	}

	// ���� ���� �̵�
	if (true == GameEngineInput::IsDown(VK_SPACE))
	{
		ChangeState(KirbyState::Jump);
		return;
	}

	// �̵����� ���� �� ��� ���� �̵�
	if (MovePos == float4::ZERO)
	{
		ChangeState(KirbyState::Idle);
		return;
	}
}