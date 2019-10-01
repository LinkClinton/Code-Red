#include "DirectX12PipelineState/DirectX12PipelineFactory.hpp"

#include "../Shared/Exception/FailedException.hpp"
#include "../Shared/DebugReport.hpp"

#include "DirectX12Resource/DirectX12Sampler.hpp"
#include "DirectX12Resource/DirectX12Texture.hpp"
#include "DirectX12Resource/DirectX12Buffer.hpp"

#include "DirectX12GraphicsCommandList.hpp"
#include "DirectX12GraphicsPipeline.hpp"
#include "DirectX12CommandAllocator.hpp"
#include "DirectX12DisplayAdapter.hpp"
#include "DirectX12ResourceLayout.hpp"
#include "DirectX12DescriptorHeap.hpp"
#include "DirectX12LogicalDevice.hpp"
#include "DirectX12CommandQueue.hpp"
#include "DirectX12FrameBuffer.hpp"
#include "DirectX12RenderPass.hpp"
#include "DirectX12SwapChain.hpp"
#include "DirectX12Fence.hpp"

#ifdef __ENABLE__DIRECTX12__

using namespace CodeRed::DirectX12;

CodeRed::DirectX12LogicalDevice::DirectX12LogicalDevice(const std::shared_ptr<GpuDisplayAdapter>& adapter)
	: GpuLogicalDevice(adapter)
{
#ifdef _DEBUG
	//get the debug layer and enable debug
	WRL::ComPtr<ID3D12Debug3> debugLayer;
	D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer));
	debugLayer->EnableDebugLayer();

	CODE_RED_DEBUG_LOG("enabled DirectX12 Debug Layer.");
#endif

	//get the adapter we used to create device
	const auto dxgiAdapter = static_cast<DirectX12DisplayAdapter*>(adapter.get());

	CODE_RED_DEBUG_LOG(DebugReport::make("create device with [0]", { mDisplayAdapter->name() }));
	
	//create Direct3D 12 Device.
	CODE_RED_THROW_IF_FAILED(
		D3D12CreateDevice(dxgiAdapter->adapter().Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&mDevice)),
		FailedException(DebugType::Create, { "ID3D12Device" })
	);
}

auto CodeRed::DirectX12LogicalDevice::createFence()
	-> std::shared_ptr<GpuFence>
{
	return std::static_pointer_cast<GpuFence>(
		std::make_shared<DirectX12Fence>(shared_from_this()));
}

auto CodeRed::DirectX12LogicalDevice::createFrameBuffer(
	const std::shared_ptr<GpuTexture>& render_target,
	const std::shared_ptr<GpuTexture>& depth_stencil)
	-> std::shared_ptr<GpuFrameBuffer>
{
	return std::static_pointer_cast<GpuFrameBuffer>(
		std::make_shared<DirectX12FrameBuffer>(
			shared_from_this(),
			render_target,
			depth_stencil));
}

auto CodeRed::DirectX12LogicalDevice::createGraphicsCommandList(
	const std::shared_ptr<GpuCommandAllocator>& allocator)
	-> std::shared_ptr<GpuGraphicsCommandList>
{
	return std::static_pointer_cast<GpuGraphicsCommandList>(
		std::make_shared<DirectX12GraphicsCommandList>(
			shared_from_this(),
			allocator));
}

auto CodeRed::DirectX12LogicalDevice::createCommandQueue()
	-> std::shared_ptr<GpuCommandQueue>
{
	return std::static_pointer_cast<GpuCommandQueue>(
		std::make_shared<DirectX12CommandQueue>(shared_from_this()));
}

auto CodeRed::DirectX12LogicalDevice::createCommandAllocator()
	-> std::shared_ptr<GpuCommandAllocator>
{
	return std::static_pointer_cast<GpuCommandAllocator>(
		std::make_shared<DirectX12CommandAllocator>(shared_from_this()));
}

