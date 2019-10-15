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
			const size_t maxInstance = 16);

		virtual ~EffectPass() = default;

		virtual void setLight(const LightType type, const size_t index, const Light& light);

		virtual void setLights(const LightType type, std::vector<Light>& lights);
		
		virtual void setTransform(const size_t index, const Transform3D& transform);

		virtual void setTransforms(const std::vector<Transform3D>& transforms);

		virtual void setAmbientLight(const glm::vec4& light);

		virtual void updateState(
			const std::optional<std::shared_ptr<GpuBlendState>>& blend,
			const std::optional<std::shared_ptr<GpuDepthStencilState>>& depthStencil,
			const std::optional<std::shared_ptr<GpuRasterizationState>>& rasterization);
		
		virtual void beginEffect(
			std::shared_ptr<GpuGraphicsCommandList>& commandList);

		virtual void endEffect();

		virtual void drawIndexed(
			const size_t indexCount,
			const size_t instanceCount,
			const size_t startIndexLocation = 0,
			const size_t baseVertexLocation = 0,
			const size_t startInstanceLocation = 0);

		virtual void updateToGpu(
			const std::shared_ptr<GpuCommandAllocator>& allocator,
			const std::shared_ptr<GpuCommandQueue>& queue) = 0;

		auto transform(const size_t index) const -> Transform3D;

		auto light(const LightType type, const size_t index) const -> Light;
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;
		std::shared_ptr<GpuGraphicsCommandList> mCommandList;

		std::shared_ptr<GpuDescriptorHeap> mDescriptorHeap;

		std::shared_ptr<GpuBuffer> mLightsBuffer;
		std::shared_ptr<GpuBuffer> mTransformsBuffer;
		
		std::shared_ptr<PipelineInfo> mPipelineInfo;

		std::vector<Light> mLights;
		std::vector<Transform3D> mTransforms;

		std::vector<Byte> mEffectVertexShaderCode;
		std::vector<Byte> mEffectPixelShaderCode;

		glm::vec4 mAmbientLight = glm::vec4(0);
	};

}