//************************************************************
//************ INCLUDES & DEFINES ****************************
//************************************************************

#include "defines.h"

IDXGISwapChain*					swapChain = nullptr;
ID3D11DeviceContext*			deviceContext = nullptr;
ID3D11RenderTargetView*			renderTargetView = nullptr;
ID3D11Device*					device = nullptr;
D3D11_VIEWPORT					viewport;
ID3D11DepthStencilView*			depthStencilView = nullptr;
ID3D11Texture2D*				depthStencil = nullptr;
XMMATRIX						ProjectionMatrix;
//************************************************************
//************ SIMPLE WINDOWS APP CLASS **********************
//************************************************************

class DEMO_APP
{
	HINSTANCE						application;
	WNDPROC							appWndProc;
	HWND							window;

	XMMATRIX ViewMatrix;
	XTime timer;

	ID3D11InputLayout*				InputLayout = nullptr;



public:

	DEMO_APP(HINSTANCE hinst, WNDPROC proc);
	bool Run();
	bool ShutDown();
};

//************************************************************
//************ CREATION OF OBJECTS & RESOURCES ***************
//************************************************************

DEMO_APP::DEMO_APP(HINSTANCE hinst, WNDPROC proc)
{
	application = hinst;
	appWndProc = proc;

	WNDCLASSEX  wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = appWndProc;
	wndClass.lpszClassName = L"RTA Project";
	wndClass.hInstance = application;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME);
	//wndClass.hIcon			= LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FSICON));
	RegisterClassEx(&wndClass);

	RECT window_size = { 0, 0, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT };
	AdjustWindowRect(&window_size, WS_OVERLAPPEDWINDOW, false);

	window = CreateWindow(L"RTA Project", L"Scorpions Scorpion's RTA Projects", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, window_size.right - window_size.left, window_size.bottom - window_size.top,
		NULL, NULL, application, this);

	ShowWindow(window, SW_SHOW);

	DXGI_MODE_DESC modeDesc = {};
	modeDesc.Width = BACKBUFFER_WIDTH;
	modeDesc.Height = BACKBUFFER_HEIGHT;
	modeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc = modeDesc;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.SampleDesc.Count = 2; // Number of msaa
	swapChainDesc.OutputWindow = window;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, NULL, 0, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, NULL, &deviceContext);

	ID3D11Resource* pBackBuffer;
	swapChain->GetBuffer(0, __uuidof(pBackBuffer), reinterpret_cast<void**>(&pBackBuffer));
	device->CreateRenderTargetView(pBackBuffer, NULL, &renderTargetView);
	SAFE_RELEASE(pBackBuffer);

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	DXGI_SWAP_CHAIN_DESC tempDesc = {};
	swapChain->GetDesc(&tempDesc);
	viewport.Width = (float)tempDesc.BufferDesc.Width;
	viewport.Height = (float)tempDesc.BufferDesc.Height;

	ViewMatrix = XMMatrixIdentity();

	ProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(65), ASPECTRATIO, NEARPLANE, FARPLANE);

	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = BACKBUFFER_WIDTH;
	descDepth.Height = BACKBUFFER_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 2;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	result = device->CreateTexture2D(&descDepth, NULL, &depthStencil);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;

	result = device->CreateDepthStencilView(depthStencil, &depthStencilViewDesc, &depthStencilView);

	/////////////////////Real work being done Stay back 100 feet//////////////////////

	D3D11_INPUT_ELEMENT_DESC vLayout[] =
	{
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	//result = device->CreateInputLayout(vLayout, 3, Trivial_VS, sizeof(Trivial_VS), &InputLayout);

}

//************************************************************
//************ EXECUTION *************************************
//************************************************************

bool DEMO_APP::Run()
{
	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);

	if (GetAsyncKeyState('W'))
	{
		ViewMatrix = XMMatrixMultiply(XMMatrixTranslation(0, 0, 3.5f * (float)timer.Delta()), ViewMatrix);
	}
	if (GetAsyncKeyState('S'))
	{
		ViewMatrix = XMMatrixMultiply(XMMatrixTranslation(0, 0, -3.5f * (float)timer.Delta()), ViewMatrix);
	}
	if (GetAsyncKeyState('A'))
	{
		ViewMatrix = XMMatrixMultiply(XMMatrixTranslation(-3.5f * (float)timer.Delta(), 0, 0), ViewMatrix);
	}
	if (GetAsyncKeyState('D'))
	{
		ViewMatrix = XMMatrixMultiply(XMMatrixTranslation(3.5f * (float)timer.Delta(), 0, 0), ViewMatrix);
	}

	if (GetAsyncKeyState(VK_LEFT))
	{
		XMVECTOR tempPos = ViewMatrix.r[3];
		ViewMatrix.r[3] = XMVectorSet(0, 0, 0, 1);
		ViewMatrix = XMMatrixMultiply(ViewMatrix, XMMatrixRotationY((float)timer.Delta() * -2));
		ViewMatrix.r[3] = tempPos;
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		XMVECTOR tempPos = ViewMatrix.r[3];
		ViewMatrix.r[3] = XMVectorSet(0, 0, 0, 1);
		ViewMatrix = XMMatrixMultiply(ViewMatrix, XMMatrixRotationY((float)timer.Delta() * 2));
		ViewMatrix.r[3] = tempPos;
	}
	if (GetAsyncKeyState(VK_UP))
	{
		XMVECTOR tempPos = ViewMatrix.r[3];
		ViewMatrix.r[3] = XMVectorSet(0, 0, 0, 1);
		ViewMatrix = XMMatrixMultiply(XMMatrixRotationX((float)timer.Delta() * -2), ViewMatrix);
		ViewMatrix.r[3] = tempPos;
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		XMVECTOR tempPos = ViewMatrix.r[3];
		ViewMatrix.r[3] = XMVectorSet(0, 0, 0, 1);
		ViewMatrix = XMMatrixMultiply(XMMatrixRotationX((float)timer.Delta() * 2), ViewMatrix);
		ViewMatrix.r[3] = tempPos;
	}

	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);

	float color[4] = { 0, 0, 1, 1 };
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	//if (GetCursorPos(&mousePos))
	//{
	//	int deltaX = prevMousePos.x - mousePos.x;
	//	int deltaY = prevMousePos.y - mousePos.y;
	//	
	//	ViewMatrix = XMMatrixMultiply(XMMatrixRotationX(deltaY * 0.01f), ViewMatrix);
	//	ViewMatrix = XMMatrixMultiply(ViewMatrix, XMMatrixRotationY(deltaX * 0.01f));
	//}
	
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
	deviceContext->RSSetViewports(1, &viewport);

	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1, 0);

	swapChain->Present(0, 0);

	return true;
}

