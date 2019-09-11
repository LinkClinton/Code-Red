#pragma once

#include "../Shared/LayoutElement.hpp"
#include "../Shared/Noncopyable.hpp"

#include <vector>

namespace CodeRed {
	
	class GpuResourceLayout : public Noncopyable {
	public:
		GpuResourceLayout(const std::vector<ResourceLayoutElement>& elements)
			: mElemnts(elements) {}
		
		~GpuResourceLayout() = default;

		auto elements() const->std::vector<ResourceLayoutElement> { return mElemnts; }
	protected:
		std::vector<ResourceLayoutElement> mElemnts;
	};
	
}