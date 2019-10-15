#include "PhysicallyBasedEffectPass.hpp"

#include "../Resources/ResourceHelper.hpp"
#include "../Shaders/ShaderResources.hpp"
#include "../Shaders/ShaderCompiler.hpp"

CodeRed::PhysicallyBasedEffectPass::PhysicallyBasedEffectPass(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuRenderPass>& renderPass, 
	const size_t maxInstance) : EffectPass(device, renderPass, maxInstance),
	mMaterials(maxInstance)
{
	if (mDevice->apiVersion() == APIVersion::DirectX12) {
#ifdef __ENABLE__DIRECTX12__
		mEffectVertexShaderCode = ShaderCompiler::compileToCso(
			ShaderType::Vertex, DxPhysicallyBasedEffectPassVertexShaderCode, "main");
		mEffectPixelShaderCode = ShaderCompiler::compileToCso(
			ShaderType::Pixel, DxPhysicallyBasedEffectPassPixelShaderCode, "main");
#endif

	}
	else if (mDevice->apiVersion() == APIVersion::Vulkan) {
#ifdef __ENABLE__VULKAN__
		mEffectVertexShaderCode = ShaderCompiler::compileToSpv(
			ShaderType::Vertex, VkPhysicallyBasedEffectPassVertexShaderCode);

		mEffectPixelShaderCode = ShaderCompiler::compileToSpv(
			ShaderType::Pixel, VkPhysicallyBasedEffectPassPixelShaderCode
		);
#endif
	}
	
	auto pipelineFactory = mPipelineInfo->pipelineFactory();

	CODE_RED_DEBUG_THROW_IF(
		mEffectVertexShaderCode.empty() ||
		mEffectPixelShaderCode.empty(),
		FailedException(DebugType::Create,
			{ "effect pass" },
			{ "compile effect pass shader failed." })
	);

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

	mMaterialsBuffer = mDevice->createBuffer(
		ResourceInfo::GroupBuffer(
			sizeof(PhysicallyBasedMaterial),
			mMaterials.size(),
			MemoryHeap::Upload
		)
	);

	mDescriptorHeap = mDevice->createDescriptorHeap(
		mPipelineInfo->resourceLayout()
	);

	mDescriptorHeap->bindBuffer(mLightsBuffer, 0);
	mDescriptorHeap->bindBuffer(mMaterialsBuffer, 1);
	mDescriptorHeap->bindBuffer(mTransformsBuffer, 2);
}

void CodeRed::PhysicallyBasedEffectPass::setMaterial(const size_t index, const PhysicallyBasedMaterial& material)
{
	mMaterials[index] = material;
}

void CodeRed::PhysicallyBasedEffectPass::setMaterials(const std::vector<PhysicallyBasedMaterial>& materials)
{
	CODE_RED_DEBUG_THROW_IF(
		materials.size() > mMaterials.size(),
		InvalidException<size_t>({ "size of materials" })
	);

	std::copy(materials.begin(), materials.end(), mMaterials.begin());
}

void CodeRed::PhysicallyBasedEffectPass::updateToGpu(
	const std::shared_ptr<GpuCommandAllocator>& allocator,
	const std::shared_ptr<GpuCommandQueue>& queue)
{
	ResourceHelper::updateBuffer(mLightsBuffer, mLights.data(), sizeof(Light) * mLights.size());
	ResourceHelper::updateBuffer(mMaterialsBuffer, mMaterials.data(), sizeof(PhysicallyBasedMaterial) * mMaterials.size());
	ResourceHelper::updateBuffer(mTransformsBuffer, mTransforms.data(), sizeof(Transform3D) * mTransforms.size());
}
