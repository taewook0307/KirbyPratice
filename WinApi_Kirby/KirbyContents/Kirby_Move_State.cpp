#include "Kirby.h"

#include <GameEngineBase/GameEngineTime.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>

void Kirby::SlideStart()
{
	ChangeAnimationState("Slide");
}

void Kirby::WalkStart()
{
	ChangeAnimationState("Walk");
}

void Kirby::RunStart()
{
	ChangeAnimationState("Run");
}

void Kirby::StopStart()
{
	ChangeAnimationState("Stop");
}

void Kirby::StopToIdleStart()
{
	ChangeAnimationState("StopToIdle");
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

void Kirby::WalkUpdate(float _Delta)
{
	DirCheck();

	KirbyGravity(_Delta);

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

	// ��ũ���� ���� �̵�
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

	// �޸��� ���� �̵�
	if (true == GameEngineInput::IsDown('E')
		|| true == GameEngineInput::IsDown('Q'))
	{
		DirCheck();
		ChangeState(KirbyState::Run);
		return;
	}
}

void Kirby::RunUpdate(float _Delta)
{
	DirCheck();

	KirbyGravity(_Delta);

	float4 MovePos = float4::ZERO;
	float4 CheckPos = float4::ZERO;

	float RunSpeed = Speed * 2.0f;

	// ���� �̵�
	if (true == GameEngineInput::IsPress('Q') && Dir == ActorDir::Left)
	{
		CheckPos = { -30.0f, -50.0f };
		MovePos = { -RunSpeed * _Delta, 0.0f };
	}

	// ������ �̵�
	if (true == GameEngineInput::IsPress('E') && Dir == ActorDir::Right)
	{
		CheckPos = { 30.0f, -50.0f };
		MovePos = { RunSpeed * _Delta, 0.0f };
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

	// ��ũ���� ���� �̵�
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
	if (true == GameEngineInput::IsUp('Q') || true == GameEngineInput::IsUp('E'))
	{
		DirCheck();
		ChangeState(KirbyState::Stop);
		return;
	}
}

void Kirby::StopUpdate(float _Delta)
{
	DirCheck();

	KirbyGravity(_Delta);

	float4 MovePos = float4::ZERO;
	float4 OppositePos = float4::ZERO;
	float4 CheckPos = float4::ZERO;

	float RunSpeed = Speed * 2.0f;

	if (ActorDir::Left == Dir)
	{
		CheckPos = { -30.0f, -50.0f };
		MovePos = { -RunSpeed * _Delta, 0.0f };
	}

	if (ActorDir::Right == Dir)
	{
		CheckPos = { 30.0f, -50.0f };
		MovePos = { RunSpeed * _Delta, 0.0f };
	}

	OppositePos = MovePos * -0.1f;

	unsigned int Color = GetGroundColor(EMPTYCOLOR, CheckPos);

	Speed *= 0.9f;

	if (EMPTYCOLOR == Color)
	{	
		AddPos(MovePos += OppositePos);
		CameraMove(MovePos += OppositePos);
	}

	if (ActorDir::Left == Dir && MovePos.X > -1.0f || ActorDir::Right == Dir && MovePos.X < 1.0f)
	{
		DirCheck();
		Speed = BASEPOWER;
		ChangeState(KirbyState::StopToIdle);
		return;
	}
}

void Kirby::StopToIdleUpdate(float _Delta)
{
	static float StopTimer = 0.0f;

	StopTimer += _Delta;

	if (StopTimer >= 0.4f)
	{
		StopTimer = 0.0f;
		ChangeState(KirbyState::Idle);
		return;
	}
}