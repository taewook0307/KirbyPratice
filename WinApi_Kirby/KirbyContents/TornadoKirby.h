#pragma once

#include "FormKirby.h"

class TornadoKirby : public FormKirby
{
public:
	// constrcuter destructer
	TornadoKirby();
	~TornadoKirby();

	// delete Function
	TornadoKirby(const TornadoKirby& _Other) = delete;
	TornadoKirby(TornadoKirby&& _Other) noexcept = delete;
	TornadoKirby& operator=(const TornadoKirby& _Other) = delete;
	TornadoKirby& operator=(TornadoKirby&& _Other) noexcept = delete;

protected:
	void ChangeAnimationState(const std::string& _StateName) override;

	void AttackLoopUpdate(float _Delta) override;
private:
	void Start() override;
};

