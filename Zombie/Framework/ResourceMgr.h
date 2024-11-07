#pragma once

template<typename T>
class ResourceMgr : public Singleton<ResourceMgr<T>>
{
	friend Singleton<ResourceMgr<T>>;

protected:
	std::unordered_set<std::string> notToUnloadResource;
	std::unordered_map<std::string, T*> resources;

	ResourceMgr() = default;
	~ResourceMgr()
	{
		for (auto pair : resources)
		{
			delete pair.second;
		}
	}

	ResourceMgr(const ResourceMgr&) = delete;
	ResourceMgr& operator=(const ResourceMgr&) = delete;

public:
	static T Empty;

	void UnloadAll()
	{
		auto it = resources.begin();

		while (it != resources.end())
		{
			if (notToUnloadResource.find(it->first) == notToUnloadResource.end())
			{
				delete it->second;
				it = resources.erase(it);
			}
			else
			{
				it++;
			}
		}
		resources.clear(); // clear°¡ Á¤È®
	}

	bool Load(const std::string& id , bool UnloadAllExclude = false)
	{
		if (resources.find(id) != resources.end())
			return false;

		T* resource = new T();
		bool success = resource->loadFromFile(id);
		if (success)
		{
			resources.insert({ id, resource });
			if (UnloadAllExclude)
				notToUnloadResource.insert(id);				
		}
		else
		{
			delete resource;
		}
		return success;
	}

	bool Unload(const std::string& id)
	{
		auto it = resources.find(id);
		if (it == resources.end())
			return false;

		delete it->second;
		resources.erase(it);

		auto find = notToUnloadResource.find(id);
		if (find != notToUnloadResource.end())
		{
			notToUnloadResource.erase(find);
		}
		return true;
	}

	T& Get(const std::string& id, bool UnloadAllExclude = false)
	{
		auto it = resources.find(id);
		if (it == resources.end() && !Load(id, UnloadAllExclude))
		{
			return Empty;
		}
		return *(resources[id]);
	}
};

template<typename T>
T ResourceMgr<T>::Empty;

#define TEXTURE_MGR (ResourceMgr<sf::Texture>::Instance())
#define FONT_MGR (ResourceMgr<sf::Font>::Instance())
#define SOUNDBUFFER_MGR (ResourceMgr<sf::SoundBuffer>::Instance())