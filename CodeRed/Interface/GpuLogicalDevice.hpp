#pragma once

#include "../Shared/Noncopyable.hpp"

#include <memory>

namespace CodeRed {

	class GpuDisplayAdapter;
	
	class GpuLogicalDevice : public Noncopyable {
	protected:
		explicit GpuLogicalDevice(const std::shared_ptr<GpuDisplayAdapter>& adapter) :
			mDisplayAdapter(adapter) {};
			
		~GpuLogicalDevice() = default;
	protected:
		std::shared_ptr<GpuDisplayAdapter> mDisplayAdapter;
	};
	
}