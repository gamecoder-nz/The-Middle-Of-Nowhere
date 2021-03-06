#ifndef SYSTEM_MANAGER_HPP
#define SYSTEM_MANAGER_HPP

#include <vector>

class FSystem;

class FSystemManager
{
public:
	FSystemManager();
	~FSystemManager();

	template<class T, class... ARGS>
	T* AddSystem(ARGS&&... systemArgs)
	{
		T* system = new T(std::forward<ARGS>(systemArgs)...);
		Systems.push_back(system);

		return system;
	}

	void Initialise();
	void Update(float dt);

protected:

private:
	std::vector<FSystem*> Systems;
};

#endif