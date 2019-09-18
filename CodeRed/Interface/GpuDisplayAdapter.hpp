#pragma once

#include <Shared/Noncopyable.hpp>
#include <Shared/Utility.hpp>

#include <string>

namespace CodeRed {

	class GpuDisplayAdapter : public Noncopyable {
	protected:
		explicit GpuDisplayAdapter(
			const std::string& name,
			const UInt32 device_id,
			const UInt32 vendor_id) :
			mName(name),
			mDeviceId(device_id),
			mVendorId(vendor_id) {}
		
		~GpuDisplayAdapter() = default;
	public:
		auto name() const noexcept ->std::string { return mName; }
		
		auto deviceId() const noexcept -> UInt32 { return mDeviceId; }

		auto vendorId() const noexcept -> UInt32 { return mVendorId; }
	protected:
		std::string mName = "";

		UInt32 mDeviceId = 0;
		UInt32 mVendorId = 0;
	};
	
}