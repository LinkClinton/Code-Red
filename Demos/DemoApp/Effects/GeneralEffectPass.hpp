#pragma once

#include "EffectPass.hpp"

namespace CodeRed {

	class GeneralEffectPass final : public EffectPass {
	public:
		explicit GeneralEffectPass(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuRenderPass>& renderPass,
			const size_t maxInstance = 16);

		~GeneralEffectPass() = default;
		
		void setMaterial(const size_t index, const Material& material);

		void setMaterials(const std::vector<Material>& materials);

		void updateToGpu(
			const std::shared_ptr<GpuCommandAllocator>& allocator, 
			const std::shared_ptr<GpuCommandQueue>& queue) override;
		
		auto material(const size_t index) const -> Material { return mMaterials[index]; }
	private:
		std::shared_ptr<GpuBuffer> mMaterialsBuffer;
		
		std::vector<Material> mMaterials;		
	};
	
}