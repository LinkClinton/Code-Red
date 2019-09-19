#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Interface/GpuSystemInfo.hpp>
#else
#include "../Interface/GpuSystemInfo.hpp"
#endif

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
