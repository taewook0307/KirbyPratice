#pragma once

#include "FormKirby.h"

#define LEFTATTACKCOLLISIONPOS { -140.0f, -30.0f }
#define RIGHTTATTACKCOLLISIONPOS { 140.0f, -30.0f }
#define FIREATTACKCOLLISIONSCALE { 130.0f, 70.0f }

class FireKirby : public FormKirby
{
public:
	// constrcuter destructer
	FireKirby();
	~FireKirby();

	// delete Function
	FireKirby(const FireKirby& _Other) = delete;
	FireKirby(FireKirby&& _Other) noexcept = delete;
	FireKirby& operator=(const FireKirby& _Other) = delete;
	FireKirby& operator=(FireKirby&& _Other) noexcept = delete;

protected:
	void ChangeAnimationState(const std::string& _StateName) override;

	void AttackLoopUpdate(float _Delta) override;
private:
	void Start() override;
};

