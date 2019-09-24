#pragma once

#include "../Interface/GpuSystemInfo.hpp"
#include "VulkanUtility.hpp"

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class VulkanSystemInfo final : public GpuSystemInfo {
	public:
		VulkanSystemInfo() = default;

		~VulkanSystemInfo() = default;

		auto selectDisplayAdapter() const -> std::vector<std::shared_ptr<GpuDisplayAdapter>> override;
	};
	
}

#endif