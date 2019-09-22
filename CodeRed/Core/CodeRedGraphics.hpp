#pragma once

#include "../Shared/Enum/PixelFormat.hpp"
#include "../Shared/Enum/AddressMode.hpp"
#include "../Shared/Enum/BlendFactor.hpp"
#include "../Shared/Enum/BlendOperator.hpp"
#include "../Shared/Enum/BorderColor.hpp"
#include "../Shared/Enum/ColorMask.hpp"
#include "../Shared/Enum/CompareOperator.hpp"
#include "../Shared/Enum/CullMode.hpp"
#include "../Shared/Enum/Dimension.hpp"
#include "../Shared/Enum/FillMode.hpp"
#include "../Shared/Enum/FilterOptions.hpp"
#include "../Shared/Enum/FrontFace.hpp"
#include "../Shared/Enum/MemoryHeap.hpp"
#include "../Shared/Enum/PrimitiveTopology.hpp"
#include "../Shared/Enum/ResourceLayout.hpp"
#include "../Shared/Enum/ResourceType.hpp"
#include "../Shared/Enum/ResourceUsage.hpp"
#include "../Shared/Enum/ShaderVisibility.hpp"
#include "../Shared/Enum/StencilOperator.hpp"

#include "../Shared/Information/ResourceInfo.hpp"
#include "../Shared/Information/SamplerInfo.hpp"
#include "../Shared/Information/WindowInfo.hpp"

#include "../Shared/BlendProperty.hpp"
#include "../Shared/DebugReport.hpp"
#include "../Shared/LayoutElement.hpp"
#include "../Shared/PixelFormatSizeOf.hpp"
#include "../Shared/ScissorRect.hpp"
#include "../Shared/StencilOperatorInfo.hpp"
#include "../Shared/Utility.hpp"
#include "../Shared/ViewPort.hpp"
#include "../Shared/Extent.hpp"

#include "../Shared/Exception/Exception.hpp"
#include "../Shared/Exception/NotSupportException.hpp"
#include "../Shared/Exception/FailedException.hpp"
#include "../Shared/Exception/InvalidException.hpp"
#include "../Shared/Exception/AssertException.hpp"
#include "../Shared/Exception/ZeroException.hpp"

#include "../Interface/GpuSystemInfo.hpp"
#include "../Interface/GpuSwapChain.hpp"
#include "../Interface/GpuResourceLayout.hpp"
#include "../Interface/GpuLogicalDevice.hpp"
#include "../Interface/GpuGraphicsPipeline.hpp"
#include "../Interface/GpuGraphicsCommandList.hpp"
#include "../Interface/GpuFrameBuffer.hpp"
#include "../Interface/GpuFence.hpp"
#include "../Interface/GpuDisplayAdapter.hpp"
#include "../Interface/GpuCommandQueue.hpp"
#include "../Interface/GpuCommandAllocator.hpp"

#include "../Interface/GpuResource/GpuSampler.hpp"
#include "../Interface/GpuResource/GpuTexture.hpp"
#include "../Interface/GpuResource/GpuBuffer.hpp"

#include "../Interface/GpuPipelineState/GpuPipelineFactory.hpp"
#include "../Interface/GpuPipelineState/GpuInputAssemblyState.hpp"
#include "../Interface/GpuPipelineState/GpuRasterizationState.hpp"
#include "../Interface/GpuPipelineState/GpuDepthStencilState.hpp"
#include "../Interface/GpuPipelineState/GpuShaderState.hpp"
#include "../Interface/GpuPipelineState/GpuBlendState.hpp"

#include "../DirectX12/DirectX12LogicalDevice.hpp"
#include "../DirectX12/DirectX12SystemInfo.hpp"