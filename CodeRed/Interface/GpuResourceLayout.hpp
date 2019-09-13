#pragma once

#include "../Shared/LayoutElement.hpp"

#include <vector>

namespace CodeRed {
	
	class GpuResourceLayout {
	protected:
		GpuResourceLayout() = default;
		
		explicit GpuResourceLayout(const std::vector<ResourceLayoutElement>& elements)
			: mElements(elements) {}
		
		~GpuResourceLayout() = default;
	public:
		auto element(const size_t index) const -> ResourceLayoutElement { return mElements[index]; }
		
		auto elements() const noexcept -> std::vector<ResourceLayoutElement> { return mElements; }
	protected:
		std::vector<ResourceLayoutElement> mElements = {};
	};
	
}