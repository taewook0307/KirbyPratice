#include "Kirby.h"

#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineRenderer.h>

void Kirby::BreatheStart()
{
	ChangeAnimationState("Breathe");
}

void Kirby::FlyStart()
{
	ChangeAnimationState("Fly");
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
}

void Kirby::FlyToTurnLandStart()
{
	ChangeAnimationState("FlyToTurnLand");
}

// ���� ���̸��ô� ����
void Kirby::BreatheUpdate(float _Delta)
{
	if (true == MainRenderer->IsAnimationEnd())
	{
		ChangeState(KirbyState::Fly);
		return;
	}
}

// ���� ����
void Kirby::FlyUpdate(float _Delta)
{
	// ���� üũ
	DirCheck();

	float4 FlyPos = float4::UP * FlyPower * _Delta;
	
	float4 MovePos = float4::ZERO;
	float4 CheckPos = float4::ZERO;

	float4 UpPos = GetPos() += FlyPos;
	// ���� ��ġ���� �ö� �� ȭ�� ������ �������� Ȯ��
	if (UpPos.Y > 100.0f)
	{
		AddPos(FlyPos);
	}

	// ���� ������ ���� ��� ������ ���ֱ�
	FlyPower -= 10.0f;

	// �ɹ� ������ �ʱⰪ���� ���� �� �ٽ� ���� �̵�
	if (true == GameEngineInput::IsDown('F'))
	{
		FlyPower = BASEPOWER;

		UpPos = GetPos() += FlyPos;

		if (UpPos.Y > 100.0f)
		{
			AddPos(FlyPos);
		}
	}

	// ������ �̵�
	if (true == GameEngineInput::IsPress('D') && Dir == ActorDir::Right)
	{
		MovePos = { Speed * _Delta, 0.0f };
		CheckPos = { 50.0f, -50.0f };
	}

	// ���� �̵�
	if (true == GameEngineInput::IsPress('A') && Dir == ActorDir::Left)
	{
		MovePos = { -Speed * _Delta, 0.0f };
		CheckPos = { -50.0f, -50.0f };
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

	// �����̽��� ������ ���� ���·� ����
	if (true == GameEngineInput::IsDown(VK_SPACE))
	{
		FlyPower = BASEPOWER;
		ChangeState(KirbyState::BreatheOut);
		return;
	}

	float4 DownCheckPos = { 0.0f, 0.0f };
	unsigned int DownColor = GetGroundColor(EMPTYCOLOR, DownCheckPos);

	// �ٴڿ� ��� FlyPos�� Y���� �Ʒ� �����̸� ���� ���·� ����
	if (EMPTYCOLOR != DownColor && FlyPos.Y > 0)
	{
		FlyPower = BASEPOWER;
		ChangeState(KirbyState::BreatheOutLand);
		return;
	}
}

// ���� ���¸� ��ġ�� �ʰ� ������ ����
void Kirby::BreatheOutLandUpdate(float _Delta)
{
	if (true == MainRenderer->IsAnimationEnd())
	{
		ChangeState(KirbyState::Idle);
		return;
	}
}

// ���߿��� �����ϱ� ���� ���⸦ ����� ����
void Kirby::BreatheOutUpdate(float _Delta)
{
	unsigned int Color = GetGroundColor(EMPTYCOLOR);

	if (EMPTYCOLOR == Color)
	{
		Gravity(_Delta);

		float4 MovePos = float4::ZERO;
		float4 CheckPos = float4::ZERO;

		// ������ �̵�
		if (true == GameEngineInput::IsPress('D') && Dir == ActorDir::Right)
		{
			MovePos = { Speed * _Delta, 0.0f };
			CheckPos = { 30.0f, -50.0f };
		}

		// ���� �̵�
		if (true == GameEngineInput::IsPress('A') && Dir == ActorDir::Left)
		{
			MovePos = { -Speed * _Delta, 0.0f };
			CheckPos = { -30.0f, -50.0f };
		}

		// �̵� ���� �տ� ��ֹ� ���� Ȯ�� �� �̵�
		{
			unsigned int XColor = GetGroundColor(EMPTYCOLOR, CheckPos);

			if (EMPTYCOLOR == XColor)
			{
				AddPos(MovePos);
				CameraMove(MovePos);
			}
		}
	}

	// ����ó��
	else
	{
		if (true == MainRenderer->IsAnimationEnd())
		{
			ChangeState(KirbyState::Idle);
			return;
		}
	}

	// �ִϸ��̼��� �� ����Ǹ� ���� ���·� ����
	if (true == MainRenderer->IsAnimationEnd())
	{
		ChangeState(KirbyState::Drop);
		return;
	}
}

// ���� ����
void Kirby::DropUpdate(float _Delta)
{
	// ���� �ð��� �˱� ���� static ��������
	static float DropTimer = 0.0f;

	unsigned int Color = GetGroundColor(EMPTYCOLOR);

	// �� ������ ���� ���
	if (EMPTYCOLOR == Color)
	{
		DropTimer += _Delta;
		Gravity(_Delta);

		float4 MovePos = float4::ZERO;
		float4 CheckPos = float4::ZERO;

		// ������ �̵�
		if (true == GameEngineInput::IsPress('D') && Dir == ActorDir::Right)
		{
			MovePos = { Speed * _Delta, 0.0f };
			CheckPos = { 30.0f, -50.0f };
		}

		// ���� �̵�
		if (true == GameEngineInput::IsPress('A') && Dir == ActorDir::Left)
		{
			CheckPos = { -30.0f, -50.0f };
			MovePos = { -Speed * _Delta, 0.0f };
		}

		// �̵� ���� �տ� ��ֹ� ���� Ȯ�� �� �̵�
		{
			unsigned int XColor = GetGroundColor(EMPTYCOLOR, CheckPos);

			if (EMPTYCOLOR == XColor)
			{
				AddPos(MovePos);
				CameraMove(MovePos);
			}
		}
	}
	// �� ������ �ƴ� ���(���鿡 ����� ���)
	else
	{
		GravityReset();
		// ������ �ð��� 1�� ���ϸ� ���� ���·� ����
		if (DropTimer < 1.0f)
		{
			DropTimer = 0.0f;
			ChangeState(KirbyState::FlyToLand);
			return;
		}
		// 1�� �̻��̸� ���� �� �� ���·� ����
		else
		{
			DropTimer = 0.0f;
			ChangeState(KirbyState::FlyToTurnUp);
			return;
		}
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

// �ٴڿ� �ε��� �� ���ϸ鼭 ��¦ ���� �ö󰡴� ����
void Kirby::FlyToTurnUpUpdate(float _Delta)
{
	float4 FlyPos = float4::UP * FlyPower * 0.7f * _Delta;
	float4 CheckPos = float4::ZERO;

	// ���� ���⿡ ��� ���� ���� �̵�
	if (Dir == ActorDir::Left)
	{
		FlyPos = { -Speed * 0.3f * _Delta, -FlyPower * 0.7f * _Delta };
		CheckPos = { -50.0f, -50.0f };
	}

	// ������ ���⿡ ��� ������ ���� �̵�
	if (Dir == ActorDir::Right)
	{
		FlyPos = { Speed * 0.3f * _Delta, -FlyPower * 0.7f * _Delta };
		CheckPos = { 50.0f, -50.0f };
	}

	unsigned int Color = GetGroundColor(EMPTYCOLOR, CheckPos);

	// ���� �ִ� �� Ȯ�� �� ������ ��ġ�� ���� �̵�
	if (EMPTYCOLOR == Color)
	{
		AddPos(FlyPos);
	}

	FlyPower -= 10.0f;
	
	// �����ִ� Pos���� �Ʒ��������� �ٲ�� FlyToTurnLand ���·� ��ȯ
	if (FlyPos.Y > 0)
	{
		ChangeState(KirbyState::FlyToTurnLand);
		return;
	}
}

// �Ʒ� �������� ����
void Kirby::FlyToTurnLandUpdate(float _Delta)
{
	float4 FlyPos = float4::UP * FlyPower * 0.7f * _Delta;
	float4 MovePos = float4::ZERO;
	float4 CheckPos = float4::ZERO;

	// ���� �������� �̵��ϴ� Pos �� �� CheckPos �� ����
	if (Dir == ActorDir::Left)
	{
		MovePos = { -Speed * 0.3f * _Delta, 0.0f };
		CheckPos = { -50.0f, -30.0f };
	}

	// ������ �������� �̵��ϴ� Pos �� �� CheckPos �� ����
	if (Dir == ActorDir::Right)
	{
		MovePos = { Speed * 0.3f * _Delta, 0.0f };
		CheckPos = { 50.0f, -30.0f };
	}

	unsigned int XColor = GetGroundColor(EMPTYCOLOR, CheckPos);
	unsigned int Color = GetGroundColor(EMPTYCOLOR);

	// ���� Ȥ�� ������ ���⿡ ���� ������� X���� Y���� ��ü �̵�
	if (EMPTYCOLOR == XColor)
	{
		AddPos(MovePos);
		AddPos(FlyPos);
	}
	else
	{
		// ���� Ȥ�� ������ ���⿡ ���� ���� ��� Y���⸸ �̵�
		if (EMPTYCOLOR == Color)
		{
			AddPos(FlyPos);
		}
		else
		{
			ChangeState(KirbyState::FlyToLand);
			return;
		}
	}

	// ������ ������ ���� ���·� �̵�
	if(EMPTYCOLOR != Color)
	{
		FlyPower = BASEPOWER;
		ChangeState(KirbyState::FlyToLand);
		return;
	}
	
	FlyPower -= 10.0f;
}