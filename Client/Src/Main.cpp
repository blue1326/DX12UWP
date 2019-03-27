#include "pch.h"
#include "Main.h"

#include <ppltasks.h>
#include "View.h"

using namespace concurrency;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
//
//using namespace Windows::ApplicationModel::Activation;
//using namespace Windows::UI::Core;
//using namespace Windows::UI::Input;
//using namespace Windows::UI::ViewManagement;
//using namespace Windows::System;
//using namespace Windows::Foundation;
//using namespace Windows::Graphics::Display;
//using namespace Windows::Devices::Enumeration;
using namespace DirectX;





ref class ViewProviderFactory : IFrameworkViewSource
{
public:
	virtual IFrameworkView^ CreateView()
	{
		return ref new CView();
	}
};


// Entry point
[Platform::MTAThread]
int __cdecl main(Platform::Array<Platform::String^>^ /*argv*/)
{
	if (!XMVerifyCPUSupport())
	{
		throw std::exception("XMVerifyCPUSupport");
	}

	auto viewProviderFactory = ref new ViewProviderFactory();
	CoreApplication::Run(viewProviderFactory);
	return 0;
}



// Exit helper
void ExitGame()
{
	Windows::ApplicationModel::Core::CoreApplication::Exit();
}
