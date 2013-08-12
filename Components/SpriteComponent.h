#pragma once

class SpriteComponent
{
public:
	//Default constructor & destructor
	SpriteComponent(void);
	virtual ~SpriteComponent(void);

	//Methods

private:
	//Datamembers


	//Disabling default copy constructor & assignment operator
	SpriteComponent(const SpriteComponent& src);
	SpriteComponent& operator=(const SpriteComponent& src);
};
