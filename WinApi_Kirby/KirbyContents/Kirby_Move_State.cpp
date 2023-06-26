#include "Kirby.h"

#include <GameEngineBase/GameEngineTime.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>

void Kirby::InclineCheck(float4& _MovePos)
{
	if (EMPTYCOLOR == GetGroundColor(EMPTYCOLOR, _MovePos))
	{
		float4 XPos = float4::ZERO;
		float4 Dir = _MovePos.X <= 0.0f ? float4::RIGHT : float4::LEFT;

		while (FLOORCOLOR != GetGroundColor(EMPTYCOLOR, _MovePos + XPos))
		{
			XPos += Dir;

			if (abs(XPos.X) > 50.0f)
			{
				break;
			}
		}

		float4 YPos = float4::ZERO;
		while (FLOORCOLOR != GetGroundColor(EMPTYCOLOR, _MovePos + YPos))
		{
			YPos.Y += 1;

			if (YPos.Y > 60.0f)
			{
				break;
			}
		}

		if (true == BodyCollision->Collision(CollisionOrder::FloorObstacle, Col, CollisionType::Rect, CollisionType::Rect))
		{
			return;
		}

		if (abs(XPos.X) >= YPos.Y)
		{
			while (FLOORCOLOR != GetGroundColor(EMPTYCOLOR, _MovePos))
			{
				_MovePos.Y += 1;
			}
		}
		else
		{
			SlideCollision->Off();
			AddPos(_MovePos);
			CameraMove(_MovePos);
			ChangeState(KirbyState::Drop);
			return;
		}
	}
}

void Kirby::ObstacleCheck(float4& _MovePos)
{
	if (true == BodyCollision->Collision(CollisionOrder::Obstacle, Col, CollisionType::Rect, CollisionType::Rect))
	{
		_MovePos = float4::ZERO;

		while (true == BodyCollision->Collision(CollisionOrder::Obstacle, Col, CollisionType::Rect, CollisionType::Rect))
		{
			GameEngineActor* CurObstacle = Col[Col.size() - 1]->GetActor();
			float4 ObstaclePos = CurObstacle->GetPos();
			float4 DirPos = GetPos() - ObstaclePos;

			if (DirPos.X < 0.0f)
			{
				AddPos(float4::LEFT);
				CameraMove(float4::LEFT);
			}
			else
			{
				AddPos(float4::RIGHT);
				CameraMove(float4::RIGHT);
			}
		}
	}
}

void Kirby::SlideStart()
{
	ChangeAnimationState("Slide");
	GameEngineSound::SoundPlay("Slide.wav");
}

void Kirby::WalkStart()
{
	ChangeAnimationState("Walk");
}

void Kirby::RunStart()
{
	ChangeAnimationState("Run");
	GameEngineSound::SoundPlay("Run.wav");
}

void Kirby::StopStart()
{
	ChangeAnimationState("Stop");
	GameEngineSound::SoundPlay("Stop.wav");
}

