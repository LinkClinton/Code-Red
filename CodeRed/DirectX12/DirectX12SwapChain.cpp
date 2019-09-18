#include "../Shared/Exception/FailedException.hpp"

#include "DirectX12Resource/DirectX12Texture.hpp"

#include "DirectX12LogicalDevice.hpp"
#include "DirectX12SwapChain.hpp"

#ifdef __ENABLE__DIRECTX12__

CodeRed::DirectX12SwapChain::DirectX12SwapChain(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const WindowInfo& info, 
	const PixelFormat& format, 
	const size_t buffer_count) :
	GpuSwapChain(device, info, format, buffer_count)
{
	const auto dxDevice = static_cast<DirectX12LogicalDevice*>(mDevice.get())->device();
	
	D3D12_COMMAND_QUEUE_DESC queueInfo = {
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
		0,
		D3D12_COMMAND_QUEUE_FLAG_NONE,
		0
	};

	throwIfFailed(
		dxDevice->CreateCommandQueue(&queueInfo, IID_PPV_ARGS(&mPresentQueue)),
		FailedException({ "ID3D12CommandQueue of Present" }, DebugType::Create)
	);
	
	DXGI_SWAP_CHAIN_DESC swapInfo = {};

	swapInfo.BufferCount = static_cast<UINT>(mBuffers.size());
	swapInfo.BufferDesc.Format = enumConvert(format);
	swapInfo.BufferDesc.Width = static_cast<UINT>(mWindowInfo.width);
	swapInfo.BufferDesc.Height = static_cast<UINT>(mWindowInfo.height);
	swapInfo.BufferDesc.RefreshRate.Denominator = 1;
	swapInfo.BufferDesc.RefreshRate.Numerator = 60;
	swapInfo.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;
	swapInfo.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapInfo.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapInfo.Flags = 0;
	swapInfo.OutputWindow = static_cast<HWND>(mWindowInfo.handle);
	swapInfo.SampleDesc.Count = 1;
	swapInfo.SampleDesc.Quality = 0;
	swapInfo.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapInfo.Windowed = true;

	WRL::ComPtr<IDXGIFactory4> factory;
	WRL::ComPtr<IDXGISwapChain> temp_swap_chain;

	throwIfFailed(
		CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&factory)),
		FailedException({ "IDXGIFactory4" }, DebugType::Create)
	);

	throwIfFailed(
		factory->CreateSwapChain(mPresentQueue.Get(), &swapInfo, temp_swap_chain.GetAddressOf()),
		FailedException({ "IDXGISwapChain" }, DebugType::Create)
	);

	throwIfFailed(
		temp_swap_chain->QueryInterface(IID_PPV_ARGS(&mSwapChain)),
		FailedException({ "IDXGISwapChain3" }, DebugType::Get)
	);

	for (size_t index = 0; index < mBuffers.size(); index++) {
		WRL::ComPtr<ID3D12Resource> backBuffer;

		throwIfFailed(
			mSwapChain->GetBuffer(static_cast<UINT>(index), IID_PPV_ARGS(&backBuffer)),
			FailedException({ "ID3D12Resource of Back Buffer" }, DebugType::Get)
		);

		mBuffers[index] = std::make_shared<DirectX12Texture>(mDevice, backBuffer, 
			ResourceInfo::Texture2D(
				static_cast<size_t>(mWindowInfo.width),
				static_cast<size_t>(mWindowInfo.height),
				mPixelFormat,
				ResourceUsage::RenderTarget,
				ResourceLayout::Present,
				MemoryHeap::Default
			));
	}
}

#endif