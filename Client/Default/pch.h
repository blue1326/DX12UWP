#pragma once

#include <wrl.h>
#include <wrl/client.h>

#include <dxgi1_6.h>

#include <d3d12.h>
#include "d3dx12.h"

#include <DirectXColors.h>
#include <DirectXMath.h>

#include <pix.h>
#include <memory>
#include <vector>
#include <queue>
#include <agile.h>
#include <concrt.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <functional>
#include <iostream>
#if defined(_DEBUG)
#include <dxgidebug.h>
#endif

using namespace std;


namespace DX
{
	// Helper class for COM exceptions
	class com_exception : public std::exception
	{
	public:
		com_exception(HRESULT hr) : result(hr) {}

		virtual const char* what() const override
		{
			static char s_str[64] = {};
			sprintf_s(s_str, "Failure with HRESULT of %08X", static_cast<unsigned int>(result));
			return s_str;
		}

	private:
		HRESULT result;
	};

	// Helper utility converts D3D API failures into exceptions.
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw com_exception(hr);
		}
	}
}