void Kirby::SlideUpdate(float _Delta)
{
	SlideCollision->On();
	KirbyGravity(_Delta);

	float4 MovePos = float4::ZERO;
	float4 CheckPos = float4::ZERO;
	float CopySpeed = Speed;

	if (true == GameEngineInput::IsDown(VK_SPACE))
	{
		SlideCollision->Off();
		Speed = BASEPOWER;

		ChangeState(KirbyState::Jump);
		return;
	}

	// �������� �����̵�
	if (Dir == ActorDir::Left)
	{
		MovePos = { -CopySpeed * _Delta, 0.0f };
		CheckPos = LEFTCHECKPOS;
		SlideCollision->SetCollisionPos(LEFTSLIDECOLLISIONPOS);
	}

	// ���������� �����̵�
	if (Dir == ActorDir::Right)
	{
		MovePos = { CopySpeed * _Delta, 0.0f };
		CheckPos = RIGHTCHECKPOS;
		SlideCollision->SetCollisionPos(RIGHTSLIDECOLLISIONPOS);
	}

	InclineCheck(MovePos);

	unsigned int Color = GetGroundColor(EMPTYCOLOR, CheckPos);

	if (EMPTYCOLOR == Color || DOORCOLOR == Color)
	{
		AddPos(MovePos);
		CameraMove(MovePos);
	}

	Speed -= 250.0f * _Delta;

	if (CopySpeed < 10.0f)
	{
		SlideCollision->Off();
		Speed = BASEPOWER;

		if (true == GameEngineInput::IsPress('A') || true == GameEngineInput::IsPress('D'))
		{
			ChangeState(KirbyState::Walk);
			return;
		}

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
		MovePos = { -Speed * _Delta, 0.0f };
		CheckPos = LEFTCHECKPOS;
	}

	// ������ �̵�
	if (true == GameEngineInput::IsPress('D') && Dir == ActorDir::Right)
	{
		MovePos = { Speed * _Delta, 0.0f };
		CheckPos = RIGHTCHECKPOS;
	}

	InclineCheck(MovePos);

	ObstacleCheck(MovePos);

	// �̵� ���� �տ� ��ֹ� ���� Ȯ�� �� �̵�
	{
		unsigned int Color = GetGroundColor(EMPTYCOLOR, CheckPos);

		if (EMPTYCOLOR == Color || DOORCOLOR == Color)
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

	// ���� ���� �̵�
	if (true == GameEngineInput::IsDown('C'))
	{
		ChangeState(KirbyState::AttackReady);
		return;
	}

	if (true == GameEngineInput::IsUp('A'))
	{
		CheckInputA = true;
	}

	if (true == GameEngineInput::IsUp('D'))
	{
		CheckInputD = true;
	}

	// �̵����� ���� �� ��� ���� �̵�
	if (MovePos == float4::ZERO)
	{
		ChangeState(KirbyState::Idle);
		return;
	}
}

void Kirby::RunUpdate(float _Delta)
{
	DirCheck();

	KirbyGravity(_Delta);

	float4 MovePos = float4::ZERO;
	float4 CheckPos = float4::ZERO;

	// ���� �̵�
	if (true == GameEngineInput::IsPress('A') && Dir == ActorDir::Left)
	{
		MovePos = { -RunSpeed * _Delta, 0.0f };
		CheckPos = LEFTCHECKPOS;
	}

	// ������ �̵�
	if (true == GameEngineInput::IsPress('D') && Dir == ActorDir::Right)
	{
		MovePos = { RunSpeed * _Delta, 0.0f };
		CheckPos = RIGHTCHECKPOS;
	}

	InclineCheck(MovePos);

	ObstacleCheck(MovePos);

	// �̵� ���� �տ� ��ֹ� ���� Ȯ�� �� �̵�
	{
		unsigned int Color = GetGroundColor(EMPTYCOLOR, CheckPos);

		if (EMPTYCOLOR == Color || DOORCOLOR == Color)
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

	// ���� ���� �̵�
	if (true == GameEngineInput::IsDown('C'))
	{
		ChangeState(KirbyState::AttackReady);
		return;
	}

	// �̵����� ���� �� ��� ���� �̵�
	if (MovePos.X == 0.0f)
	{
		ChangeState(KirbyState::Stop);
		return;
	}
}

void Kirby::StopUpdate(float _Delta)
{
	KirbyGravity(_Delta);

	float4 MovePos = float4::ZERO;
	float4 OppositePos = float4::ZERO;
	float4 CheckPos = float4::ZERO;

	if (ActorDir::Left == Dir)
	{
		MovePos = { -RunSpeed * _Delta, 0.0f };
		CheckPos = LEFTCHECKPOS;
	}

	if (ActorDir::Right == Dir)
	{
		MovePos = { RunSpeed * _Delta, 0.0f };
		CheckPos = RIGHTCHECKPOS;
	}

	OppositePos = MovePos * -0.5f;

	InclineCheck(MovePos);
	ObstacleCheck(MovePos);

	unsigned int Color = GetGroundColor(EMPTYCOLOR, CheckPos);

	Speed *= 0.8f;

	if (EMPTYCOLOR == Color || DOORCOLOR == Color)
	{	
		AddPos(MovePos += OppositePos);
		CameraMove(MovePos += OppositePos);
	}

	if (Speed < 1.0f)
	{
		Speed = BASEPOWER;
		DirCheck();

		if (true == GameEngineInput::IsPress('A') || true == GameEngineInput::IsPress('D'))
		{
			ChangeState(KirbyState::Walk);
			return;
		}

		ChangeState(KirbyState::Idle);
		return;
	}
}