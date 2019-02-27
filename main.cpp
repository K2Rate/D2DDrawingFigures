#include <Windows.h>
#include <d2d1.h>

#pragma comment(lib, "d2d1.lib")

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEXA wcex;

	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIconA(NULL, IDI_APPLICATION);;
	wcex.hCursor = LoadCursorA(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)6;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "WindowFrame";
	wcex.hIconSm = NULL;

	ATOM aWnd = RegisterClassExA(&wcex);
	HWND hWnd = CreateWindowA("WindowFrame", 
		"Figures output", 
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		HWND_DESKTOP, 
		NULL, 
		NULL, 
		NULL);

	MSG msg = { };
	while (GetMessageA(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	return EXIT_SUCCESS;
}

ID2D1Factory * pFactory = NULL;
ID2D1HwndRenderTarget * pRender = NULL;

ID2D1SolidColorBrush * pRedBrush = NULL;
ID2D1SolidColorBrush * pBlackBrush = NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);
		pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(hWnd, { 640,480 }, D2D1_PRESENT_OPTIONS_NONE),
			&pRender);

		pRender->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pRedBrush);
		pRender->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);

		ShowWindow(hWnd, SW_SHOWDEFAULT);
		break;
	case WM_PAINT:
		pRender->BeginDraw();
		pRender->Clear(D2D1::ColorF(D2D1::ColorF::WhiteSmoke));

		pRender->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(300.0f, 300.0f), 100.0f, 100.0f), pBlackBrush, 5.0f);
		pRender->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(100.0f, 200.0f), 50.0f, 50.0f), pRedBrush, 5.0f);
		pRender->DrawRectangle(D2D1::RectF(100.0f, 400.0f, 200.0f, 500.0f), pBlackBrush, 5.0f);

		pRender->EndDraw();
		break;
	case WM_SIZE:
		pRender->Resize(D2D1::SizeU(LOWORD(lParam), HIWORD(lParam)));
		break;
	case WM_DESTROY:
		pBlackBrush->Release();
		pRedBrush->Release();
		pRender->Release();
		pFactory->Release();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcA(hWnd, msg, wParam, lParam);
	}

	return 0;
}