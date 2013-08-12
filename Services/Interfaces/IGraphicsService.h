#pragma once

#include "Service.h"
#include "../../Helpers/resrc_ptr.hpp"
#include "../../Helpers/Namespace.h"

class GraphicsDevice;
class TTengine;
class Window;
class Model3D;
class Material;
struct GameContext;

class IGraphicsService : public Service
{
public:
	//Default constructor & destructor
	IGraphicsService(void){};
	virtual ~IGraphicsService(void){};

	//Methods
	virtual void InitWindow(int windowWidth, int windowHeight, TTengine* pEngine)=0;

	virtual void Draw(resource_ptr<Model3D> pModel, const tt::Matrix4x4& worldMat, resource_ptr<Material> pMat, const tt::GameContext& context)=0;

	virtual GraphicsDevice* GetGraphicsDevice(void) const=0;
	virtual Window* GetWindow(void) const=0;

private:
	//Datamembers


	//Disabling default copy constructor & assignment operator
	IGraphicsService(const IGraphicsService& src);// = delete;
	IGraphicsService& operator=(const IGraphicsService& src);// = delete;
};
