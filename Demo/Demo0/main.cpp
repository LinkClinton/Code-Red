#define __ENABLE__DIRECTX12__

#include <DirectX12/DirectX12SystemInfo.hpp>

#include <iostream>

int main() {
	CodeRed::DirectX12SystemInfo x;

	auto y = x.selectDisplayAdapter();

	for (auto &it : y) {
		std::cout << it->name() << std::endl;
	}
	
}