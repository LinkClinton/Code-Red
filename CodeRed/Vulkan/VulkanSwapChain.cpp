#include "VulkanResource/VulkanTexture.hpp"

#include "VulkanLogicalDevice.hpp"
#include "VulkanSwapChain.hpp"

#ifdef __ENABLE__VULKAN__

CodeRed::VulkanSwapChain::VulkanSwapChain(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuCommandQueue>& queue, 
	const WindowInfo& info, 
	const PixelFormat& format,
	const size_t buffer_count) :
	GpuSwapChain(device, queue, info, format, buffer_count)
{
#ifdef _WIN32
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice);

	vk::Win32SurfaceCreateInfoKHR surfaceInfo = {};

	surfaceInfo
		.setPNext(nullptr)
		.setFlags(vk::Win32SurfaceCreateFlagsKHR(0))
		.setHinstance(GetModuleHandle(nullptr))
		.setHwnd(static_cast<HWND>(info.handle));

	mSurface = vkDevice->mInstance.createWin32SurfaceKHR(surfaceInfo);
#endif
}

CodeRed::VulkanSwapChain::~VulkanSwapChain()
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice);

	vkDevice->device().destroySwapchainKHR(mSwapChain);
	vkDevice->mInstance.destroySurfaceKHR(mSurface);
}

void CodeRed::VulkanSwapChain::initializeSwapChain()
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice);
	const auto capabilities = vkDevice->mPhysicalDevice.getSurfaceCapabilitiesKHR(mSurface);

	vk::SwapchainCreateInfoKHR chainInfo = {};

	chainInfo
		.setPNext(nullptr)
		.setFlags(vk::SwapchainCreateFlagsKHR(0))
		.setSurface(mSurface)
		.setMinImageCount(static_cast<uint32_t>(mBuffers.size()))
		.setImageFormat(enumConvert(mPixelFormat))
		.setImageExtent(vk::Extent2D(
			static_cast<uint32_t>(mInfo.width),
			static_cast<uint32_t>(mInfo.height)))
		.setPreTransform(capabilities.currentTransform)
		.setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
		.setImageArrayLayers(1)
		.setPresentMode(vk::PresentModeKHR::eFifo)
		.setOldSwapchain(nullptr)
		.setClipped(false)
		.setImageColorSpace(vk::ColorSpaceKHR::eSrgbNonlinear)
		.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
		.setImageSharingMode(vk::SharingMode::eExclusive)
		.setQueueFamilyIndexCount(0)
		.setPQueueFamilyIndices(nullptr);

	if (capabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity)
		chainInfo.preTransform = vk::SurfaceTransformFlagBitsKHR::eIdentity;
	
	const vk::CompositeAlphaFlagBitsKHR compositeAlphaFlags[] = {
			vk::CompositeAlphaFlagBitsKHR::eOpaque,
			vk::CompositeAlphaFlagBitsKHR::eInherit,
			vk::CompositeAlphaFlagBitsKHR::ePostMultiplied,
			vk::CompositeAlphaFlagBitsKHR::ePreMultiplied
	};

	for (size_t index = 0; index < sizeof(compositeAlphaFlags) / sizeof(vk::CompositeAlphaFlagBitsKHR); index++) {
		if (capabilities.supportedCompositeAlpha & compositeAlphaFlags[index]) {
			chainInfo.compositeAlpha = compositeAlphaFlags[index];
			break;
		}
	}

	mSwapChain = vkDevice->device().createSwapchainKHR(chainInfo);

	auto swapChainImages = vkDevice->device().getSwapchainImagesKHR(mSwapChain);
	
	for (size_t index = 0; index < mBuffers.size(); index++) {
		mBuffers[index] = std::make_shared<VulkanTexture>(
			mDevice,
			ResourceInfo::Texture2D(
				mInfo.width,
				mInfo.height,
				mPixelFormat,
				ResourceUsage::RenderTarget,
				ResourceLayout::Present),
			swapChainImages[index]);
	}
}

#endif
