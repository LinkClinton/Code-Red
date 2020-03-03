#pragma once

#include "DirectX12Utility.hpp"
#include "../Interface/GpuRenderPass.hpp"

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12RenderPass final : public GpuRenderPass {
	public:
		explicit DirectX12RenderPass(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::vector<Attachment>& colors,
			const std::optional<Attachment>& depth = std::nullopt);
		
		~DirectX12RenderPass() = default;
	};
	
}

#endif