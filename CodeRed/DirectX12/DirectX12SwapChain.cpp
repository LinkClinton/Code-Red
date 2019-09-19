#include "../Shared/Exception/FailedException.hpp"
#include "../Shared/Exception/ZeroException.hpp"

#include "DirectX12Resource/DirectX12Texture.hpp"

#include "DirectX12LogicalDevice.hpp"
#include "DirectX12CommandQueue.hpp"
#include "DirectX12SwapChain.hpp"

#include <algorithm>

#ifdef __ENABLE__DIRECTX12__

CodeRed::DirectX12SwapChain::DirectX12SwapChain(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuCommandQueue>& queue,
	const WindowInfo& info, 
	const PixelFormat& format, 
	const size_t buffer_count) :
	GpuSwapChain(device, queue, info, format, buffer_count)
{
	const auto dxDevice = static_cast<DirectX12LogicalDevice*>(mDevice.get())->device();
	const auto dxQueue = static_cast<DirectX12CommandQueue*>(mQueue.get())->queue();
	
	DXGI_SWAP_CHAIN_DESC swapInfo = {};

	swapInfo.BufferCount = static_cast<UINT>(mBuffers.size());
	swapInfo.BufferDesc.Format = enumConvert(format);
	swapInfo.BufferDesc.Width = static_cast<UINT>(mInfo.width);
	swapInfo.BufferDesc.Height = static_cast<UINT>(mInfo.height);
	swapInfo.BufferDesc.RefreshRate.Denominator = 1;
	swapInfo.BufferDesc.RefreshRate.Numerator = 60;
	swapInfo.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;
	swapInfo.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapInfo.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapInfo.Flags = 0;
	swapInfo.OutputWindow = static_cast<HWND>(mInfo.handle);
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
		factory->CreateSwapChain(dxQueue.Get(), &swapInfo, temp_swap_chain.GetAddressOf()),
		FailedException({ "IDXGISwapChain" }, DebugType::Create)
	);

	throwIfFailed(
		temp_swap_chain->QueryInterface(IID_PPV_ARGS(&mSwapChain)),
		FailedException({ "IDXGISwapChain3", "IDXGISwapChain" }, DebugType::Get)
	);

	//we use resize to create the texture of back buffer
	//so we need to set the mInfo to zero
	const auto temp = mInfo;
	mInfo.width = 0; mInfo.height = 0;

	resize(temp.width, temp.height);
}

void CodeRed::DirectX12SwapChain::resize(const size_t width, const size_t height)
{
	//if the size is not change, we do nothing
	if (width == mInfo.width && height == mInfo.height) return;

	CODE_RED_DEBUG_THROW_IF(
		width == 0 || height == 0,
		ZeroException<UInt32>({ "width or height" })
	);

	mInfo.width = width;
	mInfo.height = height;

	//release all back buffers
	//if we use these back buffers to create rtv or any things
	//we need to destory them before we resize the swap chain
	for (auto& buffer : mBuffers) buffer.reset();

	throwIfFailed(
		mSwapChain->ResizeBuffers(2,
			static_cast<UINT>(mInfo.width),
			static_cast<UINT>(mInfo.height),
			enumConvert(mPixelFormat), 0),
		FailedException({ "Resize of IDXGISwapChain" }, DebugType::Create)
	);
	
	for (size_t index = 0; index < mBuffers.size(); index++) {
		WRL::ComPtr<ID3D12Resource> backBuffer;

		throwIfFailed(
			mSwapChain->GetBuffer(static_cast<UINT>(index), IID_PPV_ARGS(&backBuffer)),
			FailedException({ "ID3D12Resource of Back Buffer", "IDXGISwapChain" }, DebugType::Get)
		);

		mBuffers[index] = std::make_shared<DirectX12Texture>(mDevice, backBuffer,
			ResourceInfo::Texture2D(
				static_cast<size_t>(mInfo.width),
				static_cast<size_t>(mInfo.height),
				mPixelFormat,
				ResourceUsage::RenderTarget,
				ResourceLayout::Present,
				MemoryHeap::Default
			));
	}
}

void CodeRed::DirectX12SwapChain::present(bool sync)
{
	mSwapChain->Present(sync ? 1 : 0, 0);
}

auto CodeRed::DirectX12SwapChain::currentBufferIndex() const
	-> size_t
{
	return mSwapChain->GetCurrentBackBufferIndex();
}

#endif
