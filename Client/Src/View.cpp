#include "pch.h"
#include "View.h"

CView::CView()
	:
	m_exit(false),
	m_visible(true),
	m_in_sizemove(false),
	m_DPI(96.f),
	m_logicalWidth(800.f),
	m_logicalHeight(600.f),
	m_nativeOrientation(DisplayOrientations::None),
	m_currentOrientation(DisplayOrientations::None)
{

}

CView::~CView()
{

}

void CView::Initialize(CoreApplicationView^ applicationView)
{
	applicationView->Activated +=
		ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &CView::OnActivated);

	CoreApplication::Suspending +=
		ref new EventHandler<SuspendingEventArgs^>(this, &CView::OnSuspending);

	CoreApplication::Resuming +=
		ref new EventHandler<Platform::Object^>(this, &CView::OnResuming);

	m_game = std::make_unique<CGame>();

	m_audioWatcher = DeviceInformation::CreateWatcher(DeviceClass::AudioRender);

	m_audioWatcher->Added += ref new TypedEventHandler<DeviceWatcher^, DeviceInformation^>(this, &CView::OnAudioDeviceAdded);
	m_audioWatcher->Updated += ref new TypedEventHandler<DeviceWatcher^, DeviceInformationUpdate^>(this, &CView::OnAudioDeviceUpdated);

	m_audioWatcher->Start();
}

void CView::Uninitialize()
{
	m_game.reset();
}

void CView::SetWindow(CoreWindow^ window)
{
	window->SizeChanged +=
		ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &CView::OnWindowSizeChanged);

#if defined(NTDDI_WIN10_RS2) && (NTDDI_VERSION >= NTDDI_WIN10_RS2)
	try
	{
		window->ResizeStarted +=
			ref new TypedEventHandler<CoreWindow^, Object^>(this, &CView::OnResizeStarted);

		window->ResizeCompleted +=
			ref new TypedEventHandler<CoreWindow^, Object^>(this, &CView::OnResizeCompleted);
	}
	catch (...)
	{
		// Requires Windows 10 Creators Update (10.0.15063) or later
	}
#endif

	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &CView::OnVisibilityChanged);

	window->Closed +=
		ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &CView::OnWindowClosed);

	auto dispatcher = CoreWindow::GetForCurrentThread()->Dispatcher;

	dispatcher->AcceleratorKeyActivated +=
		ref new TypedEventHandler<CoreDispatcher^, AcceleratorKeyEventArgs^>(this, &CView::OnAcceleratorKeyActivated);

	auto navigation = Windows::UI::Core::SystemNavigationManager::GetForCurrentView();

	navigation->BackRequested +=
		ref new EventHandler<BackRequestedEventArgs^>(this, &CView::OnBackRequested);

	auto currentDisplayInformation = DisplayInformation::GetForCurrentView();

	currentDisplayInformation->DpiChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &CView::OnDpiChanged);

	currentDisplayInformation->OrientationChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &CView::OnOrientationChanged);

	DisplayInformation::DisplayContentsInvalidated +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &CView::OnDisplayContentsInvalidated);

	m_DPI = currentDisplayInformation->LogicalDpi;

	m_logicalWidth = window->Bounds.Width;
	m_logicalHeight = window->Bounds.Height;

	m_nativeOrientation = currentDisplayInformation->NativeOrientation;
	m_currentOrientation = currentDisplayInformation->CurrentOrientation;

	int outputWidth = ConvertDipsToPixels(m_logicalWidth);
	int outputHeight = ConvertDipsToPixels(m_logicalHeight);

	DXGI_MODE_ROTATION rotation = ComputeDisplayRotation();

	if (rotation == DXGI_MODE_ROTATION_ROTATE90 || rotation == DXGI_MODE_ROTATION_ROTATE270)
	{
		std::swap(outputWidth, outputHeight);
	}

	m_game->Initialize(reinterpret_cast<IUnknown*>(window),
		outputWidth, outputHeight, rotation);
}

void CView::Load(Platform::String^ entryPoint)
{

}

void CView::Run()
{
	while (!m_exit)
	{
		if (m_visible)
		{
			m_game->Tick();

			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
		}
		else
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
		}
	}
}

void CView::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
{
	if (args->Kind == ActivationKind::Launch)
	{
		auto launchArgs = static_cast<LaunchActivatedEventArgs^>(args);

		if (launchArgs->PrelaunchActivated)
		{
			// Opt-out of Prelaunch
			CoreApplication::Exit();
			return;
		}
	}

	int w, h;
	m_game->GetDefaultSize(w, h);

	m_DPI = DisplayInformation::GetForCurrentView()->LogicalDpi;

	ApplicationView::PreferredLaunchWindowingMode = ApplicationViewWindowingMode::PreferredLaunchViewSize;
	// TODO: Change to ApplicationViewWindowingMode::FullScreen to default to full screen

	auto desiredSize = Size(ConvertPixelsToDips(w), ConvertPixelsToDips(h));

	ApplicationView::PreferredLaunchViewSize = desiredSize;

	auto view = ApplicationView::GetForCurrentView();

	auto minSize = Size(ConvertPixelsToDips(320), ConvertPixelsToDips(200));

	view->SetPreferredMinSize(minSize);

	CoreWindow::GetForCurrentThread()->Activate();

	view->FullScreenSystemOverlayMode = FullScreenSystemOverlayMode::Minimal;

	view->TryResizeView(desiredSize);
}

