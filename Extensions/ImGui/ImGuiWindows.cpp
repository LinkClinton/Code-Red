#include "ImGuiWindows.hpp"

#include <CodeRed/Shared/DebugReport.hpp>

CodeRed::ImGuiView::ImGuiView(const ImGuiGenerator& generator) :
	mImGuiGenerator(generator)
{
	
}

CodeRed::ImGuiWindows::ImGuiWindows(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuRenderPass>& renderPass, 
	const size_t numFrameResources) :
	mImGuiContext(std::make_shared<ImGuiContext>(device, renderPass, numFrameResources))
{
}

CodeRed::ImGuiWindows::ImGuiWindows(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuRenderPass>& renderPass, 
	const std::shared_ptr<GpuCommandAllocator>& allocator,
	const std::shared_ptr<GpuCommandQueue>& queue, 
	const size_t numFrameResources) :
	mImGuiContext(std::make_shared<ImGuiContext>(device, renderPass, allocator, queue, numFrameResources))
{
}

void CodeRed::ImGuiWindows::reset(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuRenderPass>& renderPass, 
	const size_t numFrameResources)
{
	mImGuiContext.reset();
	mImGuiContext = std::make_shared<ImGuiContext>(device, renderPass, numFrameResources);
}

void CodeRed::ImGuiWindows::add(
	const std::string& windowName, 
	const std::string& viewName,
	const std::shared_ptr<ImGuiView>& view)
{
	if (mWindows.find(windowName) == mWindows.end())
		mWindows.insert({ windowName, Views() });

	CODE_RED_DEBUG_WARNING_IF(
		mWindows[windowName].find(viewName) != mWindows.end(),
		"the view name is existed in the window, we will replace it."
	);
	
	mWindows[windowName][viewName] = view;
}

void CodeRed::ImGuiWindows::add(const std::shared_ptr<ImGuiView>& view)
{
	mExtraCalls.push_back(view);
}

void CodeRed::ImGuiWindows::remove(
	const std::string& windowName, 
	const std::string& viewName)
{
	if (mWindows.find(windowName) == mWindows.end() || 
		mWindows[windowName].find(viewName) == mWindows[windowName].end()) {
		CODE_RED_DEBUG_WARNING("the window or view is not existed, we do not remove anything.")
		
		return;
	}

	mWindows[windowName].erase(viewName);

	if (mWindows[windowName].empty()) mWindows.erase(windowName);
}

void CodeRed::ImGuiWindows::update()
{
	ImGui::NewFrame();

	std::vector<std::pair<std::string, std::string>> removeList;
	
	//record the extra calls that was not deleted
	std::vector<std::weak_ptr<ImGuiView>> mNewExtraCalls;

	for (auto& extraCall : mExtraCalls) {
		//if the extra call is deleted, we do not process it
		if (extraCall.expired()) continue;

		extraCall.lock()->generator()();

		mNewExtraCalls.push_back(extraCall);
	}

	for (auto& window : mWindows) {
		//drawing the window
		ImGui::Begin(window.first.c_str());

		for (auto& weakView : window.second) {
			
			//if the imgui view is deleted, we will remove the view
			if (weakView.second.expired()) removeList.push_back({ window.first, weakView.first });
			else 
				if (ImGui::TreeNode(weakView.first.c_str())) {
					//drawing
					weakView.second.lock()->generator()();
					
					ImGui::TreePop();
				}
		}

		ImGui::End();
	}

	//remove views that is deleted.
	for (auto view : removeList) remove(view.first, view.second);
	
	//update the extra calls
	mExtraCalls = mNewExtraCalls;

	ImGui::Render();
}

void CodeRed::ImGuiWindows::draw(const std::shared_ptr<GpuGraphicsCommandList>& commandList)
{
	mImGuiContext->draw(commandList, ImGui::GetDrawData());
}
