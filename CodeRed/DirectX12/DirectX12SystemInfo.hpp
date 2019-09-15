#pragma once

#ifdef __ENABLE__DIRECTX12__

#include "../Interface/GpuSystemInfo.hpp"

namespace CodeRed {

	class DirectX12SystemInfo final : public GpuSystemInfo {
	public:
		DirectX12SystemInfo() = default;

		~DirectX12SystemInfo() = default;

		auto selectDisplayAdapter() const -> std::vector<std::shared_ptr<GpuDisplayAdapter>> override;
	};
	
}

#endif
