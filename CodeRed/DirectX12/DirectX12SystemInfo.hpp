#pragma once

#include "../Interface/GpuSystemInfo.hpp"

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12SystemInfo final : public GpuSystemInfo {
	public:
		DirectX12SystemInfo() = default;

		~DirectX12SystemInfo() = default;

		auto selectDisplayAdapter() const -> std::vector<std::shared_ptr<GpuDisplayAdapter>> override;
	};
	
}

#endif
