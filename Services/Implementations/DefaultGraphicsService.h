#pragma once

#include "../Interfaces/IGraphicsService.h"

class DefaultGraphicsService : public IGraphicsService
{
public:
	//Default constructor & destructor
	DefaultGraphicsService(void);
	virtual ~DefaultGraphicsService(void);

	//Methods
	virtual void InitWindow(int windowWidth, int windowHeight, TTengine* pEngine) override;	

	virtual void Draw(resource_ptr<Model3D> pModel, const tt::Matrix4x4& worldMat, resource_ptr<Material> pMat, const tt::GameContext& context) override;

	virtual GraphicsDevice* GetGraphicsDevice(void) const override;
	virtual Window* GetWindow(void) const override;

private:
	//Datamembers
	GraphicsDevice* m_pGraphicsDevice;
	Window* m_pWindow;

	//Disabling default copy constructor & assignment operator
	DefaultGraphicsService(const DefaultGraphicsService& src);
	DefaultGraphicsService& operator=(const DefaultGraphicsService& src);
};
