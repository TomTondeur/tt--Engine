#include "Window.h"
#include "../TTengine.h"
#include "../Services/ServiceLocator.h"

Window::Window(unsigned short windowWidth, unsigned short windowHeight):m_Style(WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX)
																		,m_ExStyle(WS_EX_APPWINDOW)
																		,m_Width(windowWidth), m_Height(windowHeight)
																		,m_Name(_T("New game"))
{

}

Window::~Window(void)
{

}

//Methods

void Window::Create(TTengine* pEngine)
{
	WNDCLASSEX wndex;
	ZeroMemory(&wndex, sizeof(WNDCLASSEX));
	wndex.cbSize = sizeof(WNDCLASSEX);
	wndex.hCursor = LoadCursor(0, IDC_ARROW);
	wndex.hInstance = GetModuleHandle(0);
	wndex.lpfnWndProc = TTengine::WndProc;
	wndex.hIcon = LoadIcon(0, IDI_APPLICATION);
	wndex.lpszClassName = _T("WindowClass");
	wndex.hbrBackground = CreateSolidBrush(RGB(255, 255, 255)); 
	wndex.style = CS_HREDRAW | CS_VREDRAW;

	if(RegisterClassEx(&wndex) == 0)
		MyServiceLocator::GetInstance()->GetService<DebugService>()->LogWin32Error(GetLastError(),__LINE__,__FILE__);
	
	RECT R = {0, 0, m_Width, m_Height};
	AdjustWindowRect(&R, m_ExStyle, false);
	int windowWidth = R.right - R.left;
	int windowHeight = R.bottom - R.top;
	int topLeftX = (GetSystemMetrics(SM_CXSCREEN) - windowWidth) /2;
	int topLeftY = (GetSystemMetrics(SM_CYSCREEN) - windowHeight) /2;

	m_Handle = CreateWindowEx(m_ExStyle, wndex.lpszClassName, m_Name.c_str(), m_Style, CW_USEDEFAULT, CW_USEDEFAULT, 
							windowWidth, windowHeight, 0, 0, GetModuleHandle(0), pEngine);
	if(m_Handle == 0)
		MyServiceLocator::GetInstance()->GetService<DebugService>()->LogWin32Error(GetLastError(),__LINE__,__FILE__);

	ShowWindow(m_Handle, SW_NORMAL);
	UpdateWindow(m_Handle);

	m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo( m_hConsole, &m_DefaultConsoleInfo );
	tcout << "Window initialized properly." << endl;
	
}

HWND Window::GetHandle(void) const
{
	return m_Handle;
}

HANDLE Window::GetConsoleWindowHandle(void) const
{
	return m_hConsole;
}

const CONSOLE_SCREEN_BUFFER_INFO& Window::GetDefaultConsoleInfo(void) const
{
	return m_DefaultConsoleInfo;
}

void Window::SetName(const std::tstring& name)
{
	m_Name = name; 
	SetWindowText(m_Handle, m_Name.c_str());
}

void Window::SetDimensions(UINT width, UINT height)
{
	m_Width = width; 
	m_Height = height;
	//COMPLETE IMPLEMENTATION
}

void Window::SetPosition(POINT pos)
{
	m_WindowPos = pos;
	//COMPLETE IMPLEMENTATION
}
