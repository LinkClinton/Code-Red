#pragma once

#include "../Shared/Noncopyable.hpp"

#include <string>

namespace CodeRed {

	class GpuDisplayAdapter : public Noncopyable {
	protected:
		explicit GpuDisplayAdapter(const std::string &name) :
			mName(name) {};
		
		~GpuDisplayAdapter() = default;
	public:
		auto name() const->std::string { return mName; }
	protected:
		std::string mName = "";
	};
	
}