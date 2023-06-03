#include "GameEngineRenderer.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineBase/GameEngineString.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineWindowTexture.h>
#include "GameEngineCamera.h"
#include "ResourcesManager.h"
#include "GameEngineActor.h"
#include "GameEngineSprite.h"
#include "GameEngineLevel.h"
#include <math.h>

GameEngineRenderer::GameEngineRenderer()
{
}

GameEngineRenderer::~GameEngineRenderer()
{
}

void GameEngineRenderer::SetSprite(const std::string& _Name, size_t _Index/* = 0*/)
{
	Sprite = ResourcesManager::GetInst().FindSprite(_Name);

	if (nullptr == Sprite)
	{
		MsgBoxAssert("�������� �ʴ� ��������Ʈ�� �����Ϸ��� �߽��ϴ�." + _Name);
	}

	const GameEngineSprite::Sprite& SpriteInfo = Sprite->GetSprite(_Index);

	Texture = SpriteInfo.BaseTexture;

	SetCopyPos(SpriteInfo.RenderPos);
	SetCopyScale(SpriteInfo.RenderScale);

}

void GameEngineRenderer::SetTexture(const std::string& _Name)
{
	Texture = ResourcesManager::GetInst().FindTexture(_Name);

	if (nullptr == Texture)
	{
		MsgBoxAssert("�������� �ʴ� �ؽ�ó�� �����Ϸ��� �߽��ϴ�." + _Name);
	}

	SetCopyPos(float4::ZERO);
	SetCopyScale(Texture->GetScale());

	if (false == ScaleCheck)
	{
		SetRenderScaleToTexture();
	}
}

void GameEngineRenderer::SetRenderScaleToTexture()
{
	if (nullptr != Texture)
	{
		RenderScale = Texture->GetScale();
	}
	ScaleCheck = false;
}

void GameEngineRenderer::TextRender(float _DeltaTime)
{
	float4 TextPos = GetActor()->GetPos() + RenderPos - Camera->GetPos();

	HDC hdc = GameEngineWindow::MainWindow.GetBackBuffer()->GetImageDC();
	HFONT hFont, OldFont;
	LOGFONTA lf;
	lf.lfHeight = TextScale;// TextHeight;
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = 0;
	lf.lfItalic = 0;
	lf.lfUnderline = 0;
	lf.lfStrikeOut = 0;
	lf.lfCharSet = HANGEUL_CHARSET;
	lf.lfOutPrecision = 0;
	lf.lfClipPrecision = 0;
	lf.lfQuality = 0;
	lf.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	// lstrcpy(lf.lfFaceName, TEXT(TextType.c_str()));
	lstrcpy(lf.lfFaceName, Face.c_str());
	hFont = CreateFontIndirect(&lf);
	OldFont = static_cast<HFONT>(SelectObject(hdc, hFont));

	//SetTextAlign(hdc, static_cast<UINT>(Align));
	SetTextColor(hdc, RGB(255, 0, 0));
	SetBkMode(hdc, TRANSPARENT);

	RECT Rect;
	Rect.left = TextPos.iX();
	Rect.top = TextPos.iY();
	Rect.right = TextPos.iX() + TextScale * static_cast<int>(Text.size());// TextBoxScale.ix();
	Rect.bottom = TextPos.iY() + TextScale;// TextBoxScale.iy();



	DrawTextA(hdc, Text.c_str(), static_cast<int>(Text.size()), &Rect, static_cast<UINT>(DT_BOTTOM));


	// TextOutA(GameEngineWindow::GetDoubleBufferImage()->GetImageDC(), RenderPos.ix(), RenderPos.iy(), RenderText.c_str(), static_cast<int>(RenderText.size()));

	SelectObject(hdc, OldFont);
	DeleteObject(hFont);

	return;
}

void GameEngineRenderer::Render(float _DeltaTime)
{
	if ("" != Text)
	{
		TextRender(_DeltaTime);
		return;
	}

	if (nullptr != CurAnimation)
	{
		if (true == CurAnimation->Loop)
		{
			CurAnimation->IsEnd = false;
		}

		CurAnimation->CurInter -= _DeltaTime;
		if (0.0f >= CurAnimation->CurInter)
		{
			++CurAnimation->CurFrame;


			if (CurAnimation->CurFrame > abs(static_cast<int>(CurAnimation->EndFrame - CurAnimation->StartFrame)))
			{
				CurAnimation->IsEnd = true;

				if (true == CurAnimation->Loop)
				{
					CurAnimation->CurFrame = 0;
				}
				else
				{
					--CurAnimation->CurFrame;
				}
			}

			CurAnimation->CurInter
				= CurAnimation->Inters[CurAnimation->CurFrame];
		}

		size_t Frame = CurAnimation->Frames[CurAnimation->CurFrame];

		Sprite = CurAnimation->Sprite;
		const GameEngineSprite::Sprite& SpriteInfo = Sprite->GetSprite(Frame);
		Texture = SpriteInfo.BaseTexture;
		SetCopyPos(SpriteInfo.RenderPos);
		SetCopyScale(SpriteInfo.RenderScale);

		if (false == ScaleCheck)
		{
			SetRenderScale(SpriteInfo.RenderScale * ScaleRatio);
		}
	}

	if (nullptr == Texture)
	{
		MsgBoxAssert("�̹����� �������� ���� ������ �Դϴ�.");
	}

	GameEngineWindowTexture* BackBuffer = GameEngineWindow::MainWindow.GetBackBuffer();

	BackBuffer->TransCopy(Texture, GetActor()->GetPos() + RenderPos - Camera->GetPos(), RenderScale, CopyPos, CopyScale);

}


