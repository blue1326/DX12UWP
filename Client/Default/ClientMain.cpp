#include "pch.h"
#include "ClientMain.h"
#include "DirectXHelper.h"

using namespace Client;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

// DirectX 12 ���� ���α׷� ���ø��� ���� ������ https://go.microsoft.com/fwlink/?LinkID=613670&clcid=0x412�� ���� �ֽ��ϴ�.

// ���� ���α׷��� �ε�Ǹ� ���� ���α׷� �ڻ��� �ε��ϰ� �ʱ�ȭ�մϴ�.
ClientMain::ClientMain()
{
	// TODO: �⺻ ���� timestep ��� �ܿ� �ٸ� ������ �Ϸ��� Ÿ�̸� ������ �����մϴ�.
	// ��: 60FPS ���� timestep ������Ʈ ������ ��� ������ ȣ���մϴ�.
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
}

// �������� ����� �ʱ�ȭ�մϴ�.
void ClientMain::CreateRenderers(const std::shared_ptr<DX::DeviceResources>& deviceResources)
{
	// TODO: �� �׸��� �� ������ �ʱ�ȭ�� ��ü�մϴ�.
	m_sceneRenderer = std::unique_ptr<Sample3DSceneRenderer>(new Sample3DSceneRenderer(deviceResources));

	OnWindowSizeChanged();
}

// �����Ӵ� �� �� ���� ���α׷� ���¸� ������Ʈ�մϴ�.
void ClientMain::Update()
{
	// ��� ��ü�� ������Ʈ�մϴ�.
	m_timer.Tick([&]()
	{
		// TODO: �� �׸��� �� ������ ������Ʈ �Լ��� ��ü�մϴ�.
		m_sceneRenderer->Update(m_timer);
	});
}

// ���� ���� ���α׷� ���¿� ���� ���� �������� �������մϴ�.
// �������� �������Ǿ� ǥ���� �غ� �Ǹ� true�� ��ȯ�մϴ�.
bool ClientMain::Render()
{
	// ó�� ������Ʈ�ϱ� ���� �ƹ��͵� ���������� ������.
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}

	// ��� ��ü�� �������մϴ�.
	// TODO: �� �׸��� �� ������ ������ �Լ��� ��ü�մϴ�.
	return m_sceneRenderer->Render();
}

// â�� ũ�Ⱑ ���ϸ�(��: ��ġ ���� ����) ���� ���α׷� ���¸� ������Ʈ�մϴ�.
void ClientMain::OnWindowSizeChanged()
{
	// TODO: �� �׸��� �� �������� ũ�� ���� �ʱ�ȭ�� ��ü�մϴ�.
	m_sceneRenderer->CreateWindowSizeDependentResources();
}

// �Ͻ� �ߴ� ���°� �ƴ� ���� �˷� �ݴϴ�.
void ClientMain::OnSuspending()
{
	// TODO: �̰��� ���� �Ͻ� �ߴ� ������ �ٲټ���.

	// �������� ���μ��� ���� ������ �Ͻ� ������ ���� ������ ���� �ֽ��ϴ�. ����
	// ���� �ߴܵ� ��ġ���� ���� �ٽ� ���۵� �� �ֵ��� ���� ��� ���¸� �����ϴ� ���� �����ϴ�. 

	m_sceneRenderer->SaveState();

	// ���� ���α׷����� �ٽ� ����Ⱑ ���� ���� �޸� �Ҵ��� ����ϴ� ���,
	// �ش� �޸𸮸� �������Ͽ� �ٸ� ���� ���α׷��� ����� �� �ֵ��� �غ�����.
}

// �� �̻� �Ͻ� �ߴ� ���°� �ƴ� ���� �˷� �ݴϴ�.
void ClientMain::OnResuming()
{
	// TODO: �̰��� ���� �ٽ� ���� ������ �ٲټ���.
}

// �������� �ʿ��� ��ġ ���ҽ��� �������� �˸��ϴ�.
void ClientMain::OnDeviceRemoved()
{
	// TODO: �ʿ��� ���� ���α׷� �Ǵ� ������ ���¸� �����ϰ� �������� �������ϼ���.
	// �� �� �̻� ��ȿ���� ���� ���ҽ��Դϴ�.
	m_sceneRenderer->SaveState();
	m_sceneRenderer = nullptr;
}