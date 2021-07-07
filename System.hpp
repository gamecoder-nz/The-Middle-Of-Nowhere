#ifndef SYSTEM_HPP
#define SYSTEM_HPP

class FSystem
{
public:
	bool Enabled;

	FSystem();

	virtual void Initialise() = 0;
	virtual void Update(float dt) = 0;
};

#endif
