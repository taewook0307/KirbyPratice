#pragma once
#include <string>

// ���� :
class GameEngineSound
{
	// Management
public:
	// static void Init();

	static void SoundLoad(std::string& _Name, std::string& _Path);

	// 1���� ����� �ϰ� ������ �׳� �����ϰ� ���� ��Ʈ���Ҽ� ����.
	static void SoundPlay(std::string& _Name);

	// 1���� ����� �ϰ� ���� Ư�� ���� �ȴ�.
	static void PlayBgm(std::string& _Name);

	// Ư�� �����Ǵ� ���� ����� �����.
	static void StopBgm();


public:
	// constrcuter destructer
	GameEngineSound();
	~GameEngineSound();

	// delete Function
	GameEngineSound(const GameEngineSound& _Other) = delete;
	GameEngineSound(GameEngineSound&& _Other) noexcept = delete;
	GameEngineSound& operator=(const GameEngineSound& _Other) = delete;
	GameEngineSound& operator=(GameEngineSound&& _Other) noexcept = delete;

protected:

	// GameEngineSound::SoundLoad("C:\\AAAA\\AAAA\\A\\AAA.Mp3");
	// GameEngineSound::SoundPlay("AAA.Mp3");
	// GameEngineSound::PlayBgm("AAA.Mp3");
	// GameEngineSound::StopBgm("AAA.Mp3");
private:

};