auto CodeRed::DirectX12LogicalDevice::createGraphicsPipeline(
	const std::shared_ptr<GpuRenderPass>& render_pass,
	const std::shared_ptr<GpuResourceLayout>& resource_layout,
	const std::shared_ptr<GpuInputAssemblyState>& input_assembly_state,
	const std::shared_ptr<GpuShaderState>& vertex_shader_state,
	const std::shared_ptr<GpuShaderState>& pixel_shader_state,
	const std::shared_ptr<GpuDepthStencilState>& depth_stencil_state,
	const std::shared_ptr<GpuBlendState>& blend_state,
	const std::shared_ptr<GpuRasterizationState>& rasterization_state)
	-> std::shared_ptr<GpuGraphicsPipeline>
{
	return std::static_pointer_cast<GpuGraphicsPipeline>(
		std::make_shared<DirectX12GraphicsPipeline>(
			shared_from_this(),
			render_pass,
			resource_layout,
			input_assembly_state,
			vertex_shader_state,
			pixel_shader_state,
			depth_stencil_state,
			blend_state,
			rasterization_state
			));
}

auto CodeRed::DirectX12LogicalDevice::createResourceLayout(
	const std::vector<ResourceLayoutElement>& elements,
	const std::vector<SamplerLayoutElement>& samplers)
	-> std::shared_ptr<GpuResourceLayout>
{
	return std::static_pointer_cast<GpuResourceLayout>(
		std::make_shared<DirectX12ResourceLayout>(
			shared_from_this(),
			elements,
			samplers));
}

auto CodeRed::DirectX12LogicalDevice::createDescriptorHeap(
	const std::shared_ptr<GpuResourceLayout>& resource_layout)
	-> std::shared_ptr<GpuDescriptorHeap>
{
	return std::static_pointer_cast<GpuDescriptorHeap>(
		std::make_shared<DirectX12DescriptorHeap>(
			shared_from_this(),
			resource_layout));
}

auto CodeRed::DirectX12LogicalDevice::createRenderPass(
	const std::optional<Attachment>& color,
	const std::optional<Attachment>& depth)
	-> std::shared_ptr<GpuRenderPass>
{
	return std::static_pointer_cast<GpuRenderPass>(
		std::make_shared<DirectX12RenderPass>(
			shared_from_this(),
			color, depth));
}

auto CodeRed::DirectX12LogicalDevice::createSampler(const SamplerInfo& info)
	-> std::shared_ptr<GpuSampler>
{
	return std::static_pointer_cast<GpuSampler>(
		std::make_shared<DirectX12Sampler>(
			shared_from_this(),
			info));
}

auto CodeRed::DirectX12LogicalDevice::createSwapChain(
	const std::shared_ptr<GpuCommandQueue> &queue,
	const WindowInfo& info,
	const PixelFormat& format,
	const size_t buffer_count)
	-> std::shared_ptr<GpuSwapChain>
{
	return std::static_pointer_cast<GpuSwapChain>(
		std::make_shared<DirectX12SwapChain>(
			shared_from_this(),
			queue,
			info,
			format,
			buffer_count));
}

auto CodeRed::DirectX12LogicalDevice::createBuffer(const ResourceInfo& info)
	-> std::shared_ptr<GpuBuffer>
{
	return std::static_pointer_cast<GpuBuffer>(
		std::make_shared<DirectX12Buffer>(
			shared_from_this(),
			info));
}

auto CodeRed::DirectX12LogicalDevice::createTexture(const ResourceInfo& info)
	-> std::shared_ptr<GpuTexture>
{
	return std::static_pointer_cast<GpuTexture>(
		std::make_shared<DirectX12Texture>(
			shared_from_this(),
			info));
}

auto CodeRed::DirectX12LogicalDevice::createPipelineFactory()
	-> std::shared_ptr<GpuPipelineFactory>
{
	return std::static_pointer_cast<GpuPipelineFactory>(
		std::make_shared<DirectX12PipelineFactory>(shared_from_this()));
}

#endif