//************************************************************
//************ DESTRUCTION ***********************************
//************************************************************

bool DEMO_APP::ShutDown()
{
	SAFE_RELEASE(device);
	SAFE_RELEASE(deviceContext);
	SAFE_RELEASE(renderTargetView);
	SAFE_RELEASE(swapChain);
	SAFE_RELEASE(depthStencil);
	SAFE_RELEASE(depthStencilView);

	UnregisterClass(L"DirectXApplication", application);
	return true;
}

//************************************************************
//************ WINDOWS RELATED *******************************
//************************************************************

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	srand(unsigned int(time(0)));
	DEMO_APP myApp(hInstance, (WNDPROC)WndProc);
	MSG msg; ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT && myApp.Run())
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	myApp.ShutDown();
	return 0;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (GetAsyncKeyState(VK_ESCAPE))
		message = WM_DESTROY;
	switch (message)
	{
	case WM_SIZE:
	{
		if (swapChain)
		{
			deviceContext->OMSetRenderTargets(0, 0, 0);

			renderTargetView->Release();
			depthStencil->Release();
			depthStencilView->Release();

			HRESULT result = swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

			ID3D11Texture2D* buffer;
			result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

			result = device->CreateRenderTargetView(buffer, NULL, &renderTargetView);

			ID3D11Texture2D* texture;
			ID3D11Resource* res;
			D3D11_TEXTURE2D_DESC dsc;

			renderTargetView->GetResource(&res);
			res->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&texture);
			texture->GetDesc(&dsc);

			UINT height = dsc.Height;
			UINT width = dsc.Width;

			D3D11_TEXTURE2D_DESC descDepth = {};
			descDepth.Width = width;
			descDepth.Height = height;
			descDepth.MipLevels = 1;
			descDepth.ArraySize = 1;
			descDepth.Format = DXGI_FORMAT_D32_FLOAT;
			descDepth.SampleDesc.Count = 2;
			descDepth.Usage = D3D11_USAGE_DEFAULT;
			descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			result = device->CreateTexture2D(&descDepth, NULL, &depthStencil);

			D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
			depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;

			result = device->CreateDepthStencilView(depthStencil, &depthStencilViewDesc, &depthStencilView);

			buffer->Release();

			deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

			viewport.Width = (float)width;
			viewport.Height = (float)height;
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;

			deviceContext->RSSetViewports(1, &viewport);

			ProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(65), (float)width / (float)height, NEARPLANE, FARPLANE);

			texture->Release();
			res->Release();
		}
		break;
	}
	case (WM_DESTROY) : { PostQuitMessage(0); }
						break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}


XMMATRIX Movement(float time)
{
	XMMATRIX matrix;
	if (GetAsyncKeyState('W'))
	{
		matrix = XMMatrixMultiply(matrix, XMMatrixTranslation(0, 0, -1.5f * time));
	}
	if (GetAsyncKeyState('S'))
	{
		matrix = XMMatrixMultiply(matrix, XMMatrixTranslation(0, 0, 1.5f * time));
	}
	if (GetAsyncKeyState('A'))
	{
		matrix = XMMatrixMultiply(matrix, XMMatrixTranslation(1.5f * time, 0, 0));
	}
	if (GetAsyncKeyState('D'))
	{
		matrix = XMMatrixMultiply(matrix, XMMatrixTranslation(-1.5f * time, 0, 0));
	}

	if (GetAsyncKeyState(VK_LEFT))
	{
		matrix = XMMatrixMultiply(matrix, XMMatrixRotationY(time * 2));
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		matrix = XMMatrixMultiply(matrix, XMMatrixRotationY(time * -2));
	}
	if (GetAsyncKeyState(VK_UP))
	{
		matrix = XMMatrixMultiply(matrix, XMMatrixRotationX(time * 2));
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		matrix = XMMatrixMultiply(matrix, XMMatrixRotationX(time * -2));
	}

	return matrix;
}

vector<int> SortByDepth(float distances[], int numItems)
{
	vector<int> indicies;
	indicies.push_back(0);
	indicies.push_back(1);
	indicies.push_back(2);

	for (int i = 1; i < numItems; ++i)
	{
		int counter = i;
		while (counter > 0 && (distances[counter] > distances[counter - 1]))
		{
			swap(distances[counter], distances[counter - 1]);
			swap(indicies[counter], indicies[counter - 1]);
			counter--;
		}
	}

	return indicies;
}