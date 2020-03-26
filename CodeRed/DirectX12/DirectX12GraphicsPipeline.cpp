#include "../Shared/Exception/FailedException.hpp"

#include "DirectX12PipelineState/DirectX12InputAssemblyState.hpp"
#include "DirectX12PipelineState/DirectX12RasterizationState.hpp"
#include "DirectX12PipelineState/DirectX12DepthStencilState.hpp"
#include "DirectX12PipelineState/DirectX12ShaderState.hpp"
#include "DirectX12PipelineState/DirectX12BlendState.hpp"

#include "DirectX12GraphicsPipeline.hpp"
#include "DirectX12ResourceLayout.hpp"
#include "DirectX12LogicalDevice.hpp"

#ifdef __ENABLE__DIRECTX12__

using namespace CodeRed::DirectX12;

CodeRed::DirectX12GraphicsPipeline::DirectX12GraphicsPipeline(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuRenderPass>& render_pass,
	const std::shared_ptr<GpuResourceLayout>& resource_layout,
	const std::shared_ptr<GpuInputAssemblyState>& input_assembly_state,
	const std::shared_ptr<GpuShaderState>& vertex_shader_state,
	const std::shared_ptr<GpuShaderState>& pixel_shader_state,
	const std::shared_ptr<GpuDepthStencilState>& depth_stencil_state,
	const std::shared_ptr<GpuBlendState>& blend_state,
	const std::shared_ptr<GpuRasterizationState>& rasterization_state) :
	GpuGraphicsPipeline(
		device,
		render_pass,
		resource_layout,
		input_assembly_state,
		vertex_shader_state,
		pixel_shader_state,
		depth_stencil_state,
		blend_state,
		rasterization_state
	)
{
	const auto dxDevice = static_cast<DirectX12LogicalDevice*>(mDevice.get())->device();
	
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
	
	desc.PrimitiveTopologyType = enumConvert1(mInputAssemblyState->primitiveTopology());
	desc.pRootSignature = static_cast<DirectX12ResourceLayout*>(mResourceLayout.get())->rootSignature().Get();
	desc.InputLayout = static_cast<DirectX12InputAssemblyState*>(mInputAssemblyState.get())->layout();
	desc.VS = static_cast<DirectX12ShaderState*>(mVertexShaderState.get())->shader();
	desc.PS = static_cast<DirectX12ShaderState*>(mPixelShaderState.get())->shader();
	desc.DepthStencilState = static_cast<DirectX12DepthStencilState*>(mDepthStencilState.get())->state();
	desc.BlendState = static_cast<DirectX12BlendState*>(mBlendState.get())->state();
	desc.RasterizerState = static_cast<DirectX12RasterizationState*>(mRasterizationState.get())->state();
	desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.DSVFormat = enumConvert(mRenderPass->depth().has_value() ? mRenderPass->depth()->Format : PixelFormat::Unknown);
	desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	desc.NodeMask = 0;
	desc.NumRenderTargets = static_cast<UINT>(mRenderPass->size());
	desc.SampleDesc.Count = static_cast<UINT>(mRenderPass->maxSample());
	desc.SampleDesc.Quality = 0;
	desc.SampleMask = UINT_MAX;

	for (size_t index = 0; index < mRenderPass->size(); index++) 
		desc.RTVFormats[index] = enumConvert(mRenderPass->color(index)->Format);
	
	CODE_RED_THROW_IF_FAILED(
		dxDevice->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&mGraphicsPipeline)),
		FailedException(DebugType::Create, { "ID3D12Pipeline of Graphics" })
	);
}

#endif