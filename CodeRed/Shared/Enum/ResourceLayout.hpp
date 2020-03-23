#pragma once

#include "../Utility.hpp"

namespace CodeRed {

	enum class ResourceLayout : UInt32
	{
		GeneralRead,
		RenderTarget,
		DepthStencil,
		CopyDestination,
		CopySource,
		Present
	};
	
}