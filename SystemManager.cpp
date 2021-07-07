#include "SystemManager.hpp"
#include "System.hpp"

FSystemManager::FSystemManager()
{
}

FSystemManager::~FSystemManager()
{
	for (FSystem* System : Systems)
	{
		delete System;
	}
}

void FSystemManager::Update(float dt)
{
	for (FSystem* System : Systems)
	{
		if (System->Enabled == true)
		{
			System->Update(dt);
		}
	}
}
