#pragma once
#include <string>

/*
�̱���
�����ڸ� private�� ��Ű��
���ο� static�̵� �������̵� �ڱ��ڽ��� ��ü �ϳ��� �ڽ��� ��� �ִ� ������ ����
*/

class GameEngineTexture;
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

	void TextureLoad(const std::string& _Path)
	{

	}

	void TextureLoad(const std::string& _Name, const std::string& _Path)
	{

	}

	GameEngineTexture* FindTexture(const std::string& _Image);

	bool IsLoadTexture(const std::string& _Image);

protected:

private:
	static ResourcesManager Inst;

	/*
	static ResourcesManager* Inst;
	*/

	// constrcuter destructer
	ResourcesManager();
	~ResourcesManager();
};