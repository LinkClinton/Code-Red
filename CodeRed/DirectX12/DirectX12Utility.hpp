#pragma once

#ifdef __ENABLE__DIRECTX12__

#include <wrl/client.h>
#include <dxgi1_5.h>
#include <d3d12.h>

#include <string>

using namespace Microsoft;

namespace CodeRed {

#define throwIfFailed(result, exception) if ((result) != S_OK) throw exception;

	auto wideStringToMultiString(const std::wstring& wstring)->std::string;
	
}

#endif