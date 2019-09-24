#pragma once

#include "../Interface/GpuDisplayAdapter.hpp"
#include "VulkanUtility.hpp"

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class VulkanDisplayAdapter final : public GpuDisplayAdapter {
	public:
		explicit VulkanDisplayAdapter(
			const std::string& name,
			const size_t device_id,
			const size_t vendor_id);

		~VulkanDisplayAdapter() = default;
	};
	
}

#endif