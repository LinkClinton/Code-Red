#include <Shared/Exception/FailedException.hpp>
#include <Shared/Exception/ZeroException.hpp>

#include "../DirectX12LogicalDevice.hpp"
#include "DirectX12Texture.hpp"

#ifdef __ENABLE__DIRECTX12__

CodeRed::DirectX12Texture::DirectX12Texture(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const ResourceInfo& info) :
	GpuTexture(device, info)
{
	CODE_RED_DEBUG_THROW_IF(
		std::get<TextureProperty>(mInfo.Property).Size == 0,
		ZeroException<size_t>({ "info.Property.Size" })
	);

}

#endif