#include "stdafx.h"
#include "VideoDevice.h"


VideoDevice::VideoDevice()
	: d3d9(nullptr)
	, d3dDevice9(nullptr)
	, texture(nullptr)
{
}

VideoDevice::~VideoDevice()
{


}

void VideoDevice::Init()
{
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9	stCaps;
	int			nVertexProcessing;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &stCaps);
	if (stCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		nVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		nVertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	D3DPRESENT_PARAMETERS stD3DPP;
	ZeroMemory(&stD3DPP, sizeof(D3DPRESENT_PARAMETERS));
	stD3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	stD3DPP.Windowed = TRUE;
	stD3DPP.BackBufferFormat = D3DFMT_A8R8G8B8;
	stD3DPP.EnableAutoDepthStencil = TRUE;
	stD3DPP.AutoDepthStencilFormat = D3DFMT_D24S8;
	stD3DPP.hDeviceWindow = g_hWnd;
	stD3DPP.Flags = 0;
	stD3DPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	stD3DPP.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	stD3DPP.BackBufferCount = 1;
	stD3DPP.MultiSampleQuality = 0;
	stD3DPP.MultiSampleType = D3DMULTISAMPLE_NONE;

	d3d9->CreateDevice(D3DADAPTER_DEFAULT,
					   D3DDEVTYPE_HAL,
					   g_hWnd,
					   nVertexProcessing,
					   &stD3DPP,
					   &d3dDevice9);
}

void VideoDevice::RenderBegin()
{
	d3dDevice9->Clear(
		0,
		nullptr,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 0),
		1.0f,
		0
	);


	d3dDevice9->BeginScene();

	d3dDevice9->SetTransform(D3DTS_WORLD, &worldMatrix);
}

void VideoDevice::RenderEnd()
{
	d3dDevice9->EndScene();

	d3dDevice9->Present(nullptr, nullptr, nullptr, nullptr);
}

void VideoDevice::Destroy()
{
	// 일부러 오류냄
	if (texture != nullptr)
		texture->Release();


	if (d3dDevice9 != nullptr) {
		ULONG ul = d3dDevice9->Release();
		//assert( ul == 0 && "device exists unreleased object..." );
	}

	if (d3d9 != nullptr)
		d3d9->Release();
}

LPDIRECT3DDEVICE9 VideoDevice::GetDevice()
{
	return d3dDevice9;
}



