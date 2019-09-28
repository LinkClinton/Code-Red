#pragma once

#include "DirectX12RenderPass.hpp"

#ifdef __ENABLE__DIRECTX12__

using namespace CodeRed::DirectX12;

CodeRed::DirectX12RenderPass::DirectX12RenderPass(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::optional<Attachment>& color, 
	const std::optional<Attachment>& depth) :
	GpuRenderPass(device, color, depth)
{
}

#endif