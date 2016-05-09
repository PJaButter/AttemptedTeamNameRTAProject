#pragma once

#include <iostream>
#include <ctime>
#include "XTime.h"
#include <Windows.h>
#include <vector>
#include <fstream>
#include <thread>
#include <mutex>

using namespace std;

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

#pragma comment(lib, "d3d11.lib")
#define SAFE_RELEASE(p) { if(p) {p->Release(); p = nullptr;}}

#define BACKBUFFER_WIDTH	1000
#define BACKBUFFER_HEIGHT	1000
#define ASPECTRATIO (BACKBUFFER_WIDTH / BACKBUFFER_HEIGHT)
#define NEARPLANE 0.1f
#define FARPLANE 100
#define NUMVIEWPORTS 2

struct SIMPLE_VERTEX
{
	XMFLOAT3 pos;
	XMFLOAT4 rgba;
};

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT3 uvw;
	XMFLOAT3 nrm;
	XMFLOAT4 tan;
};

struct MATRICES
{
	XMMATRIX view;
	XMMATRIX proj;
};

// Function Prototypes
XMMATRIX Movement(float time);
vector<int> SortByDepth(float distances[], int numItems);
void MakeVertexBufferDesc(D3D11_BUFFER_DESC& desc, UINT ByteWidth, UINT ByteStride);
void MakeIndexBufferDesc(D3D11_BUFFER_DESC& desc, UINT ByteWidth, UINT ByteStride);
void MakeConstBufferDesc(D3D11_BUFFER_DESC& desc, UINT ByteWidth, UINT ByteStride);