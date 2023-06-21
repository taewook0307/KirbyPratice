#include "Kirby.h"

#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineCore.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>

void Kirby::KirbyGravity(float _Delta)
{
	unsigned int Color = GetGroundColor(EMPTYCOLOR);

	if (EMPTYCOLOR == Color || DOORCOLOR == Color)
	{
		Gravity(_Delta);
	}
	else
	{
		unsigned int CheckColor = GetGroundColor(EMPTYCOLOR, float4::UP);

		while (CheckColor != EMPTYCOLOR && CheckColor != DOORCOLOR)
		{
			CheckColor = GetGroundColor(EMPTYCOLOR, float4::UP);
			AddPos(float4::UP);
		}

		GravityReset();
	}
}

void Kirby::IdleStart()
{
	ChangeAnimationState("Idle");
}

void Kirby::DownStart()
{
	ChangeAnimationState("Down");
}

void Kirby::LevelMoveStart()
{
	ChangeAnimationState("LevelMove");
}

void Kirby::ChangeStart()
{
	ChangeAnimationState("Change");
}

void Kirby::IdleUpdate(float _Delta)
{
	DirCheck();

	KirbyGravity(_Delta);

	unsigned int Color = GetGroundColor(EMPTYCOLOR);

	// �ȱ� ���� ��ȯ
	if (true == GameEngineInput::IsDown('A') && Color != EMPTYCOLOR
		|| true == GameEngineInput::IsDown('D') && Color != EMPTYCOLOR
		|| true == GameEngineInput::IsPress('A') && Color != EMPTYCOLOR
		|| true == GameEngineInput::IsPress('D') && Color != EMPTYCOLOR)
	{
		DirCheck();
		ChangeState(KirbyState::Walk);
		return;
	}

	// ��ũ���� ���� ��ȯ
	if (true == GameEngineInput::IsDown('S') && Color != EMPTYCOLOR
		|| true == GameEngineInput::IsPress('S') && Color != EMPTYCOLOR)
	{
		DirCheck();
		ChangeState(KirbyState::Down);
		return;
	}

	// ���� ���� ��ȯ
	if (true == GameEngineInput::IsDown(VK_SPACE) && Color != EMPTYCOLOR)
	{
		DirCheck();
		ChangeState(KirbyState::Jump);
		return;
	}

	// ���� ���� ��ȯ
	if (true == GameEngineInput::IsDown('C') && Color != EMPTYCOLOR)
	{
		DirCheck();
		ChangeState(KirbyState::AttackReady);
		return;
	}

	// ���� �������� �Ѿ�� ���� ��ȯ
	{
		unsigned int Color = GetGroundColor(EMPTYCOLOR, TOPCHECKPOS);

		if (true == GameEngineInput::IsDown('W') && DOORCOLOR == Color)
		{
			ChangeState(KirbyState::LevelMove);
			return;
		}
	}
}

void Kirby::DownUpdate(float _Delta)
{
	DirCheck();

	KirbyGravity(_Delta);

	// �����̵� ���� ��ȯ
	if (true == GameEngineInput::IsDown('A') && Dir == ActorDir::Left
		|| true == GameEngineInput::IsDown('D') && Dir == ActorDir::Right)
	{
		ChangeState(KirbyState::Slide);
	}

	// ��ũ���� ���� ����
	else if (true == GameEngineInput::IsPress('S'))
	{
		ChangeState(KirbyState::Down);
		return;
	}

	// ��� ���� ��ȯ
	if (true == GameEngineInput::IsUp('S'))
	{
		ChangeState(KirbyState::Idle);
		return;
	}
}

void Kirby::LevelMoveUpdate(float _Delta)
{
	if (true == MainRenderer->IsAnimationEnd())
	{
		MapChangeAnimationEnd = true;
		return;
	}
}

void Kirby::ChangeUpdate(float _Delta)
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