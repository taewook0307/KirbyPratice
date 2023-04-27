#pragma once
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineBase/GameEngineString.h>
#include <Windows.h>
#include <string>
#include <map>

// ���� :
class GameEngineLevel;
class GameEngineCore
{
public:
	// constrcuter destructer
	GameEngineCore();
	~GameEngineCore();

	// delete Function
	GameEngineCore(const GameEngineCore& _Other) = delete;
	GameEngineCore(GameEngineCore&& _Other) noexcept = delete;
	GameEngineCore& operator=(const GameEngineCore& _Other) = delete;
	GameEngineCore& operator=(GameEngineCore&& _Other) noexcept = delete;

	static void EngineStart(const std::string& _Title, HINSTANCE _Inst);

	template<typename LevelType>
	static void CreateLevel(const std::string& _Name)
	{
		std::string Upper = GameEngineString::ToUpperReturn(_Name);

		// �̹� ���ο� TitleLevel�� �����Ѵ�.
		if (AllLevel.end() != AllLevel.find(Upper))
		{
			MsgBoxAssert(Upper + "�� �̸��� ���� GameEngineLevel�� �̹� �����մϴ�.");
			return;
		}

		GameEngineLevel* NewLevel = new LevelType();

		AllLevel.insert(std::make_pair(Upper, NewLevel));

		//std::pair<std::map<std::string, class GameEngineLevel*>::iterator, bool> Pair 
		//	= AllLevel.insert(std::make_pair(_Title, nullptr));

		//if (false == Pair.second)
		//{
		//	MsgBoxAssert("�̹� �����ϴ� �̸��� ������ �� ������� �߽��ϴ�" + _Title);
		//	return;
		//}
	}


protected:

private:
	static std::string WindowTitle;

	static void CoreStart(HINSTANCE _Inst);
	static void CoreUpdate();
	static void CoreEnd();

	static std::map<std::string, GameEngineLevel*> AllLevel;
};
