#pragma once

#include <shaderc/shaderc.hpp>
#include <d3dcompiler.h>
#include <string>


using UInt32 = unsigned;
using Byte = unsigned char;
	
enum class ShaderType : UInt32 {
	Vertex,
	Pixel
};
	
class ShaderCompiler {
public:
	static auto readShader(const std::string& fileName)->std::string;

	static void writeToBinary(
		const std::string& fileName,
		const std::vector<Byte>& code);

	static void writeToCpp(
		const std::string& fileName,
		const std::string& arrayName,
		const std::vector<Byte>& code);

	static auto compileToSpv(
		const ShaderType& shaderType,
		const std::string& shader)
		->std::vector<Byte>;

	static auto compileToCso(
		const ShaderType& type,
		const std::string& shader,
		const std::string& entry = "main")
		->std::vector<Byte>;

};
	
	