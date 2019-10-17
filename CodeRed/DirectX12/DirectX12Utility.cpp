#include "../Shared/Exception/NotSupportException.hpp"
#include "../Shared/Exception/InvalidException.hpp"
#include "../Shared/Exception/Exception.hpp"

#include "../Shared/Enum/PrimitiveTopology.hpp"
#include "../Shared/Enum/ShaderVisibility.hpp"
#include "../Shared/Enum/CompareOperator.hpp"
#include "../Shared/Enum/StencilOperator.hpp"
#include "../Shared/Enum/ResourceLayout.hpp"
#include "../Shared/Enum/FilterOptions.hpp"
#include "../Shared/Enum/ResourceUsage.hpp"
#include "../Shared/Enum/BlendOperator.hpp"
#include "../Shared/Enum/ResourceType.hpp"
#include "../Shared/Enum/PixelFormat.hpp"
#include "../Shared/Enum/AddressMode.hpp"
#include "../Shared/Enum/BorderColor.hpp"
#include "../Shared/Enum/BlendFactor.hpp"
#include "../Shared/Enum/MemoryHeap.hpp"
#include "../Shared/Enum/FrontFace.hpp"
#include "../Shared/Enum/IndexType.hpp"
#include "../Shared/Enum/Dimension.hpp"
#include "../Shared/Enum/FillMode.hpp"
#include "../Shared/Enum/CullMode.hpp"


#include "DirectX12Utility.hpp"


#ifdef __ENABLE__DIRECTX12__

using namespace CodeRed::DirectX12;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

auto CodeRed::DirectX12::wideStringToMultiString(const std::wstring& wstring) -> std::string
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

auto CodeRed::DirectX12::charArrayToString(void* str, const size_t length) -> std::string
{
	std::string res = "";

	for (size_t index = 0; index < length; index++) 
		res.push_back(static_cast<char*>(str)[index]);

	return res;
}

