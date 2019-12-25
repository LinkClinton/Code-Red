#include "../Shared/Exception/FailedException.hpp"
#include "../Shared/Exception/ZeroException.hpp"

#include "VulkanResource/VulkanTexture.hpp"

#include "VulkanLogicalDevice.hpp"
#include "VulkanCommandQueue.hpp"
#include "VulkanSwapChain.hpp"

#ifdef __ENABLE__VULKAN__

using namespace CodeRed::Vulkan;

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
	
	auto formats = vkDevice->mPhysicalDevice.getSurfaceFormatsKHR(mSurface);

	auto supportFormat = false;
	
	for (const auto vkFormat : formats) {
		if (enumConvert(mPixelFormat) == vkFormat.format &&
			vkFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {

			supportFormat = true;
			break;
		}
	}
	
	CODE_RED_DEBUG_WARNING_IF(
		supportFormat == false,
		"the format of swapchain is not supported."
	);

	CODE_RED_DEBUG_WARNING_IF(
		vkDevice->mPhysicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(
			vkDevice->mQueueFamilyIndex), mSurface) == false,
		"the queue family is not supported this surface."
	);
	
	initializeSwapChain();
}

CodeRed::VulkanSwapChain::~VulkanSwapChain()
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice);

	vkDevice->device().destroySwapchainKHR(mSwapChain);
	vkDevice->device().destroySemaphore(mSemaphore);
	vkDevice->mInstance.destroySurfaceKHR(mSurface);
}

void CodeRed::VulkanSwapChain::resize(const size_t width, const size_t height)
{
	if (width == mInfo.width && height == mInfo.height) return;

	CODE_RED_DEBUG_THROW_IF(
		width == 0 || height == 0,
		ZeroException<size_t>({ "width or height" })
	);
	
	mInfo.width = width;
	mInfo.height = height;

	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();

	//destroy the old resources and create the new resource
	for (auto& buffer : mBuffers) buffer.reset();

	vkDevice.destroySwapchainKHR(mSwapChain);
	vkDevice.destroySemaphore(mSemaphore);

	initializeSwapChain();
}

void CodeRed::VulkanSwapChain::present()
{
	vk::PresentInfoKHR info = {};

	info
		.setPNext(nullptr)
		.setSwapchainCount(1)
		.setPSwapchains(&mSwapChain)
		.setPImageIndices(&mCurrentBufferIndex)
		.setWaitSemaphoreCount(1)
		.setPWaitSemaphores(&mSemaphore)
		.setPResults(nullptr);

	const auto vkQueue = std::static_pointer_cast<VulkanCommandQueue>(mQueue)->queue();

	CODE_RED_THROW_IF(
		vkQueue.presentKHR(info) != vk::Result::eSuccess,
		Exception("present failed.")
	);

	updateCurrentFrameIndex();
}

auto CodeRed::VulkanSwapChain::currentBufferIndex() const -> size_t
{
	return static_cast<size_t>(mCurrentBufferIndex);
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
				1,
				ResourceUsage::RenderTarget),
			swapChainImages[index]);
	}

	vk::SemaphoreCreateInfo semaphoreInfo = {};

	semaphoreInfo
		.setPNext(nullptr)
		.setFlags(vk::SemaphoreCreateFlags(0));

	mSemaphore = vkDevice->device().createSemaphore(semaphoreInfo);

	updateCurrentFrameIndex();
}

void CodeRed::VulkanSwapChain::updateCurrentFrameIndex()
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice);
	const auto nextImage = vkDevice->device().acquireNextImageKHR(mSwapChain, UINT64_MAX, mSemaphore, nullptr);

	CODE_RED_THROW_IF(
		nextImage.result != vk::Result::eSuccess &&
		nextImage.result != vk::Result::eSuboptimalKHR,
		FailedException(DebugType::Get, { "current frame index", "swap chain" })
	);

	mCurrentBufferIndex = nextImage.value;
}

#endif