GameEngineRenderer::Animation* GameEngineRenderer::FindAnimation(const std::string& _AniamtionName)
{
	std::string UpperName = GameEngineString::ToUpperReturn(_AniamtionName);

	std::map<std::string, Animation>::iterator FindIter = AllAnimation.find(UpperName);

	if (FindIter == AllAnimation.end())
	{
		return nullptr;
	}

	return &FindIter->second;
}

void GameEngineRenderer::CreateAnimation(
	const std::string& _AniamtionName,
	const std::string& _SpriteName,
	size_t _Start /*= -1*/, size_t _End /*= -1*/,
	float _Inter /*= 0.1f*/,
	bool _Loop /*= true*/)
{
	std::string UpperName = GameEngineString::ToUpperReturn(_AniamtionName);

	if (nullptr != FindAnimation(UpperName))
	{
		MsgBoxAssert("�̹� �����ϴ� �ִϸ��̼� �����Դϴ�." + UpperName);
		return;
	}

	GameEngineSprite* Sprite = ResourcesManager::GetInst().FindSprite(_SpriteName);

	if (nullptr == Sprite)
	{
		MsgBoxAssert("�������� �ʴ� ��������Ʈ�� �ִϸ��̼��� ������� �߽��ϴ�." + _SpriteName);
		return;
	}

	GameEngineRenderer::Animation& Animation = AllAnimation[UpperName];

	Animation.Sprite = Sprite;

	if (_Start != -1)
	{
		Animation.StartFrame = _Start;
	}
	else
	{
		Animation.StartFrame = 0;
	}

	if (_End != -1)
	{
		Animation.EndFrame = _End;
	}
	else
	{
		Animation.EndFrame = Animation.Sprite->GetSpriteCount() - 1;
	}

	// 0 - 5 - 5
	// ��

	// 0, 0
	Animation.Inters.resize(abs(static_cast<int>(Animation.EndFrame - Animation.StartFrame)) + 1);
	Animation.Frames.resize(abs(static_cast<int>(Animation.EndFrame - Animation.StartFrame)) + 1);

	int FrameDir = 1;

	if (_Start > _End)
	{
		FrameDir = -1;
	}

	size_t Start = Animation.StartFrame;

	for (size_t i = 0; i < Animation.Inters.size(); i++)
	{
		Animation.Frames[i] = Start;
		Animation.Inters[i] = _Inter;
		Start += FrameDir;
	}

	Animation.Loop = _Loop;

}


void GameEngineRenderer::ChangeAnimation(const std::string& _AniamtionName, int _FrameCount, bool _ForceChange)
{
	Animation* ChangeAni = FindAnimation(_AniamtionName);

	if (ChangeAni == CurAnimation && false == _ForceChange)
	{
		return;
	}

	CurAnimation = FindAnimation(_AniamtionName);

	CurAnimation->CurInter = CurAnimation->Inters[0];
	CurAnimation->CurFrame = _FrameCount;
	CurAnimation->IsEnd = false;

	if (nullptr == CurAnimation)
	{
		MsgBoxAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�." + _AniamtionName);
		return;
	}
}

void GameEngineRenderer::MainCameraSetting()
{
	Camera = GetActor()->GetLevel()->GetMainCamera();
	CameraTypeValue = CameraType::MAIN;
}

void GameEngineRenderer::UICameraSetting()
{
	Camera = GetActor()->GetLevel()->GetUICamera();
	CameraTypeValue = CameraType::UI;
}

void GameEngineRenderer::Start()
{
}

void GameEngineRenderer::SetOrder(int _Order)
{
	if (nullptr == Camera)
	{
		MsgBoxAssert("ī�޶� ���õ��� �ʾҴµ� ������ �����߽��ϴ�.");
	}

	// 0 => 5������ �ٲٰ� �ʹ�.

	// ������ �����ϴ°� �������̷� �����Ѱ� �ƴϴ�. 
	// 0�� ���� �׷�
	// 0���׷쿡���� ������ �ȴ�.
	std::list<GameEngineRenderer*>& PrevRenders = Camera->Renderers[GetOrder()];
	PrevRenders.remove(this);

	GameEngineObject::SetOrder(_Order);

	std::list<GameEngineRenderer*>& NextRenders = Camera->Renderers[GetOrder()];
	NextRenders.push_back(this);

}