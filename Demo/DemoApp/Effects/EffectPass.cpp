#include "EffectPass.hpp"

#include "../Resources/ResourceHelper.hpp"
#include "../Shaders/ShaderResources.hpp"
#include "../Shaders/ShaderCompiler.hpp"

CodeRed::EffectPass::EffectPass(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuRenderPass>& renderPass,
	const std::optional<std::shared_ptr<GpuBlendState>>& blend,
	const std::optional<std::shared_ptr<GpuRasterizationState>>& rasterization,
	const std::optional<std::shared_ptr<GpuDepthStencilState>>& depthStencil) :
	mDevice(device)
{
	CODE_RED_DEBUG_THROW_IF(
		device == nullptr,
		InvalidException<GpuLogicalDevice>({ "device" })
	);

	CODE_RED_DEBUG_THROW_IF(
		renderPass == nullptr,
		InvalidException<GpuRenderPass>({ "render pass" })
	);
	
	mPipelineInfo = std::make_shared<PipelineInfo>(mDevice);
	
	CODE_RED_TRY_EXECUTE(
		blend.has_value(),
		mPipelineInfo->setBlendState(blend.value())
	);
	
	CODE_RED_TRY_EXECUTE(
		rasterization.has_value(),
		mPipelineInfo->setRasterizationState(rasterization.value())
	);

	CODE_RED_TRY_EXECUTE(
		depthStencil.has_value(),
		mPipelineInfo->setDepthStencilState(depthStencil.value())
	);

	auto pipelineFactory = mPipelineInfo->pipelineFactory();
	
	mPipelineInfo->setRenderPass(renderPass);
	mPipelineInfo->setInputAssemblyState(
		pipelineFactory->createInputAssemblyState(
			{
				InputLayoutElement("POSITION", PixelFormat::RedGreenBlue32BitFloat),
				InputLayoutElement("NORMAL", PixelFormat::RedGreenBlue32BitFloat)
			},
			PrimitiveTopology::TriangleList
		)
	);

	mPipelineInfo->setResourceLayout(
		mDevice->createResourceLayout(
			{
				ResourceLayoutElement(ResourceType::Buffer, 0, 0),
				ResourceLayoutElement(ResourceType::Buffer, 1, 0),
				ResourceLayoutElement(ResourceType::Buffer, 2, 0)
			},
			{},
			Constant32Bits(6, 3, 0)
		)
	);

	if (mDevice->apiVersion() == APIVersion::DirectX12) {
		
		mEffectVertexShaderCode = ShaderCompiler::compileToCso(
			ShaderType::Vertex, DxEffectPassVertexShaderCode, "main");
		mEffectPixelShaderCode = ShaderCompiler::compileToCso(
			ShaderType::Pixel, DxEffectPassPixelShaderCode, "main");
		
	}else if (mDevice->apiVersion() == APIVersion::Vulkan) {
		
	}

	mPipelineInfo->setVertexShaderState(
		pipelineFactory->createShaderState(
			ShaderType::Vertex,
			mEffectVertexShaderCode,
			"main"
		)
	);

	mPipelineInfo->setPixelShaderState(
		pipelineFactory->createShaderState(
			ShaderType::Pixel,
			mEffectPixelShaderCode,
			"main"
		)
	);
	
	mPipelineInfo->updateState();

	mLightsBuffer = mDevice->createBuffer(
		ResourceInfo::ConstantBuffer(
			sizeof(Light) * MAX_ALL_LIGHTS,
			MemoryHeap::Default
		)
	);

	mMaterialsBuffer = mDevice->createBuffer(
		ResourceInfo::ConstantBuffer(
			sizeof(Material) * MAX_MATERIALS,
			MemoryHeap::Default
		)
	);

	mTransformsBuffer = mDevice->createBuffer(
		ResourceInfo::ConstantBuffer(
			sizeof(Transform3D) * MAX_TRANSFORMS,
			MemoryHeap::Default
		)
	);

	mDescriptorHeap = mDevice->createDescriptorHeap(
		mPipelineInfo->resourceLayout()
	);

	mDescriptorHeap->bindBuffer(mLightsBuffer, 0);
	mDescriptorHeap->bindBuffer(mMaterialsBuffer, 1);
	mDescriptorHeap->bindBuffer(mTransformsBuffer, 2);
}

void CodeRed::EffectPass::setLights(const LightType type, const size_t index, const Light& light)
{
	mLights[static_cast<size_t>(type) * MAX_LIGHTS_PER_TYPE + index] = light;
}

void CodeRed::EffectPass::setMaterial(const size_t index, const Material& material)
{
	mMaterials[index] = material;
}

void CodeRed::EffectPass::setTransform(const size_t index, const Transform3D& transform)
{
	mTransforms[index] = transform;
}

void CodeRed::EffectPass::setAmbientLight(const glm::vec4& light)
{
	mAmbientLight = light;
}

void CodeRed::EffectPass::updateToGpu(
	const std::shared_ptr<GpuCommandAllocator>& allocator,
	const std::shared_ptr<GpuCommandQueue>& queue)
{
	ResourceHelper::updateBuffer(mDevice, allocator, queue, mLightsBuffer, mLights.data());
	ResourceHelper::updateBuffer(mDevice, allocator, queue, mMaterialsBuffer, mMaterials.data());
	ResourceHelper::updateBuffer(mDevice, allocator, queue, mTransformsBuffer, mTransforms.data());
}

void CodeRed::EffectPass::beginEffect(std::shared_ptr<GpuGraphicsCommandList>& commandList)
{
	mCommandList = commandList;
	
	mCommandList->setGraphicsPipeline(mPipelineInfo->graphicsPipeline());
	mCommandList->setResourceLayout(mPipelineInfo->resourceLayout());
	mCommandList->setDescriptorHeap(mDescriptorHeap);
}

void CodeRed::EffectPass::endEffect()
{
	mCommandList = nullptr;
}

void CodeRed::EffectPass::drawIndexed(
	const size_t indexCount, 
	const size_t materialIndex,
	const size_t transformIndex,
	const size_t startIndexLocation,
	const size_t baseVertexLocation)
{
	CODE_RED_DEBUG_THROW_IF(
		mCommandList == nullptr,
		Exception(DebugReport::makeError("please begin effect before drawing."))
	);

	mCommandList->setConstant32Bits(
		{
			static_cast<UInt32>(materialIndex),
			static_cast<UInt32>(transformIndex),
			mAmbientLight.r,
			mAmbientLight.g,
			mAmbientLight.b,
			mAmbientLight.a
		});

	mCommandList->drawIndexed(
		indexCount, 
		1, 
		startIndexLocation, 
		baseVertexLocation,
		0);
}

auto CodeRed::EffectPass::transform(const size_t index) const -> Transform3D
{
	return mTransforms[index];
}

auto CodeRed::EffectPass::material(const size_t index) const -> Material
{
	return mMaterials[index];
}

auto CodeRed::EffectPass::light(const LightType type, const size_t index) const -> Light
{
	return mLights[static_cast<size_t>(type)* MAX_LIGHTS_PER_TYPE + index];
}
