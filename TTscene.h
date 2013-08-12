#pragma once

#include "Scenegraph/GameScene.h"

class TTscene : public GameScene
{
public:
	//Default constructor & destructor
	TTscene(void);
	virtual ~TTscene(void);

	//Methods
	virtual void Initialize(void);

private:
	//Datamembers

	//Disabling default copy constructor & assignment operator
	TTscene(const TTscene& src);
	TTscene& operator=(const TTscene& src);
};
