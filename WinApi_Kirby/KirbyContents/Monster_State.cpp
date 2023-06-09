#include "Monster.h"

#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineRenderer.h>

void Monster::IdleStart()
{
	ChangeAnimationState("Idle");
}

void Monster::WalkStart()
{
	ChangeAnimationState("Walk");
}

void Monster::DamageStart()
{
	ChangeAnimationState("Damage");
}

void Monster::DeathStart()
{
	ChangeAnimationState("Death");
}

void Monster::IdleUpdate(float _Delta)
{
	unsigned int Color = GetGroundColor(EMPTYCOLOR);

	if (EMPTYCOLOR == Color || DOORCOLOR == Color)
	{
		Gravity(_Delta);
	}

	static float IdleTimer = 0.0f;

	if (IdleTimer > 3.0f)
	{
		IdleTimer = 0.0f;
		ChangeState(MonsterState::Walk);
		return;
	}

	if (true == BodyCollision->Collision(CollisionOrder::Attack, Col, CollisionType::Rect, CollisionType::Rect)
		|| true == BodyCollision->Collision(CollisionOrder::SpecialAttack, Col, CollisionType::Rect, CollisionType::Rect)
		|| true == BodyCollision->Collision(CollisionOrder::Inhale, Col, CollisionType::Rect, CollisionType::Rect))
	{
		ChangeState(MonsterState::Damage);
		return;
	}

	IdleTimer += _Delta;
}

void Monster::WalkUpdate(float _Delta)
{
	static float MoveTimer = 0.0f;

	float4 MovePos = float4::ZERO;
	float4 CheckPos = float4::ZERO;

	// ���� �̵�
	if (Dir == ActorDir::Left)
	{
		MovePos = { -Speed * _Delta, 0.0f };
		CheckPos = LEFTCHECKPOS;
	}

	// ������ �̵�
	if (Dir == ActorDir::Right)
	{
		MovePos = { Speed * _Delta, 0.0f };
		CheckPos = RIGHTCHECKPOS;
	}

	// �̵� ���� �տ� ��ֹ� ���� Ȯ�� �� �̵�
	{
		unsigned int Color = GetGroundColor(EMPTYCOLOR, CheckPos);

		if (EMPTYCOLOR == Color || DOORCOLOR == Color)
		{
			AddPos(MovePos);
		}
		else
		{
			DirChange();
		}
	}

	// �̵����� ���� �� ��� ���� �̵�
	if (MoveTimer >= 5.0f)
	{
		MoveTimer = 0.0f;
		ChangeState(MonsterState::Idle);
		return;
	}

	Col.reserve(Col.size() + 1);

	if (true == BodyCollision->Collision(CollisionOrder::Attack, Col, CollisionType::Rect, CollisionType::Rect)
		|| true == BodyCollision->Collision(CollisionOrder::SpecialAttack, Col, CollisionType::Rect, CollisionType::Rect)
		|| true == BodyCollision->Collision(CollisionOrder::Inhale, Col, CollisionType::Rect, CollisionType::Rect))
	{
		ChangeState(MonsterState::Damage);
		return;
	}

	MoveTimer += _Delta;
}

void Monster::DamageUpdate(float _Delta)
{
	if (true == BodyCollision->Collision(CollisionOrder::SpecialAttack, Col, CollisionType::Rect, CollisionType::Rect))
	{
		ChangeState(MonsterState::Death);
		return;
	}

	else if (true == BodyCollision->Collision(CollisionOrder::Attack, Col, CollisionType::Rect, CollisionType::Rect))
	{
		if (true == MainRenderer->IsAnimationEnd())
		{
			ChangeState(MonsterState::Death);
			return;
		}
	}

	else if (false == BodyCollision->Collision(CollisionOrder::Inhale, Col, CollisionType::Rect, CollisionType::Rect))
	{
		ChangeState(MonsterState::Idle);
		return;
	}
}

void Monster::DeathUpdate(float _Delta)
{
	if (true == MainRenderer->IsAnimationEnd())
	{
		Death();
		return;
	}
}