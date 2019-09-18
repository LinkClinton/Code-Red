#include "../Shared/Exception/NotSupportException.hpp"
#include "../Shared/Exception/InvalidException.hpp"
#include "../Shared/Exception/Exception.hpp"

#include "../Shared/Enum/ShaderVisibility.hpp"
#include "../Shared/Enum/ResourceLayout.hpp"
#include "../Shared/Enum/FilterOptions.hpp"
#include "../Shared/Enum/ResourceUsage.hpp"
#include "../Shared/Enum/ResourceType.hpp"
#include "../Shared/Enum/PixelFormat.hpp"
#include "../Shared/Enum/AddressMode.hpp"
#include "../Shared/Enum/BorderColor.hpp"
#include "../Shared/Enum/MemoryHeap.hpp"


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

auto CodeRed::enumConvert(const ResourceLayout layout)
	-> D3D12_RESOURCE_STATES
{
	switch (layout) {
	case ResourceLayout::GeneralRead: return D3D12_RESOURCE_STATE_GENERIC_READ;
	case ResourceLayout::RenderTarget: return D3D12_RESOURCE_STATE_RENDER_TARGET;
	case ResourceLayout::DepthStencil: return D3D12_RESOURCE_STATE_DEPTH_READ | D3D12_RESOURCE_STATE_DEPTH_WRITE;
	case ResourceLayout::CopyDestination: return D3D12_RESOURCE_STATE_COPY_DEST;
	case ResourceLayout::CopySource: return D3D12_RESOURCE_STATE_COPY_SOURCE;
	case ResourceLayout::Present: return D3D12_RESOURCE_STATE_PRESENT;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::enumConvert(const ResourceUsage usage)
	-> D3D12_RESOURCE_FLAGS
{
	//only run with macro __ENABLE_CODE_RED_DEBUG__
	//This macro is used to enable internal debug in lib
#ifdef __ENABLE__CODE__RED__DEBUG__

	//we record all masks that we can not combine
	static ResourceUsage disableMask[] = {
		ResourceUsage::RenderTarget | ResourceUsage::DepthStencil
	};

	//if usage has this mask we disable, we will throw a InvalidException with nullptr
	for (auto mask : disableMask) {
		if (enumHas(usage, mask))
			throw InvalidException<ResourceUsage>({ "usage" });
	}
#endif

	static std::vector<ResourceUsage> sourcePool = {
		ResourceUsage::None,
		ResourceUsage::VertexBuffer,
		ResourceUsage::IndexBuffer,
		ResourceUsage::ConstantBuffer,
		ResourceUsage::RenderTarget,
		ResourceUsage::DepthStencil
	};

	static std::vector<D3D12_RESOURCE_FLAGS> targetPool = {
		D3D12_RESOURCE_FLAG_NONE,
		D3D12_RESOURCE_FLAG_NONE,
		D3D12_RESOURCE_FLAG_NONE,
		D3D12_RESOURCE_FLAG_NONE,
		D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	};

	auto res = D3D12_RESOURCE_FLAG_NONE;
	
	for (size_t index = 0; index < sourcePool.size(); index++) {
		if (enumHas(usage, sourcePool[index]))
			res = res | targetPool[index];
	}

	return res;
}

auto CodeRed::enumConvert(const MemoryHeap heap)
	-> D3D12_HEAP_TYPE
{
	switch (heap) {
	case MemoryHeap::Default: return D3D12_HEAP_TYPE_DEFAULT;
	case MemoryHeap::Upload: return D3D12_HEAP_TYPE_UPLOAD;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::enumConvert(const PixelFormat format)
	-> DXGI_FORMAT
{
	switch (format) {
	case PixelFormat::RedGreenBlueAlpha8BitUnknown: return DXGI_FORMAT_R8G8B8A8_UNORM;
	case PixelFormat::Unknown: return DXGI_FORMAT_UNKNOWN;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}	
}

#endif
