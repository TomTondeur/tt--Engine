#pragma once

class ParticleEmitterComponent
{
public:
	//Default constructor & destructor
	ParticleEmitterComponent(void);
	virtual ~ParticleEmitterComponent(void);

	//Methods

private:
	//Datamembers


	//Disabling default copy constructor & assignment operator
	ParticleEmitterComponent(const ParticleEmitterComponent& src);
	ParticleEmitterComponent& operator=(const ParticleEmitterComponent& src);
};
