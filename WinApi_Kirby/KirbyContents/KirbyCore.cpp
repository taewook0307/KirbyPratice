﻿#include "KirbyCore.h"
#include "TitleLevel.h"
#include "FirstStageLevel.h"
#include "SecondStageLevel.h"
#include "ThirdStageLevel.h"
#include "BossStageLevel.h"
#include "EndingLevel.h"

#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineCore.h>

KirbyCore::KirbyCore()
{
}

KirbyCore::~KirbyCore()
{
}

void KirbyCore::Start()
{
	GameEngineWindow::MainWindow.SetPosAndScale({ 100, 100 }, { 1280, 960 });
	
	GameEngineCore::CreateLevel<TitleLevel>("TitleLevel");
	GameEngineCore::CreateLevel<FirstStageLevel>("FirstStageLevel");
	GameEngineCore::CreateLevel<SecondStageLevel>("SecondStageLevel");
	GameEngineCore::CreateLevel<ThirdStageLevel>("ThirdStageLevel");
	GameEngineCore::CreateLevel<BossStageLevel>("BossStageLevel");
	GameEngineCore::CreateLevel<EndingLevel>("EndingLevel");

	GameEngineCore::ChangeLevel("FirstStageLevel");
}

void KirbyCore::Update(float _Delta)
{

}