#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Utility.hpp>
#else
#include "../Utility.hpp"
#endif

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