﻿#include "FirstStageLevel.h"
#include "BackGround.h"
#include "Map.h"
#include "Kirby.h"
#include "KirbyGameEnum.h"

#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineCore.h>
#include <GameEngineCore/GameEngineCamera.h>

FirstStageLevel::FirstStageLevel()
{
}

FirstStageLevel::~FirstStageLevel()
{
}

void FirstStageLevel::Start()
{
	BackGround* Back = CreateActor<BackGround>(RenderOrder::BackGround);
	Back->Init("FirstStageLevel.Bmp");

	Map* FirstStageMap = CreateActor<Map>(RenderOrder::Map);
	FirstStageMap->Init("FirstStageMap_1.bmp");

	LevelPlayer = CreateActor<Kirby>(RenderOrder::Play);
	LevelPlayer->FormInit("NormalKirby", "Kirby_Idle_1.bmp");
}

void FirstStageLevel::Update(float _Delta)
{
	if (true == GameEngineInput::IsDown('P'))
	{
		GameEngineCore::ChangeLevel("SecondStageLevel");
	}
}

void FirstStageLevel::LevelStart(GameEngineLevel* _PrevLevel)
{
	if (nullptr == LevelPlayer)
	{
		MsgBoxAssert("플레이어를 세팅해주지 않았습니다");
	}

	float4 WinScale = GameEngineWindow::MainWindow.GetScale();
	LevelPlayer->SetPos({ WinScale.Half().Half().X, WinScale.Half().Half().Y });
	GetMainCamera()->SetPos(float4::ZERO);
}