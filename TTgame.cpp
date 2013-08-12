//    __  __      ______            _          
//   / /_/ /__ _ / ____/___  ____ _(_)___  ___ 
//  / __/ __(_|_) __/ / __ \/ __ `/ / __ \/ _ \
// / /_/ /__ _ / /___/ / / / /_/ / / / / /  __/
// \__/\__(_|_)_____/_/ /_/\__, /_/_/ /_/\___/ 
//                        /____/               
//
// TTgame.cpp - Copyright © 2013 Tom Tondeur
//

//------------
// Includes
//------------
#include "Helpers/stdafx.h"
#include "TTgame.h"
#include "Helpers/Namespace.h"
#include "Timer.h"

#include "Services/ServiceLocator.h"
#include "Graphics/GraphicsDevice.h"
#include "Graphics/Material.h"

#include "TTscene.h"

//------------
// Defines
//------------
#define TTENGINE TTengine::GetInstance()

using namespace std;

//---------------------------------
// TTgame Implementation
//---------------------------------

TTgame::TTgame():AbstractGame()			// Default constructor
	,m_pFont(nullptr)
{

}

TTgame::~TTgame(void)			// Destructor
{
	//m_pFont->Release();
}

void TTgame::Initialize(void)
{/*
    D3DX10_FONT_DESC fontDesc;
    fontDesc.Height				= 24;
    fontDesc.Width				= 0;
    fontDesc.Weight				= 0;
    fontDesc.MipLevels			= 1;
    fontDesc.Italic				= false;
    fontDesc.CharSet			= DEFAULT_CHARSET;
    fontDesc.OutputPrecision	= OUT_DEFAULT_PRECIS;
    fontDesc.Quality			= DEFAULT_QUALITY;
    fontDesc.PitchAndFamily		= DEFAULT_PITCH | FF_DONTCARE;
    wcscpy_s(fontDesc.FaceName, sizeof(_T("Arial")), _T("Arial"));

	m_pFont = MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->CreateFontFromDescriptor(fontDesc);
	*/
	AddGameScene( _T(""), new TTscene() );
	SetActiveScene(_T(""));
}

void TTgame::Update(const tt::GameContext& context)
{
}

void TTgame::Draw(const tt::GameContext& context)
{
	//MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->DrawString(m_pFont,_T("Hello World!"),20,20);
}
