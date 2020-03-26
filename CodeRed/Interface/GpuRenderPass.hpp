#pragma once

#include "../Shared/Noncopyable.hpp"
#include "../Shared/Attachment.hpp"
#include "../Shared/ClearValue.hpp"

#include <optional>
#include <vector>
#include <array>

namespace CodeRed {

	class GpuLogicalDevice;
	class GpuFrameBuffer;
	
	class GpuRenderPass : public Noncopyable {
	protected:
		explicit GpuRenderPass(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::vector<Attachment>& colors,
			const std::optional<Attachment>& depth = std::nullopt);
		
		~GpuRenderPass() = default;
	public:
		void setClear(
			const std::optional<ClearValue>& color,
			const std::optional<ClearValue>& depth = std::nullopt);

		void setClear(
			const std::vector<ClearValue>& colors,
			const std::optional<ClearValue>& depth = std::nullopt);

		auto compatible(const std::shared_ptr<GpuFrameBuffer>& frameBuffer) const -> bool;

		auto maxSample() const noexcept -> MultiSample { return mMaxSample; }
		
		auto colorClear() const noexcept -> std::vector<ClearValue> { return mColors; }

		auto depthClear() const noexcept -> std::optional<ClearValue> { return mDepth; }
		
		auto color(const size_t index = 0) const -> std::optional<Attachment> { return mColorAttachments[index]; }

		auto depth() const noexcept -> std::optional<Attachment> { return mDepthAttachment; }

		auto size() const noexcept -> size_t { return mColorAttachments.size(); }
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;

		std::vector<Attachment> mColorAttachments;
		std::vector<ClearValue> mColors;

		std::optional<Attachment> mDepthAttachment;
		std::optional<ClearValue> mDepth;

		MultiSample mMaxSample = MultiSample::Count1;
	};
	
}