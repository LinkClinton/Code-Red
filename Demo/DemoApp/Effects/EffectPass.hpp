#pragma once

#include <Core/CodeRedGraphics.hpp>

#include "../Pipelines/PipelineInfo.hpp"

#include "EffectLight.hpp"

#include <vector>

namespace CodeRed {

	class EffectPass {
	public:
		explicit EffectPass(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::optional<std::shared_ptr<GpuRasterizationState>> &rasterization = std::nullopt,
			const std::optional<std::shared_ptr<GpuDepthStencilState>> &depthStencil = std::nullopt);

		virtual ~EffectPass() = default;

		virtual void setDirectionalLight(const size_t index, const Light& light);

		virtual void setPointLight(const size_t index, const Light& light);

		virtual void setSpotLight(const size_t index, const Light& light);

		virtual void updateToGpu(
			const std::shared_ptr<GpuCommandAllocator>& allocator,
			const std::shared_ptr<GpuCommandQueue>& queue);

	private:
		std::shared_ptr<GpuLogicalDevice> mDevice;

		std::shared_ptr<PipelineInfo> mPipelineInfo;
		
		std::vector<Light> mDirectionalLights = std::vector<Light>(MAX_LIGHTS);
		std::vector<Light> mPointLights = std::vector<Light>(MAX_LIGHTS);
		std::vector<Light> mSpotLights = std::vector<Light>(MAX_LIGHTS);

		std::vector<Material> mMaterials = std::vector<Material>(MAX_MATERIALS);
	};
	
}