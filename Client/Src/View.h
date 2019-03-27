
#include <ppltasks.h>
using namespace concurrency;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::UI::ViewManagement;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Windows::Devices::Enumeration;

#include "Game.h"

ref class CView sealed : public IFrameworkView
{
public:
	CView();
	virtual ~CView();

	// IFrameworkView methods
	virtual void Initialize(CoreApplicationView^ applicationView);
	

	virtual void Uninitialize();

	virtual void SetWindow(CoreWindow^ window);
	

	virtual void Load(Platform::String^ entryPoint);
	

	virtual void Run();
	

protected:
	// Event handlers
	void OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args);
	

	void OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args);
	
	void OnResuming(Platform::Object^ sender, Platform::Object^ args);
	
	void OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args);
	

#if defined(NTDDI_WIN10_RS2) && (NTDDI_VERSION >= NTDDI_WIN10_RS2)
	void OnResizeStarted(CoreWindow^ sender, Platform::Object^ args);


	void OnResizeCompleted(CoreWindow^ sender, Platform::Object^ args);
	
#endif

	void OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args);
	

	void OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args);
	

	void OnAcceleratorKeyActivated(CoreDispatcher^, AcceleratorKeyEventArgs^ args);
	

	void OnBackRequested(Platform::Object^, Windows::UI::Core::BackRequestedEventArgs^ args);
	

	void OnDpiChanged(DisplayInformation^ sender, Object^ args);
	

	void OnOrientationChanged(DisplayInformation^ sender, Object^ args);
	

	void OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args);
	

	void OnAudioDeviceAdded(Windows::Devices::Enumeration::DeviceWatcher^ sender, Windows::Devices::Enumeration::DeviceInformation^ args);
	
	void OnAudioDeviceUpdated(Windows::Devices::Enumeration::DeviceWatcher^ sender, Windows::Devices::Enumeration::DeviceInformationUpdate^ args);
	

private:
	bool                    m_exit;
	bool                    m_visible;
	bool                    m_in_sizemove;
	float                   m_DPI;
	float                   m_logicalWidth;
	float                   m_logicalHeight;
	std::unique_ptr<CGame>   m_game;

	Windows::Graphics::Display::DisplayOrientations	m_nativeOrientation;
	Windows::Graphics::Display::DisplayOrientations	m_currentOrientation;

	Windows::Devices::Enumeration::DeviceWatcher^ m_audioWatcher;

	inline int ConvertDipsToPixels(float dips) const
	{
		return int(dips * m_DPI / 96.f + 0.5f);
	}

	inline float ConvertPixelsToDips(int pixels) const
	{
		return (float(pixels) * 96.f / m_DPI);
	}

	DXGI_MODE_ROTATION ComputeDisplayRotation() const;
	
	void HandleWindowSizeChanged();
	
};
