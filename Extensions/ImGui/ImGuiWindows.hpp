#pragma once

#include "ImGuiContext.hpp"

#include <unordered_map>
#include <functional>

namespace CodeRed {

	using ImGuiGenerator = std::function<void()>;
	
	class ImGuiView final : public Noncopyable {
	public:
		explicit ImGuiView(
			const ImGuiGenerator& generator);

		auto generator() const -> const ImGuiGenerator& { return mImGuiGenerator; }
	private:
		ImGuiGenerator mImGuiGenerator;
	};

	class ImGuiWindows final : Noncopyable {
	public:
		explicit ImGuiWindows(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuRenderPass>& renderPass,
			const size_t numFrameResources = 2);

		explicit ImGuiWindows(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuRenderPass>& renderPass,
			const std::shared_ptr<GpuCommandAllocator>& allocator,
			const std::shared_ptr<GpuCommandQueue>& queue,
			const size_t numFrameResources = 2);
		
		~ImGuiWindows() = default;

		void reset(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuRenderPass>& renderPass,
			const size_t numFrameResources = 2);

		void add(
			const std::string& windowName,
			const std::string& viewName,
			const std::shared_ptr<ImGuiView>& view);

		void add(
			const std::string& name,
			const std::shared_ptr<ImGuiView>& view);

		void remove(
			const std::string& windowName,
			const std::string& viewName);

		void remove(
			const std::string& name);

		void update();
		
		void draw(
			const std::shared_ptr<GpuGraphicsCommandList>& commandList);
	private:
		using Views = std::unordered_map<std::string, std::weak_ptr<ImGuiView>>;
		
		std::shared_ptr<ImGuiContext> mImGuiContext;

		std::unordered_map<std::string, Views> mWindows;
		std::unordered_map<std::string, std::weak_ptr<ImGuiView>> mExtraCalls;
	};
	
}