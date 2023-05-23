#pragma once
#include <string>
#include <map>
#include <GameEngineBase/GameEnginePath.h>

// ���� : �������� ���������� �˷��ִ� �̱���
// �̱����� ����?
// �̱����� ������������� 1990�� ���α׷��� ������
// ��ټ��� ���α׷��Ӱ� ����� �԰��� �ڵ��� �ϴ°��� �����ϰ� �̸��� ���ΰ��� 
// ������ �����̶�� �ϴµ� ���߿��� �ϳ��� ���ϸ�
// ���� UML�� �Բ� ���°��� ����.
// ���� ���п� �ش�
// ���� ��ü�� �����ϴ� ����� 
// ���� �Ҿ� �����θ� �˰������� �˴ϴ�.
// �� ���α׷����� �� ��ü�� 1���� �־����� ���ڴ�.
class GameEngineSprite;
class GameEngineWindowTexture;
class ResourcesManager
{
public:
	// delete Function
	ResourcesManager(const ResourcesManager& _Other) = delete;
	ResourcesManager(ResourcesManager&& _Other) noexcept = delete;
	ResourcesManager& operator=(const ResourcesManager& _Other) = delete;
	ResourcesManager& operator=(ResourcesManager&& _Other) noexcept = delete;

	// 3. �������ش�.
	static ResourcesManager& GetInst() 
	{
		return Inst;
	}

	//static ResourcesManager* GetInst()
	//{
	//	if (nullptr == Inst)
	//	{
	//		Inst = new ResourcesManager();
	//	}
	//	return Inst;
	//}

	//static void ResetInst() 
	//{
	//	if (nullptr != Inst)
	//	{
	//		delete Inst;
	//  Inst = new ResourcesManager();
	//}

	//static void Destroy() 
	//{
	//	if (nullptr != Inst)
	//	{
	//		delete Inst;
	//	}
	//}

	// ���ϸ��� �� ã������ �̸��� �ȴ�.
	GameEngineWindowTexture* TextureLoad(const std::string& _Path) 
	{
		GameEnginePath LoadPath = _Path;
		return TextureLoad(LoadPath.GetFileName(), _Path);
	}

	GameEngineWindowTexture* TextureLoad(const std::string& _Name, const std::string& _Path);

	GameEngineWindowTexture* FindTexture(const std::string& _Name);

	GameEngineSprite* FindSprite(const std::string& _Name);


	GameEngineSprite* CreateSpriteFolder(const std::string& _Path) 
	{
		GameEnginePath FolderPath = _Path;

		return CreateSpriteFolder(FolderPath.GetFileName(), _Path);
	}

	GameEngineSprite* CreateSpriteFolder(const std::string& _SpriteName, const std::string& _Path);

	// ������ �ִ� �ָ� �ڸ��� ��ɺ��� ����ڽ��ϴ�.
	// _TexturePath ��ο� �����ϴ� �ؽ�ó�� �ε��ϰ� 
	// _SpriteName �̸��� ��������Ʈ�� ����ϴ�.
	GameEngineSprite* CreateSpriteSheet(const std::string& _TexturePath, int _XCount, int _YCount)
	{
		GameEnginePath Path = _TexturePath;

		return CreateSpriteSheet(Path.GetFileName(), _TexturePath, _XCount, _YCount);
	}

	GameEngineSprite* CreateSpriteSheet(const std::string& _SpriteName, const std::string& _TexturePath, int _XCount, int _YCount);

	bool IsLoadTexture(const std::string& _Image);

	void TextureFileLoad(const std::string& _FileName, const std::string& _Path);
	void SpriteFileLoad(const std::string& _FileName, const std::string& _Path, int _XCount, int _YCount);


protected:

private:
	// 2. ���ο� static �����͵� ���̵� ���� 1���� �д�.
	static ResourcesManager Inst;

	// static ResourcesManager* Inst;

	// 1. �����ڸ� private�ȿ� �ִ´�.
	ResourcesManager();
	~ResourcesManager();

	std::map<std::string, GameEngineWindowTexture*> AllTexture;

	std::map<std::string, GameEngineSprite*> AllSprite;
};

