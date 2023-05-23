#pragma once
#include <GameEnginePlatform/GameEngineWindowTexture.h>
#include <vector>

// �ؽ�ó�� �̿��ؼ� �ؽ�ó�� �Ϻκи� ������ �ϰų�.

// ���� :
class GameEngineSprite
{
public:
	class Sprite 
	{
	public:
		GameEngineWindowTexture* BaseTexture = nullptr;
		float4 RenderPos = float4::ZERO;
		float4 RenderScale = float4::ZERO;
	};

public:
	// constrcuter destructer
	GameEngineSprite();
	~GameEngineSprite();

	// delete Function
	GameEngineSprite(const GameEngineSprite& _Other) = delete;
	GameEngineSprite(GameEngineSprite&& _Other) noexcept = delete;
	GameEngineSprite& operator=(const GameEngineSprite& _Other) = delete;
	GameEngineSprite& operator=(GameEngineSprite&& _Other) noexcept = delete;

	void CreateSpriteSheet(GameEngineWindowTexture* _Texture, int _XCount, int _YCount);

	void CreateSpriteFolder(const std::string& _Path);

	const Sprite& GetSprite(size_t _Index);

	size_t GetSpriteCount() 
	{
		return AllSprite.size();
	}

protected:

private:

	std::vector<Sprite> AllSprite;
	
};

