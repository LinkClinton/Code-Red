#pragma once

#include "EffectPass.hpp"

namespace CodeRed {

	class PhysicallyBasedEffectPass final : public EffectPass {
	public:
		explicit PhysicallyBasedEffectPass(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuRenderPass>& renderPass,
			const size_t maxInstance = 16);

		~PhysicallyBasedEffectPass() = default;

		void setMaterial(const size_t index, const PhysicallyBasedMaterial& material);

		void setMaterials(const std::vector<PhysicallyBasedMaterial>& materials);

		void updateToGpu(
			const std::shared_ptr<GpuCommandAllocator>& allocator,
			const std::shared_ptr<GpuCommandQueue>& queue) override;

		auto material(const size_t index) const -> PhysicallyBasedMaterial { return mMaterials[index]; }
	private:
		std::shared_ptr<GpuBuffer> mMaterialsBuffer;

		std::vector<PhysicallyBasedMaterial> mMaterials;
	};
	
}