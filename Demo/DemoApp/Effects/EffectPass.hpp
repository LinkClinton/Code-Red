#pragma once

#include "../Pipelines/PipelineInfo.hpp"

#include "EffectProperties.hpp"

#include <vector>

namespace CodeRed {

	class EffectPass {
	public:
		explicit EffectPass(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuRenderPass>& renderPass,
			const std::optional<std::shared_ptr<GpuBlendState>>& blend = std::nullopt,
			const std::optional<std::shared_ptr<GpuRasterizationState>>& rasterization = std::nullopt,
			const std::optional<std::shared_ptr<GpuDepthStencilState>>& depthStencil = std::nullopt);

		virtual ~EffectPass() = default;

		virtual void setLights(const LightType type, const size_t index, const Light& light);

		virtual void setMaterial(const size_t index, const Material& material);

		virtual void setTransform(const size_t index, const Transform3D& transform);

		virtual void setAmbientLight(const glm::vec4& light);
		
		virtual void updateToGpu(
			const std::shared_ptr<GpuCommandAllocator>& allocator,
			const std::shared_ptr<GpuCommandQueue>& queue);

		virtual void beginEffect(
			std::shared_ptr<GpuGraphicsCommandList>& commandList);

		virtual void endEffect();

		virtual void drawIndexed(
			const size_t indexCount,
			const size_t materialIndex,
			const size_t transformIndex,
			const size_t startIndexLocation = 0,
			const size_t baseVertexLocation = 0);

		auto transform(const size_t index) const -> Transform3D;

		auto material(const size_t index) const -> Material;

		auto light(const LightType type, const size_t index) const->Light;
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;
		std::shared_ptr<GpuGraphicsCommandList> mCommandList;

		std::shared_ptr<GpuDescriptorHeap> mDescriptorHeap;

		std::shared_ptr<GpuBuffer> mLightsBuffer;
		std::shared_ptr<GpuBuffer> mMaterialsBuffer;
		std::shared_ptr<GpuBuffer> mTransformsBuffer;
		
		std::shared_ptr<PipelineInfo> mPipelineInfo;

		std::vector<Light> mLights = std::vector<Light>(MAX_ALL_LIGHTS);
		std::vector<Material> mMaterials = std::vector<Material>(MAX_MATERIALS);
		std::vector<Transform3D> mTransforms = std::vector<Transform3D>(MAX_TRANSFORMS);

		std::vector<Byte> mEffectVertexShaderCode;
		std::vector<Byte> mEffectPixelShaderCode;

		glm::vec4 mAmbientLight = glm::vec4(0);
	};

}