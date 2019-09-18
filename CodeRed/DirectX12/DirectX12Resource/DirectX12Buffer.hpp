#pragma once

#include <Interface/GpuResource/GpuBuffer.hpp>
#include <DirectX12/DirectX12Utility.hpp>

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12Buffer final : public GpuBuffer {
	public:
		explicit DirectX12Buffer(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const ResourceInfo info);

		~DirectX12Buffer() = default;
	private:
		WRL::ComPtr<ID3D12Resource> mBuffer;
	};
	
}

#endif