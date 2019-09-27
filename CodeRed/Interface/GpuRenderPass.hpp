#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Noncopyable.hpp>
#include <Shared/Attachment.hpp>
#include <Shared/ClearValue.hpp>
#else
#include "../Shared/Noncopyable.hpp"
#include "../Shared/Attachment.hpp"
#include "../Shared/ClearValue.hpp"
#endif

#include <optional>
#include <vector>
#include <array>

namespace CodeRed {

	class GpuLogicalDevice;
	
	class GpuRenderPass : public Noncopyable {
	protected:
		explicit GpuRenderPass(
			const std::shared_ptr<GpuLogicalDevice> &device, 
			const std::optional<Attachment>& color,
			const std::optional<Attachment>& depth = std::nullopt);

		~GpuRenderPass() = default;
	public:
		void setClear(
			const std::optional<ClearValue>& color,
			const std::optional<ClearValue>& depth = std::nullopt);

		auto getClear(const size_t index = 0)
			-> std::pair<ClearValue, ClearValue> { return std::make_pair(mColor[index], mDepth); }
		
		auto color(const size_t index = 0) const -> std::optional<Attachment> { return mColorAttachments[index]; }

		auto depth() const noexcept -> std::optional<Attachment> { return mDepthAttachment; }
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;

		std::vector<std::optional<Attachment>> mColorAttachments = { std::nullopt };
		std::optional<Attachment> mDepthAttachment;
		
		std::vector<ClearValue> mColor = { ClearValue() };
		ClearValue mDepth;
	};
	
}