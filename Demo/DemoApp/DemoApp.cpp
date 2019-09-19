#include "DemoApp.hpp"

LRESULT DefaultWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_DESTROY: {
		PostQuitMessage(0);
		break;
	}
	default:
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

Demo::DemoApp::DemoApp(const std::string& name, size_t width, size_t height)
	: mName(name), mWidth(width), mHeight(height), mHwnd(nullptr), mExisted(false)
{
	const auto hInstance = GetModuleHandle(nullptr);
	const auto class_name = this->name();

	WNDCLASS appInfo;

	appInfo.style = CS_DBLCLKS;
	appInfo.lpfnWndProc = DefaultWindowProc;
	appInfo.cbClsExtra = 0;
	appInfo.cbWndExtra = 0;
	appInfo.hInstance = hInstance;
	appInfo.hIcon = static_cast<HICON>(LoadImage(nullptr, "", IMAGE_ICON, 0, 0, LR_LOADFROMFILE));
	appInfo.hCursor = LoadCursor(nullptr, IDC_ARROW);
	appInfo.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	appInfo.lpszMenuName = nullptr;
	appInfo.lpszClassName = &class_name[0];

	RegisterClass(&appInfo);

	RECT rect;

	rect.top = 0;
	rect.left = 0;
	rect.right = static_cast<UINT>(this->width());
	rect.bottom = static_cast<UINT>(this->height());

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	mHwnd = CreateWindow(&class_name[0], &class_name[0], WS_OVERLAPPEDWINDOW ^
		WS_SIZEBOX ^ WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, hInstance, nullptr);

	mExisted = true;
}

void Demo::DemoApp::show() const
{
	ShowWindow(mHwnd, SW_SHOW);
}

void Demo::DemoApp::hide() const
{
	ShowWindow(mHwnd, SW_HIDE);
}

void Demo::DemoApp::runLoop()
{
	while (mExisted == true) {
		MSG message;

		message.hwnd = mHwnd;

		while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);

			processMessage(this, message);

			if (message.message == WM_QUIT) mExisted = false;
		}

		update();
		render();
	}
}

void Demo::DemoApp::processMessage(DemoApp* app, const MSG& message)
{
	
}