void CView::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
{
	auto deferral = args->SuspendingOperation->GetDeferral();

	create_task([this, deferral]()
	{
		m_game->OnSuspending();

		deferral->Complete();
	});
}

void CView::OnResuming(Platform::Object^ sender, Platform::Object^ args)
{
	m_game->OnResuming();
}

void CView::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
	m_logicalWidth = sender->Bounds.Width;
	m_logicalHeight = sender->Bounds.Height;

	if (m_in_sizemove)
		return;

	HandleWindowSizeChanged();
}

void CView::OnResizeStarted(CoreWindow^ sender, Platform::Object^ args)
{
	m_in_sizemove = true;
}

void CView::OnResizeCompleted(CoreWindow^ sender, Platform::Object^ args)
{
	m_in_sizemove = false;

	HandleWindowSizeChanged();
}

void CView::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	m_visible = args->Visible;
	if (m_visible)
	{
		m_game->OnActivated();
	}
	else
	{
		m_game->OnDeactivated();
	}	
}

void CView::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
	m_exit = true;
}

void CView::OnAcceleratorKeyActivated(CoreDispatcher^, AcceleratorKeyEventArgs^ args)
{
	if (args->EventType == CoreAcceleratorKeyEventType::SystemKeyDown
		&& args->VirtualKey == VirtualKey::Enter
		&& args->KeyStatus.IsMenuKeyDown
		&& !args->KeyStatus.WasKeyDown)
	{
		// Implements the classic ALT+ENTER fullscreen toggle
		auto view = ApplicationView::GetForCurrentView();

		if (view->IsFullScreenMode)
			view->ExitFullScreenMode();
		else
			view->TryEnterFullScreenMode();

		args->Handled = true;
	}
}

void CView::OnBackRequested(Platform::Object^, Windows::UI::Core::BackRequestedEventArgs^ args)
{
	// UWP on Xbox One triggers a back request whenever the B button is pressed
	  // which can result in the app being suspended if unhandled
	args->Handled = true;
}

void CView::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
	m_DPI = sender->LogicalDpi;

	HandleWindowSizeChanged();
}

void CView::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
	auto resizeManager = CoreWindowResizeManager::GetForCurrentView();
	resizeManager->ShouldWaitForLayoutCompletion = true;

	m_currentOrientation = sender->CurrentOrientation;

	HandleWindowSizeChanged();

	resizeManager->NotifyLayoutCompleted();
}

void CView::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
	m_game->ValidateDevice();
}

void CView::OnAudioDeviceAdded(Windows::Devices::Enumeration::DeviceWatcher^ sender, Windows::Devices::Enumeration::DeviceInformation^ args)
{
	m_game->NewAudioDevice();
}

void CView::OnAudioDeviceUpdated(Windows::Devices::Enumeration::DeviceWatcher^ sender, Windows::Devices::Enumeration::DeviceInformationUpdate^ args)
{
	m_game->NewAudioDevice();
}

DXGI_MODE_ROTATION CView::ComputeDisplayRotation() const
{
	DXGI_MODE_ROTATION rotation = DXGI_MODE_ROTATION_UNSPECIFIED;

	switch (m_nativeOrientation)
	{
	case DisplayOrientations::Landscape:
		switch (m_currentOrientation)
		{
		case DisplayOrientations::Landscape:
			rotation = DXGI_MODE_ROTATION_IDENTITY;
			break;

		case DisplayOrientations::Portrait:
			rotation = DXGI_MODE_ROTATION_ROTATE270;
			break;

		case DisplayOrientations::LandscapeFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE180;
			break;

		case DisplayOrientations::PortraitFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE90;
			break;
		}
		break;

	case DisplayOrientations::Portrait:
		switch (m_currentOrientation)
		{
		case DisplayOrientations::Landscape:
			rotation = DXGI_MODE_ROTATION_ROTATE90;
			break;

		case DisplayOrientations::Portrait:
			rotation = DXGI_MODE_ROTATION_IDENTITY;
			break;

		case DisplayOrientations::LandscapeFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE270;
			break;

		case DisplayOrientations::PortraitFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE180;
			break;
		}
		break;
	}

	return rotation;
}

void CView::HandleWindowSizeChanged()
{
	int outputWidth = ConvertDipsToPixels(m_logicalWidth);
	int outputHeight = ConvertDipsToPixels(m_logicalHeight);

	DXGI_MODE_ROTATION rotation = ComputeDisplayRotation();

	if (rotation == DXGI_MODE_ROTATION_ROTATE90 || rotation == DXGI_MODE_ROTATION_ROTATE270)
	{
		std::swap(outputWidth, outputHeight);
	}

	m_game->OnWindowSizeChanged(outputWidth, outputHeight, rotation);
}
