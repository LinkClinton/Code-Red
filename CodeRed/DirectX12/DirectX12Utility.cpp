#include "../Shared/Exception/NotSupportException.hpp"
#include "../Shared/Exception/Exception.hpp"

#include "../Shared/Enum/ShaderVisibility.hpp"
#include "../Shared/Enum/FilterOptions.hpp"
#include "../Shared/Enum/ResourceType.hpp"
#include "../Shared/Enum/AddressMode.hpp"
#include "../Shared/Enum/BorderColor.hpp"


#include "DirectX12Utility.hpp"


#ifdef __ENABLE__DIRECTX12__

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

auto CodeRed::wideStringToMultiString(const std::wstring& wstring) -> std::string
{
	const auto size = WideCharToMultiByte(
		CP_ACP, 0, wstring.c_str(),
		-1, nullptr, 0, nullptr, nullptr);

	const auto res = new char[size];

	WideCharToMultiByte(
		CP_ACP, 0, wstring.c_str(),
		-1, res, size, nullptr, nullptr);

	return res;
}

void CodeRed::throwIfFailed(HRESULT result, const Exception& exception) {
	if (result != S_OK) throw exception;
}

auto CodeRed::enumConvert(const FilterOptions filter) -> D3D12_FILTER {
	switch (filter) {
	case FilterOptions::MinPointMagPointMipPoint:
		return D3D12_FILTER_MIN_MAG_MIP_POINT;
	case FilterOptions::MinPointMagPointMipLinear:
		return D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	case FilterOptions::MinPointMagLinearMipPoint:
		return D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
	case FilterOptions::MinPointMagLinearMipLinear:
		return D3D12_FILTER_MIN_POINT_MAG_MIP_LINEAR;
	case FilterOptions::MinLinearMagPointMipPoint:
		return D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT;
	case FilterOptions::MinLinearMagPointMipLinear:
		return D3D12_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
	case FilterOptions::MinLinearMagLinearMipPoint:
		return D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	case FilterOptions::MinLinearMagLinearMipLinear:
		return D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	case FilterOptions::Anisotropy:
		return D3D12_FILTER_ANISOTROPIC;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::enumConvert(const AddressMode mode)
	-> D3D12_TEXTURE_ADDRESS_MODE
{
	switch (mode) {
	case AddressMode::Repeat: return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	case AddressMode::Mirror: return D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
	case AddressMode::Clamp: return D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	case AddressMode::Border: return D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::enumConvert(const BorderColor border)
	-> D3D12_STATIC_BORDER_COLOR
{
	switch (border) {
	case BorderColor::TransparentBlack: return D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	case BorderColor::OpaqueBlack: return D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
	case BorderColor::OpaqueWhite: return D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::enumConvert(const ResourceType type)
	-> D3D12_ROOT_PARAMETER_TYPE
{
	switch (type) {
	case ResourceType::Buffer: return D3D12_ROOT_PARAMETER_TYPE_CBV;
	case ResourceType::Texture: return D3D12_ROOT_PARAMETER_TYPE_SRV;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::enumConvert(const ShaderVisibility visibility)
	-> D3D12_SHADER_VISIBILITY
{
	switch (visibility) {
	case ShaderVisibility::All: return D3D12_SHADER_VISIBILITY_ALL;
	case ShaderVisibility::Vertex: return D3D12_SHADER_VISIBILITY_VERTEX;
	case ShaderVisibility::Pixel: return D3D12_SHADER_VISIBILITY_PIXEL;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}	
}

#endif
