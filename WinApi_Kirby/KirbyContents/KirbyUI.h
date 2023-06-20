#pragma once

#include <GameEngineCore/GameEngineActor.h>

#define FORMUIPOS { 0.0f, 20.0f }
#define FORMUISCALE { 120.0f, 100.0f }

#define HPBARPOS { 200.0f, 50.0f }
#define HPBARSCALE { 200.0f, 30.0f }

#define LIFECOUNTPOS { 200.0f, 0.0f }
#define LIFECOUNTSCALE { 50.0f, 40.0f }

#define LIFEICONPOS { 150.0f, 0.0f }
#define LIFEICONSCALE { 50.0f, 40.0f }

class GameEngineRenderer;
class KirbyUI : public GameEngineActor
{
public:
	// constrcuter destructer
	KirbyUI();
	~KirbyUI();

	// delete Function
	KirbyUI(const KirbyUI& _Other) = delete;
	KirbyUI(KirbyUI&& _Other) noexcept = delete;
	KirbyUI& operator=(const KirbyUI& _Other) = delete;
	KirbyUI& operator=(KirbyUI&& _Other) noexcept = delete;

protected:
	inline void IsCheckPosPointChange()
	{
		IsCheckPosPoint = !IsCheckPosPoint;
	}

	bool IsCheckPosPoint = false;
private:
	GameEngineRenderer* FormUI = nullptr;
	GameEngineRenderer* HpBar = nullptr;
	GameEngineRenderer* LifeIcon = nullptr;
	GameEngineRenderer* LifeCount = nullptr;

	void Start() override;
	void Update(float _Delta) override;

	void LevelStart() override;

	void KirbyFormUIUpdate(const std::string& _ImageName);
	void KirbyHpBarImageUpdate(const std::string& _ImageName);
	void KirbyLifeCountImageUpdate(const std::string& _ImageName);
};

