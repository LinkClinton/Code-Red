#pragma once

#ifdef __ENABLE__DIRECTX12__

#include "../Interface/GpuDisplayAdapter.hpp"

namespace CodeRed {

	class DirectX12DisplayAdapter final : public GpuDisplayAdapter {
	public:
		explicit DirectX12DisplayAdapter(const std::string& name);

		~DirectX12DisplayAdapter() = default;
	};
	
}

#endif
