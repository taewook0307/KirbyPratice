#pragma once
#include <string>
#include <map>
#include <GameEngineBase/GameEnginePath.h>

/*
�̱���
�����ڸ� private�� ��Ű��
���ο� static�̵� �������̵� �ڱ��ڽ��� ��ü �ϳ��� �ڽ��� ��� �ִ� ������ ����
*/

class GameEngineWindowTexture;
class ResourcesManager
{
public:
	// delete Function
	ResourcesManager(const ResourcesManager& _Other) = delete;
	ResourcesManager(ResourcesManager&& _Other) noexcept = delete;
	ResourcesManager& operator=(const ResourcesManager& _Other) = delete;
	ResourcesManager& operator=(ResourcesManager&& _Other) noexcept = delete;

	static ResourcesManager& GetInst()
	{
		return Inst;
	}

	/*
	�����͸� �̿��� �̱���
	static ResourcesManager* GetInst()
	{
		if (nullptr == Inst)
		{
			Inst = new ResourcesManager();
		}
		return Inst;
	}

	static void Destory()
	{
		if (nullptr != Inst)
		{
			delete Inst;
			Inst = nullptr;
		}
	}

	static void ResetInst()
	{
		if (nullptr != Inst)
		{
			delete Inst;
			Inst = new ResourcesManager();
		}
	}
	*/

	GameEngineWindowTexture* TextureLoad(const std::string& _Path)
	{
		GameEnginePath LoadPath = _Path;
		return TextureLoad(LoadPath.GetFileName(), _Path);
	}

	GameEngineWindowTexture* TextureLoad(const std::string& _Name, const std::string& _Path);

	GameEngineWindowTexture* FindTexture(const std::string& _Name);

	bool IsLoadTexture(const std::string& _Name);

protected:

private:
	static ResourcesManager Inst;

	std::map<std::string, GameEngineWindowTexture*> AllTexture;

	/*
	static ResourcesManager* Inst;
	*/

	// constrcuter destructer
	ResourcesManager();
	~ResourcesManager();
};