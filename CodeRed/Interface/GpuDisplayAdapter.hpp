#pragma once

#include "../Shared/Noncopyable.hpp"

#include <string>

namespace CodeRed {

	class GpuDisplayAdapter : public Noncopyable {
	protected:
		explicit GpuDisplayAdapter(
			const std::string& name,
			const size_t device_id,
			const size_t vendor_id) :
			mName(name),
			mDeviceId(device_id),
			mVendorId(vendor_id) {}
		
		~GpuDisplayAdapter() = default;
	public:
		auto name() const noexcept ->std::string { return mName; }
		
		auto deviceId() const noexcept -> size_t { return mDeviceId; }

		auto vendorId() const noexcept -> size_t { return mVendorId; }
	protected:
		std::string mName = "";

		size_t mDeviceId = 0;
		size_t mVendorId = 0;
	};
	
}