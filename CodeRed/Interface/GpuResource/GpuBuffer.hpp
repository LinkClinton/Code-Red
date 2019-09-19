#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Interface/GpuResource/GpuResource.hpp>
#include <Shared/Exception/ZeroException.hpp>
#else
#include "GpuResource.hpp"
#include "../../Shared/Exception/ZeroException.hpp"
#endif

namespace CodeRed {

	class GpuBuffer : public GpuResource {
	protected:
		explicit GpuBuffer(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const ResourceInfo& info);
		
		~GpuBuffer() = default;
	public:
		auto size() const -> size_t override { return std::get<BufferProperty>(mInfo.Property).Size; }

		auto stride() const -> size_t { return std::get<BufferProperty>(mInfo.Property).Stride; }

		auto count() const -> size_t { return std::get<BufferProperty>(mInfo.Property).Count; }
	};
	
}