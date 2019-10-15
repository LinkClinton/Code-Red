#include "EffectPass.hpp"

#include "../Resources/ResourceHelper.hpp"

CodeRed::EffectPass::EffectPass(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuRenderPass>& renderPass,
	const size_t maxInstance) :
	mDevice(device), mLights(MAX_ALL_LIGHTS), mTransforms(maxInstance)
{
	CODE_RED_DEBUG_THROW_IF(
		device == nullptr,
		InvalidException<GpuLogicalDevice>({ "device" })
	);

	CODE_RED_DEBUG_THROW_IF(
		renderPass == nullptr,
		InvalidException<GpuRenderPass>({ "render pass" })
	);

	mSampler = mDevice->createSampler(
		SamplerInfo(16)
	);
	
	mPipelineInfo = std::make_shared<PipelineInfo>(mDevice);

	auto pipelineFactory = mPipelineInfo->pipelineFactory();
	
	mPipelineInfo->setRenderPass(renderPass);
	mPipelineInfo->setInputAssemblyState(
		pipelineFactory->createInputAssemblyState(
			{
				InputLayoutElement("POSITION", PixelFormat::RedGreenBlue32BitFloat),
				InputLayoutElement("NORMAL", PixelFormat::RedGreenBlue32BitFloat),
				InputLayoutElement("TEXCOORD", PixelFormat::RedGreen32BitFloat)
			},
			PrimitiveTopology::TriangleList
		)
	);

	mPipelineInfo->setResourceLayout(
		mDevice->createResourceLayout(
			{
				ResourceLayoutElement(ResourceType::Buffer, 0, 0),
				ResourceLayoutElement(ResourceType::GroupBuffer, 1, 0),
				ResourceLayoutElement(ResourceType::GroupBuffer, 2, 0),
				ResourceLayoutElement(ResourceType::Texture, 3, 0),
				ResourceLayoutElement(ResourceType::Texture, 4, 0),
				ResourceLayoutElement(ResourceType::Texture, 5, 0),
				ResourceLayoutElement(ResourceType::Texture, 6, 0),
				ResourceLayoutElement(ResourceType::Texture, 7, 0)
			},
			{
				SamplerLayoutElement(mSampler, 8, 0)
			},
			Constant32Bits(5, 9, 0)
		)
	);
	
	mLightsBuffer = mDevice->createBuffer(
		ResourceInfo::ConstantBuffer(
			sizeof(Light) * MAX_ALL_LIGHTS,
			MemoryHeap::Upload
		)
	);

	mTransformsBuffer = mDevice->createBuffer(
		ResourceInfo::GroupBuffer(
			sizeof(Transform3D),
			mTransforms.size(),
			MemoryHeap::Upload
		)
	);
}

void CodeRed::EffectPass::setLight(const LightType type, const size_t index, const Light& light)
{
	mLights[static_cast<size_t>(type) * MAX_LIGHTS_PER_TYPE + index] = light;
}

void CodeRed::EffectPass::setLights(const LightType type, std::vector<Light>& lights)
{
	CODE_RED_DEBUG_THROW_IF(
		lights.size() > 16,
		InvalidException<size_t>({ "size of lights" })
	);
	
	std::copy(lights.begin(), lights.end(), mLights.begin() + static_cast<size_t>(type) * MAX_LIGHTS_PER_TYPE);
}

void CodeRed::EffectPass::setTransform(const size_t index, const Transform3D& transform)
{
	mTransforms[index] = transform;
}

void CodeRed::EffectPass::setTransforms(const std::vector<Transform3D>& transforms)
{
	CODE_RED_DEBUG_THROW_IF(
		transforms.size() > mTransforms.size(),
		InvalidException<size_t>({ "size of transforms" })
	);

	std::copy(transforms.begin(), transforms.end(), mTransforms.begin());
}

void CodeRed::EffectPass::setAmbientLight(const glm::vec4& light)
{
	mAmbientLight = light;
}

void CodeRed::EffectPass::updateState(
	const std::optional<std::shared_ptr<GpuBlendState>>& blend,
	const std::optional<std::shared_ptr<GpuDepthStencilState>>& depthStencil,
	const std::optional<std::shared_ptr<GpuRasterizationState>>& rasterization)
{
	CODE_RED_TRY_EXECUTE(
		blend.has_value(),
		mPipelineInfo->setBlendState(blend.value())
	);

	CODE_RED_TRY_EXECUTE(
		depthStencil.has_value(),
		mPipelineInfo->setDepthStencilState(depthStencil.value())
	);

	CODE_RED_TRY_EXECUTE(
		rasterization.has_value(),
		mPipelineInfo->setRasterizationState(rasterization.value())
	);

	CODE_RED_TRY_EXECUTE(
		blend.has_value() || depthStencil.has_value() || rasterization.has_value(),
		mPipelineInfo->updateState()
	);
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
	const size_t instanceCount,
	const size_t startIndexLocation, 
	const size_t baseVertexLocation, 
	const size_t startInstanceLocation)
{
	CODE_RED_DEBUG_THROW_IF(
		mCommandList == nullptr,
		Exception(DebugReport::makeError("please begin effect before drawing."))
	);

	mCommandList->setConstant32Bits(
		{
			mAmbientLight.r,
			mAmbientLight.g,
			mAmbientLight.b,
			mAmbientLight.a,
			static_cast<UInt32>(MaterialType::Buffer)
		});

	mCommandList->drawIndexed(
		indexCount,
		instanceCount,
		startIndexLocation,
		baseVertexLocation,
		startInstanceLocation);
}

void CodeRed::EffectPass::drawIndexedWithTextureMaterial(
	const size_t indexCount, 
	const size_t instanceCount,
	const size_t startIndexLocation, 
	const size_t baseVertexLocation, 
	const size_t startInstanceLocation)
{
	CODE_RED_DEBUG_THROW_IF(
		mCommandList == nullptr,
		Exception(DebugReport::makeError("please begin effect before drawing."))
	);

	mCommandList->setConstant32Bits(
		{
			mAmbientLight.r,
			mAmbientLight.g,
			mAmbientLight.b,
			mAmbientLight.a,
			static_cast<UInt32>(MaterialType::Texture)
		});

	mCommandList->drawIndexed(
		indexCount,
		instanceCount,
		startIndexLocation,
		baseVertexLocation,
		startInstanceLocation);
}

auto CodeRed::EffectPass::transform(const size_t index) const -> Transform3D
{
	return mTransforms[index];
}

auto CodeRed::EffectPass::light(const LightType type, const size_t index) const -> Light
{
	return mLights[static_cast<size_t>(type)* MAX_LIGHTS_PER_TYPE + index];
}
