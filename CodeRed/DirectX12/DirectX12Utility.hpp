#pragma once

#ifdef __ENABLE__DIRECTX12__

#include <wrl/client.h>
#include <dxgi1_5.h>
#include <d3d12.h>

#include <string>
#include <vector>

using namespace Microsoft;

namespace CodeRed {

	class Exception;

	enum class ShaderVisibility : UInt32;
	enum class FilterOptions : UInt32;
	enum class ResourceType : UInt32;
	enum class AddressMode : UInt32;
	enum class BorderColor : UInt32;
	
	auto wideStringToMultiString(const std::wstring& wstring)->std::string;

	
	void throwIfFailed(HRESULT result, const Exception& exception);

	auto enumConvert(const FilterOptions filter) -> D3D12_FILTER;

	auto enumConvert(const AddressMode mode) -> D3D12_TEXTURE_ADDRESS_MODE;

	auto enumConvert(const BorderColor border) -> D3D12_STATIC_BORDER_COLOR;

	auto enumConvert(const ResourceType type) -> D3D12_ROOT_PARAMETER_TYPE;
	
	auto enumConvert(const ShaderVisibility visibility) -> D3D12_SHADER_VISIBILITY;
}

#endif