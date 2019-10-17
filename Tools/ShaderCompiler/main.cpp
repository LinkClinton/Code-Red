#include "ShaderCompiler.hpp"
#include "CommandList.hpp"

class ShaderContext {
public:
	void run() const {
		//no input file, we do not run
		if (mInputName.empty()) return;

		const auto shaderString = ShaderCompiler::readShader(mInputName);
		const auto shaderType = mShaderType == "pixel" ? ShaderType::Pixel : ShaderType::Vertex;

		std::cout << "compile file [" + mInputName + "] to " +
			(shaderType == ShaderType::Pixel ? "pixel" : "vertex") + " shader." << std::endl;

		std::cout << "compile file [" + mInputName + "] to " +
			(mAPIVersion == "glsl" ? "glsl" : "hlsl") + " shader." << std::endl;

		std::cout << "compile file [" + mInputName + "] to shader with entry point ["
			+ mEntryName << "]." << std::endl;
		
		const auto shaderCode = mAPIVersion == "glsl" ?
			ShaderCompiler::compileToSpv(shaderType, shaderString) :
			ShaderCompiler::compileToCso(shaderType, shaderString, mEntryName);

		//no ouput file or compile failed, we do not output any
		if (mOutputName.empty() || shaderCode.empty()) return;

		if (mArrayName.empty()) ShaderCompiler::writeToBinary(mOutputName, shaderCode);
		else ShaderCompiler::writeToCpp(mOutputName, mArrayName, shaderCode);
	}
public:
	std::string mInputName;
	std::string mArrayName;
	std::string mOutputName;
	std::string mAPIVersion = "hlsl";
	std::string mShaderType = "vertex";
	std::string mEntryName = "main";
};

/*
 * program
 * -i name : input the shader file
 * -cpp name : compile shader to a constexpr char[]
 * -e name : set the entry point(default: main)
 * -o name : output the code to file.
 * -v name : "vertex" or "pixel" indicate the version of shader(default: vertex)
 * -a name : "glsl" or "hlsl" indicate the api version(default: hlsl)
 */

int main(int argc, char** argv) {
	ShaderContext context;
	CommandList commandList;

	commandList.setCommand("-i", [](void* ctx, const std::string& fileName)
		{
			if (fileName.size() == 0) {
				std::cout << "error: input shader file is invalid." << std::endl;
				return false;
			}

			static_cast<ShaderContext*>(ctx)->mInputName = fileName;

			return true;
		});
	commandList.setCommand("-cpp", [](void* ctx, const std::string& arrayName)
		{
			static_cast<ShaderContext*>(ctx)->mArrayName = arrayName;

			return true;
		});
	commandList.setCommand("-e", [](void* ctx, const std::string& entry)
		{
			static_cast<ShaderContext*>(ctx)->mEntryName = entry;

			return true;
		});
	commandList.setCommand("-o", [](void* ctx, const std::string& fileName)
		{
			if (fileName.size() == 0) {
				std::cout << "error: output file is invalid." << std::endl;
				return false;
			}

			static_cast<ShaderContext*>(ctx)->mOutputName = fileName;

			return true;
		});
	commandList.setCommand("-v", [](void* ctx, const std::string& type)
		{
			static_cast<ShaderContext*>(ctx)->mShaderType = type;

			return true;
		});
	commandList.setCommand("-a", [](void* ctx, const std::string& version)
		{
			static_cast<ShaderContext*>(ctx)->mAPIVersion = version;

			return true;
		});

	commandList.execute(&context, CommandList::read_from_argv(argc, argv));
	
	context.run();
}