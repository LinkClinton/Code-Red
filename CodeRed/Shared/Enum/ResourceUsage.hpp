#pragma once

#include "../Utility.hpp"

namespace CodeRed {

	enum class ResourceUsage : UInt32
	{
		None = 0,
		VertexBuffer = 1 << 0,
		IndexBuffer = 1 << 1,
		ConstantBuffer = 1 << 2,
		RenderTarget = 1 << 3,
		DepthStencil = 1 << 4
	};

	inline ResourceUsage operator | (const ResourceUsage& left, const ResourceUsage &right) {
		return
			static_cast<ResourceUsage>(
			static_cast<UInt32>(left) |
			static_cast<UInt32>(right));
	}

	inline ResourceUsage operator & (const ResourceUsage &left, const ResourceUsage& right) {
		return
			static_cast<ResourceUsage>(
			static_cast<UInt32>(left) &
			static_cast<UInt32>(right));
	}

	inline bool enumHas(const ResourceUsage& left, const ResourceUsage& right) {
		if ((left & right) == right) return true;
		return false;
	}
}