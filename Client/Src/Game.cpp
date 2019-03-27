#include "pch.h"
#include "Game.h"
#include "Globals.h"
#include "TaskMaster.h"
#include "ThreadPool.h"
using namespace DirectX;

//extern ExitGame();

CGame::CGame() noexcept(false)
{
	m_deviceResources = make_shared<DX::DeviceResources>();
	m_deviceResources->RegisterDeviceNotify(this);
}

CGame::~CGame()
{

}

void CGame::Initialize(IUnknown* window, int width, int height, DXGI_MODE_ROTATION rotation)
{
	CTaskMaster::GetInstance();
	//CThreadPool::GetInstance();
	m_deviceResources->SetWindow(window, width, height, rotation);
	m_deviceResources->CreateDeviceResources();
	CreateDeviceDependentResources();

	m_deviceResources->CreateWindowSizeDependentResources();
	CreateWindowSizeDependentResources();

}

void CGame::Tick()
{
	m_timer.Tick([&]() 
	{
		Update(m_timer);
	});

	Render();
}

void CGame::OnDeviceLost()
{
	//reset
}

void CGame::OnDeviceRestored()
{

}

void CGame::OnActivated()
{}

void CGame::OnDeactivated()
{}

void CGame::OnSuspending()
{}

void CGame::OnResuming()
{
	m_timer.ResetElapsedTime();
}

void CGame::OnWindowSizeChanged(int width, int height, DXGI_MODE_ROTATION rotation)
{
	if (!m_deviceResources->WindowSizeChanged(width, height, rotation))
		return;

	CreateWindowSizeDependentResources();
}

void CGame::ValidateDevice()
{
	m_deviceResources->ValidateDevice();
}

void CGame::NewAudioDevice()
{}

void CGame::GetDefaultSize(int& width, int& height) const
{
	width = 800;
	height = 600;
}

void CGame::Update(DX::StepTimer const& timer)
{

	PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

	PIXEndEvent();

}

void CGame::Render()
{
	//첫 시작시 렌더 금지
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}
	m_deviceResources->Prepare();
	Clear();

	auto cmdlist = m_deviceResources->GetCommandList();
	PIXBeginEvent(cmdlist, PIX_COLOR_DEFAULT, L"Render");

	//DrawGrid()
	//ID3D12DescriptorHeap* heap[]{m_resourcedescriptors}

	PIXEndEvent(cmdlist);


	//present
	PIXBeginEvent(m_deviceResources->GetCommandQueue(), PIX_COLOR_DEFAULT, L"Present");
	m_deviceResources->Present();
	
	PIXEndEvent(m_deviceResources->GetCommandQueue());

}

void CGame::Clear()
{
	auto cmdlist = m_deviceResources->GetCommandList();
	PIXBeginEvent(cmdlist, PIX_COLOR_DEFAULT, L"Clear");

	// Clear the views.
	auto rtvDescriptor = m_deviceResources->GetRenderTargetView();
	auto dsvDescriptor = m_deviceResources->GetDepthStencilView();

	cmdlist->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
	cmdlist->ClearRenderTargetView(rtvDescriptor, Colors::CornflowerBlue, 0, nullptr);
	cmdlist->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// Set the viewport and scissor rect.
	auto viewport = m_deviceResources->GetScreenViewport();
	auto scissorRect = m_deviceResources->GetScissorRect();
	cmdlist->RSSetViewports(1, &viewport);
	cmdlist->RSSetScissorRects(1, &scissorRect);

	PIXEndEvent(cmdlist);
}

void CGame::CreateDeviceDependentResources()
{

}

void CGame::CreateWindowSizeDependentResources()
{

}

void XM_CALLCONV CGame::DrawGrid(DirectX::FXMVECTOR xAxis, DirectX::FXMVECTOR yAxis, DirectX::FXMVECTOR origin, size_t xdivs, size_t ydivs, DirectX::GXMVECTOR color)
{

}
