#pragma once

#include "../../Interface/GpuResource/GpuTextureBuffer.hpp"
#include "../DirectX12Utility.hpp"

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12TextureBuffer final : public GpuTextureBuffer {
	public:
		explicit DirectX12TextureBuffer(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const TextureBufferInfo& info);

		explicit DirectX12TextureBuffer(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuTexture>& texture,
			const size_t mipSlice = 0);
		
		~DirectX12TextureBuffer() = default;

		auto read(const Extent3D<size_t>& extent) const -> std::vector<Byte> override;
		
		auto read() const -> std::vector<Byte> override;

		void write(const Extent3D<size_t>& extent, const void* data) override;
		
		void write(const void* data) override;

		auto texture() const noexcept -> WRL::ComPtr<ID3D12Resource> { return mTexture; }
	private:
		WRL::ComPtr<ID3D12Resource> mTexture;
	};
	
}

#endif