auto CodeRed::DirectX12::enumConvert(const FilterOptions filter) -> D3D12_FILTER {
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

auto CodeRed::DirectX12::enumConvert(const AddressMode mode)
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

auto CodeRed::DirectX12::enumConvert(const BorderColor color)
	-> D3D12_STATIC_BORDER_COLOR
{
	switch (color) {
	case BorderColor::TransparentBlack: return D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	case BorderColor::OpaqueBlack: return D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
	case BorderColor::OpaqueWhite: return D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::DirectX12::enumConvert(const ResourceType type)
	-> D3D12_DESCRIPTOR_RANGE_TYPE
{
	switch (type) {
	case ResourceType::Buffer: return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	case ResourceType::Texture: return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	case ResourceType::GroupBuffer: return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::DirectX12::enumConvert(const ShaderVisibility visibility)
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

auto CodeRed::DirectX12::enumConvert(const ResourceLayout layout)
	-> D3D12_RESOURCE_STATES
{
	switch (layout) {
	case ResourceLayout::Undefined: return D3D12_RESOURCE_STATE_COMMON;
	case ResourceLayout::GeneralRead: return D3D12_RESOURCE_STATE_GENERIC_READ;
	case ResourceLayout::RenderTarget: return D3D12_RESOURCE_STATE_RENDER_TARGET;
	case ResourceLayout::DepthStencil: return D3D12_RESOURCE_STATE_DEPTH_WRITE;
	case ResourceLayout::CopyDestination: return D3D12_RESOURCE_STATE_COPY_DEST;
	case ResourceLayout::CopySource: return D3D12_RESOURCE_STATE_COPY_SOURCE;
	case ResourceLayout::Present: return D3D12_RESOURCE_STATE_PRESENT;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::DirectX12::enumConvert(const ResourceUsage usage)
	-> D3D12_RESOURCE_FLAGS
{
	//only run with macro __ENABLE_CODE_RED_DEBUG__
	//This macro is used to enable internal debug in lib
#ifdef __ENABLE__CODE__RED__DEBUG__

	//we record all masks that we can not combine
	static ResourceUsage disableMask[] = {
		ResourceUsage::RenderTarget | ResourceUsage::DepthStencil,
		ResourceUsage::VertexBuffer | ResourceUsage::RenderTarget,
		ResourceUsage::VertexBuffer | ResourceUsage::DepthStencil,
		ResourceUsage::IndexBuffer | ResourceUsage::RenderTarget,
		ResourceUsage::IndexBuffer | ResourceUsage::DepthStencil,
		ResourceUsage::ConstantBuffer | ResourceUsage::RenderTarget,
		ResourceUsage::ConstantBuffer | ResourceUsage::DepthStencil
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

auto CodeRed::DirectX12::enumConvert(const MemoryHeap heap)
	-> D3D12_HEAP_TYPE
{
	switch (heap) {
	case MemoryHeap::Default: return D3D12_HEAP_TYPE_DEFAULT;
	case MemoryHeap::Upload: return D3D12_HEAP_TYPE_UPLOAD;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::DirectX12::enumConvert(const PixelFormat format)
	-> DXGI_FORMAT
{
	switch (format) {
	case PixelFormat::RedGreenBlueAlpha8BitUnknown: return DXGI_FORMAT_R8G8B8A8_UNORM;
	case PixelFormat::BlueGreenRedAlpha8BitUnknown: return DXGI_FORMAT_B8G8R8A8_UNORM;
	case PixelFormat::RedGreenBlueAlpha32BitFloat: return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case PixelFormat::RedGreenBlue32BitFloat: return DXGI_FORMAT_R32G32B32_FLOAT;
	case PixelFormat::RedGreen32BitFloat: return DXGI_FORMAT_R32G32_FLOAT;
	case PixelFormat::Depth32BitFloat: return DXGI_FORMAT_D32_FLOAT;
	case PixelFormat::Red8BitUnknown: return DXGI_FORMAT_R8_UNORM;
	case PixelFormat::Unknown: return DXGI_FORMAT_UNKNOWN;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}	
}

auto CodeRed::DirectX12::enumConvert(const Dimension dimension)
	-> D3D12_RESOURCE_DIMENSION
{
	switch (dimension) {
	case Dimension::Dimension1D: return D3D12_RESOURCE_DIMENSION_TEXTURE1D;
	case Dimension::Dimension2D: return D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	case Dimension::Dimension3D: return D3D12_RESOURCE_DIMENSION_TEXTURE3D;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}	
}

auto CodeRed::DirectX12::enumConvert(const CompareOperator op)
	-> D3D12_COMPARISON_FUNC
{
	switch (op) {
	case CompareOperator::Never: return D3D12_COMPARISON_FUNC_NEVER;
	case CompareOperator::Less: return D3D12_COMPARISON_FUNC_LESS;
	case CompareOperator::Equal: return D3D12_COMPARISON_FUNC_EQUAL;
	case CompareOperator::LessEqual: return D3D12_COMPARISON_FUNC_LESS_EQUAL;
	case CompareOperator::Greater: return D3D12_COMPARISON_FUNC_GREATER;
	case CompareOperator::NotEqual: return D3D12_COMPARISON_FUNC_NOT_EQUAL;
	case CompareOperator::GreaterEqual: return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
	case CompareOperator::Always: return D3D12_COMPARISON_FUNC_ALWAYS;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::DirectX12::enumConvert(const StencilOperator op)
	-> D3D12_STENCIL_OP
{
	switch (op) {
	case StencilOperator::Keep: return D3D12_STENCIL_OP_KEEP;
	case StencilOperator::Zero: return D3D12_STENCIL_OP_ZERO;
	case StencilOperator::Replace: return D3D12_STENCIL_OP_REPLACE;
	case StencilOperator::IncrementAndClamp: return D3D12_STENCIL_OP_INCR_SAT;
	case StencilOperator::DecrementAndClamp: return D3D12_STENCIL_OP_DECR_SAT;
	case StencilOperator::Invert: return D3D12_STENCIL_OP_INVERT;
	case StencilOperator::IncrementAndWrap: return D3D12_STENCIL_OP_INCR;
	case StencilOperator::DecrementAndWrap: return D3D12_STENCIL_OP_DECR;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::DirectX12::enumConvert(const BlendOperator op)
	-> D3D12_BLEND_OP
{
	switch (op) {
	case BlendOperator::Add: return D3D12_BLEND_OP_ADD;
	case BlendOperator::Subtract: return D3D12_BLEND_OP_SUBTRACT;
	case BlendOperator::ReverseSubtract: return D3D12_BLEND_OP_REV_SUBTRACT;
	case BlendOperator::Min: return D3D12_BLEND_OP_MIN;
	case BlendOperator::Max: return D3D12_BLEND_OP_MAX;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::DirectX12::enumConvert(const BlendFactor factor)
	-> D3D12_BLEND
{
	switch (factor) {
	case BlendFactor::Zero: return D3D12_BLEND_ZERO;
	case BlendFactor::One: return D3D12_BLEND_ONE;
	case BlendFactor::SrcColor: return D3D12_BLEND_SRC_COLOR;
	case BlendFactor::InvSrcColor: return D3D12_BLEND_INV_SRC_COLOR;
	case BlendFactor::SrcAlpha: return D3D12_BLEND_SRC_ALPHA;
	case BlendFactor::InvSrcAlpha: return D3D12_BLEND_INV_SRC_ALPHA;
	case BlendFactor::DestAlpha: return D3D12_BLEND_DEST_ALPHA;
	case BlendFactor::InvDestAlpha: return D3D12_BLEND_INV_DEST_ALPHA;
	case BlendFactor::DescColor: return D3D12_BLEND_DEST_COLOR;
	case BlendFactor::InvDestColor: return D3D12_BLEND_INV_DEST_COLOR;
	case BlendFactor::Factor: return D3D12_BLEND_BLEND_FACTOR;
	case BlendFactor::InvFactor: return D3D12_BLEND_INV_BLEND_FACTOR;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::DirectX12::enumConvert(const ColorMask mask)
	-> D3D12_COLOR_WRITE_ENABLE
{
	//the enum is same as D3D12_COLOR_WRITE_ENABLE
	//so we only change the type
	return static_cast<D3D12_COLOR_WRITE_ENABLE>(mask);
}

auto CodeRed::DirectX12::enumConvert(const FillMode mode)
	-> D3D12_FILL_MODE
{
	switch (mode) {
	case FillMode::Wireframe: return D3D12_FILL_MODE_WIREFRAME;
	case FillMode::Solid: return D3D12_FILL_MODE_SOLID;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::DirectX12::enumConvert(const CullMode mode)
	-> D3D12_CULL_MODE
{
	switch (mode) {
	case CullMode::None: return D3D12_CULL_MODE_NONE;
	case CullMode::Front: return D3D12_CULL_MODE_FRONT;
	case CullMode::Back: return D3D12_CULL_MODE_BACK;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::DirectX12::enumConvert(const FrontFace face)
	-> bool
{
	return face == FrontFace::CounterClockwise;
}

auto CodeRed::DirectX12::enumConvert(const IndexType type) -> DXGI_FORMAT
{
	switch (type) {
	case IndexType::UInt32: return DXGI_FORMAT_R32_UINT;
	case IndexType::UInt16: return DXGI_FORMAT_R16_UINT;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::DirectX12::enumConvert(const PrimitiveTopology topology)
	-> D3D_PRIMITIVE_TOPOLOGY
{
	switch (topology) {
	case PrimitiveTopology::TriangleList: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	case PrimitiveTopology::TriangleStrip: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::DirectX12::enumConvert1(const Dimension dimension)
	-> D3D12_SRV_DIMENSION
{
	switch (dimension) {
	case Dimension::Dimension1D: return D3D12_SRV_DIMENSION_TEXTURE1D;
	case Dimension::Dimension2D: return D3D12_SRV_DIMENSION_TEXTURE2D;
	case Dimension::Dimension3D: return D3D12_SRV_DIMENSION_TEXTURE3D;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

#endif
