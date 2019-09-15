#include "DirectX12Utility.hpp"

#ifdef __ENABLE__DIRECTX12__

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

auto CodeRed::wideStringToMultiString(const std::wstring& wstring) -> std::string
{
	const auto size = WideCharToMultiByte(
		CP_ACP, 0, wstring.c_str(),
		-1, nullptr, 0, nullptr, nullptr);

	const auto res = new char[size];

	WideCharToMultiByte(
		CP_ACP, 0, wstring.c_str(),
		-1, res, size, nullptr, nullptr);

	return res;
}


#endif