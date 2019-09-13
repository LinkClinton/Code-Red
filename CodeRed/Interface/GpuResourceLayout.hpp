#pragma once

#include "../Shared/LayoutElement.hpp"

#include <vector>

namespace CodeRed {
	
	class GpuResourceLayout {
	protected:
		GpuResourceLayout() = default;
		
		explicit GpuResourceLayout(const std::vector<ResourceLayoutElement>& elements)
			: mElemnts(elements) {}
		
		~GpuResourceLayout() = default;
	public:
		auto element(const size_t index) const -> ResourceLayoutElement { return mElemnts[index]; }
		
		auto elements() const noexcept -> std::vector<ResourceLayoutElement> { return mElemnts; }
	protected:
		std::vector<ResourceLayoutElement> mElemnts = {};
	};
	
}