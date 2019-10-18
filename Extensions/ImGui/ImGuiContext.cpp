#include "ImGuiContext.hpp"

#include <Interface/GpuPipelineState/GpuPipelineFactory.hpp>
#include <Interface/GpuCommandAllocator.hpp>
#include <Interface/GpuCommandQueue.hpp>

//using Tools/ShaderCompiler compile the shader in shaders to .cso/.spv
constexpr CodeRed::Byte DxImGuiVertexShaderCode[] = { 68, 88, 66, 67, 131, 20, 245, 238, 75, 139, 17, 56, 36, 104, 184, 132, 17, 55, 210, 211, 1, 0, 0, 0, 40, 4, 0, 0, 5, 0, 0, 0, 52, 0, 0, 0, 132, 1, 0, 0, 244, 1, 0, 0, 104, 2, 0, 0, 140, 3, 0, 0, 82, 68, 69, 70, 72, 1, 0, 0, 1, 0, 0, 0, 112, 0, 0, 0, 1, 0, 0, 0, 60, 0, 0, 0, 1, 5, 254, 255, 0, 5, 0, 0, 32, 1, 0, 0, 19, 19, 68, 37, 60, 0, 0, 0, 24, 0, 0, 0, 40, 0, 0, 0, 40, 0, 0, 0, 36, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 116, 114, 97, 110, 115, 102, 111, 114, 109, 0, 171, 171, 100, 0, 0, 0, 1, 0, 0, 0, 136, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 0, 2, 0, 0, 0, 252, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 73, 110, 100, 101, 120, 0, 80, 114, 111, 106, 101, 99, 116, 105, 111, 110, 0, 102, 108, 111, 97, 116, 52, 120, 52, 0, 171, 171, 3, 0, 3, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 193, 0, 0, 0, 182, 0, 0, 0, 204, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 1, 0, 16, 0, 0, 0, 1, 0, 240, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 176, 0, 0, 0, 77, 105, 99, 114, 111, 115, 111, 102, 116, 32, 40, 82, 41, 32, 72, 76, 83, 76, 32, 83, 104, 97, 100, 101, 114, 32, 67, 111, 109, 112, 105, 108, 101, 114, 32, 49, 48, 46, 49, 0, 73, 83, 71, 78, 104, 0, 0, 0, 3, 0, 0, 0, 8, 0, 0, 0, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 89, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 1, 0, 0, 0, 15, 15, 0, 0, 95, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 2, 0, 0, 0, 3, 3, 0, 0, 80, 79, 83, 73, 84, 73, 79, 78, 0, 67, 79, 76, 79, 82, 0, 84, 69, 88, 67, 79, 79, 82, 68, 0, 79, 83, 71, 78, 108, 0, 0, 0, 3, 0, 0, 0, 8, 0, 0, 0, 80, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 92, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 1, 0, 0, 0, 15, 0, 0, 0, 98, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 2, 0, 0, 0, 3, 12, 0, 0, 83, 86, 95, 80, 79, 83, 73, 84, 73, 79, 78, 0, 67, 79, 76, 79, 82, 0, 84, 69, 88, 67, 79, 79, 82, 68, 0, 171, 83, 72, 69, 88, 28, 1, 0, 0, 81, 0, 1, 0, 71, 0, 0, 0, 106, 8, 0, 1, 89, 0, 0, 7, 70, 142, 48, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 95, 0, 0, 3, 50, 16, 16, 0, 0, 0, 0, 0, 95, 0, 0, 3, 242, 16, 16, 0, 1, 0, 0, 0, 95, 0, 0, 3, 50, 16, 16, 0, 2, 0, 0, 0, 103, 0, 0, 4, 242, 32, 16, 0, 0, 0, 0, 0, 1, 0, 0, 0, 101, 0, 0, 3, 242, 32, 16, 0, 1, 0, 0, 0, 101, 0, 0, 3, 50, 32, 16, 0, 2, 0, 0, 0, 104, 0, 0, 2, 1, 0, 0, 0, 56, 0, 0, 9, 242, 0, 16, 0, 0, 0, 0, 0, 86, 21, 16, 0, 0, 0, 0, 0, 70, 142, 48, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 50, 0, 0, 11, 242, 0, 16, 0, 0, 0, 0, 0, 70, 142, 48, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 6, 16, 16, 0, 0, 0, 0, 0, 70, 14, 16, 0, 0, 0, 0, 0, 0, 0, 0, 9, 242, 32, 16, 0, 0, 0, 0, 0, 70, 14, 16, 0, 0, 0, 0, 0, 70, 142, 48, 0, 0, 0, 0, 0, 2, 0, 0, 0, 3, 0, 0, 0, 54, 0, 0, 5, 242, 32, 16, 0, 1, 0, 0, 0, 70, 30, 16, 0, 1, 0, 0, 0, 54, 0, 0, 5, 50, 32, 16, 0, 2, 0, 0, 0, 70, 16, 16, 0, 2, 0, 0, 0, 62, 0, 0, 1, 83, 84, 65, 84, 148, 0, 0, 0, 6, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
constexpr CodeRed::Byte DxImGuiPixelShaderCode[] = { 68, 88, 66, 67, 217, 39, 164, 9, 210, 72, 12, 206, 92, 224, 25, 201, 56, 79, 27, 21, 1, 0, 0, 0, 8, 3, 0, 0, 5, 0, 0, 0, 52, 0, 0, 0, 4, 1, 0, 0, 120, 1, 0, 0, 172, 1, 0, 0, 108, 2, 0, 0, 82, 68, 69, 70, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 60, 0, 0, 0, 1, 5, 255, 255, 0, 5, 0, 0, 158, 0, 0, 0, 19, 19, 68, 37, 60, 0, 0, 0, 24, 0, 0, 0, 40, 0, 0, 0, 40, 0, 0, 0, 36, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 140, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 149, 0, 0, 0, 2, 0, 0, 0, 5, 0, 0, 0, 4, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 1, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 115, 97, 109, 112, 108, 101, 114, 48, 0, 116, 101, 120, 116, 117, 114, 101, 48, 0, 77, 105, 99, 114, 111, 115, 111, 102, 116, 32, 40, 82, 41, 32, 72, 76, 83, 76, 32, 83, 104, 97, 100, 101, 114, 32, 67, 111, 109, 112, 105, 108, 101, 114, 32, 49, 48, 46, 49, 0, 171, 171, 73, 83, 71, 78, 108, 0, 0, 0, 3, 0, 0, 0, 8, 0, 0, 0, 80, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 92, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 1, 0, 0, 0, 15, 15, 0, 0, 98, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 2, 0, 0, 0, 3, 3, 0, 0, 83, 86, 95, 80, 79, 83, 73, 84, 73, 79, 78, 0, 67, 79, 76, 79, 82, 0, 84, 69, 88, 67, 79, 79, 82, 68, 0, 171, 79, 83, 71, 78, 44, 0, 0, 0, 1, 0, 0, 0, 8, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 83, 86, 95, 84, 65, 82, 71, 69, 84, 0, 171, 171, 83, 72, 69, 88, 184, 0, 0, 0, 81, 0, 0, 0, 46, 0, 0, 0, 106, 8, 0, 1, 90, 0, 0, 6, 70, 110, 48, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 88, 24, 0, 7, 70, 126, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 85, 85, 0, 0, 0, 0, 0, 0, 98, 16, 0, 3, 242, 16, 16, 0, 1, 0, 0, 0, 98, 16, 0, 3, 50, 16, 16, 0, 2, 0, 0, 0, 101, 0, 0, 3, 242, 32, 16, 0, 0, 0, 0, 0, 104, 0, 0, 2, 1, 0, 0, 0, 69, 0, 0, 11, 242, 0, 16, 0, 0, 0, 0, 0, 70, 16, 16, 0, 2, 0, 0, 0, 70, 126, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 96, 32, 0, 0, 0, 0, 0, 1, 0, 0, 0, 56, 0, 0, 7, 242, 32, 16, 0, 0, 0, 0, 0, 70, 14, 16, 0, 0, 0, 0, 0, 70, 30, 16, 0, 1, 0, 0, 0, 62, 0, 0, 1, 83, 84, 65, 84, 148, 0, 0, 0, 3, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

constexpr CodeRed::Byte VkImGuiVertexShaderCode[] = { 3, 2, 35, 7, 0, 0, 1, 0, 7, 0, 13, 0, 48, 0, 0, 0, 0, 0, 0, 0, 17, 0, 2, 0, 1, 0, 0, 0, 11, 0, 6, 0, 1, 0, 0, 0, 71, 76, 83, 76, 46, 115, 116, 100, 46, 52, 53, 48, 0, 0, 0, 0, 14, 0, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 15, 0, 11, 0, 0, 0, 0, 0, 4, 0, 0, 0, 109, 97, 105, 110, 0, 0, 0, 0, 9, 0, 0, 0, 11, 0, 0, 0, 15, 0, 0, 0, 17, 0, 0, 0, 24, 0, 0, 0, 34, 0, 0, 0, 3, 0, 3, 0, 2, 0, 0, 0, 194, 1, 0, 0, 4, 0, 9, 0, 71, 76, 95, 65, 82, 66, 95, 115, 101, 112, 97, 114, 97, 116, 101, 95, 115, 104, 97, 100, 101, 114, 95, 111, 98, 106, 101, 99, 116, 115, 0, 0, 4, 0, 10, 0, 71, 76, 95, 71, 79, 79, 71, 76, 69, 95, 99, 112, 112, 95, 115, 116, 121, 108, 101, 95, 108, 105, 110, 101, 95, 100, 105, 114, 101, 99, 116, 105, 118, 101, 0, 0, 4, 0, 8, 0, 71, 76, 95, 71, 79, 79, 71, 76, 69, 95, 105, 110, 99, 108, 117, 100, 101, 95, 100, 105, 114, 101, 99, 116, 105, 118, 101, 0, 5, 0, 4, 0, 4, 0, 0, 0, 109, 97, 105, 110, 0, 0, 0, 0, 5, 0, 5, 0, 9, 0, 0, 0, 111, 117, 116, 67, 111, 108, 111, 114, 0, 0, 0, 0, 5, 0, 4, 0, 11, 0, 0, 0, 105, 110, 67, 111, 108, 111, 114, 0, 5, 0, 4, 0, 15, 0, 0, 0, 111, 117, 116, 85, 118, 0, 0, 0, 5, 0, 4, 0, 17, 0, 0, 0, 105, 110, 85, 118, 0, 0, 0, 0, 5, 0, 6, 0, 22, 0, 0, 0, 103, 108, 95, 80, 101, 114, 86, 101, 114, 116, 101, 120, 0, 0, 0, 0, 6, 0, 6, 0, 22, 0, 0, 0, 0, 0, 0, 0, 103, 108, 95, 80, 111, 115, 105, 116, 105, 111, 110, 0, 6, 0, 7, 0, 22, 0, 0, 0, 1, 0, 0, 0, 103, 108, 95, 80, 111, 105, 110, 116, 83, 105, 122, 101, 0, 0, 0, 0, 6, 0, 7, 0, 22, 0, 0, 0, 2, 0, 0, 0, 103, 108, 95, 67, 108, 105, 112, 68, 105, 115, 116, 97, 110, 99, 101, 0, 6, 0, 7, 0, 22, 0, 0, 0, 3, 0, 0, 0, 103, 108, 95, 67, 117, 108, 108, 68, 105, 115, 116, 97, 110, 99, 101, 0, 5, 0, 3, 0, 24, 0, 0, 0, 0, 0, 0, 0, 5, 0, 4, 0, 28, 0, 0, 0, 73, 110, 100, 101, 120, 0, 0, 0, 6, 0, 6, 0, 28, 0, 0, 0, 0, 0, 0, 0, 80, 114, 111, 106, 101, 99, 116, 105, 111, 110, 0, 0, 5, 0, 4, 0, 30, 0, 0, 0, 105, 110, 100, 101, 120, 0, 0, 0, 5, 0, 5, 0, 34, 0, 0, 0, 105, 110, 80, 111, 115, 105, 116, 105, 111, 110, 0, 0, 71, 0, 4, 0, 9, 0, 0, 0, 30, 0, 0, 0, 0, 0, 0, 0, 71, 0, 4, 0, 11, 0, 0, 0, 30, 0, 0, 0, 1, 0, 0, 0, 71, 0, 4, 0, 15, 0, 0, 0, 30, 0, 0, 0, 1, 0, 0, 0, 71, 0, 4, 0, 17, 0, 0, 0, 30, 0, 0, 0, 2, 0, 0, 0, 72, 0, 5, 0, 22, 0, 0, 0, 0, 0, 0, 0, 11, 0, 0, 0, 0, 0, 0, 0, 72, 0, 5, 0, 22, 0, 0, 0, 1, 0, 0, 0, 11, 0, 0, 0, 1, 0, 0, 0, 72, 0, 5, 0, 22, 0, 0, 0, 2, 0, 0, 0, 11, 0, 0, 0, 3, 0, 0, 0, 72, 0, 5, 0, 22, 0, 0, 0, 3, 0, 0, 0, 11, 0, 0, 0, 4, 0, 0, 0, 71, 0, 3, 0, 22, 0, 0, 0, 2, 0, 0, 0, 72, 0, 4, 0, 28, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 72, 0, 5, 0, 28, 0, 0, 0, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0, 0, 0, 72, 0, 5, 0, 28, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 16, 0, 0, 0, 71, 0, 3, 0, 28, 0, 0, 0, 2, 0, 0, 0, 71, 0, 4, 0, 34, 0, 0, 0, 30, 0, 0, 0, 0, 0, 0, 0, 19, 0, 2, 0, 2, 0, 0, 0, 33, 0, 3, 0, 3, 0, 0, 0, 2, 0, 0, 0, 22, 0, 3, 0, 6, 0, 0, 0, 32, 0, 0, 0, 23, 0, 4, 0, 7, 0, 0, 0, 6, 0, 0, 0, 4, 0, 0, 0, 32, 0, 4, 0, 8, 0, 0, 0, 3, 0, 0, 0, 7, 0, 0, 0, 59, 0, 4, 0, 8, 0, 0, 0, 9, 0, 0, 0, 3, 0, 0, 0, 32, 0, 4, 0, 10, 0, 0, 0, 1, 0, 0, 0, 7, 0, 0, 0, 59, 0, 4, 0, 10, 0, 0, 0, 11, 0, 0, 0, 1, 0, 0, 0, 23, 0, 4, 0, 13, 0, 0, 0, 6, 0, 0, 0, 2, 0, 0, 0, 32, 0, 4, 0, 14, 0, 0, 0, 3, 0, 0, 0, 13, 0, 0, 0, 59, 0, 4, 0, 14, 0, 0, 0, 15, 0, 0, 0, 3, 0, 0, 0, 32, 0, 4, 0, 16, 0, 0, 0, 1, 0, 0, 0, 13, 0, 0, 0, 59, 0, 4, 0, 16, 0, 0, 0, 17, 0, 0, 0, 1, 0, 0, 0, 21, 0, 4, 0, 19, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0, 43, 0, 4, 0, 19, 0, 0, 0, 20, 0, 0, 0, 1, 0, 0, 0, 28, 0, 4, 0, 21, 0, 0, 0, 6, 0, 0, 0, 20, 0, 0, 0, 30, 0, 6, 0, 22, 0, 0, 0, 7, 0, 0, 0, 6, 0, 0, 0, 21, 0, 0, 0, 21, 0, 0, 0, 32, 0, 4, 0, 23, 0, 0, 0, 3, 0, 0, 0, 22, 0, 0, 0, 59, 0, 4, 0, 23, 0, 0, 0, 24, 0, 0, 0, 3, 0, 0, 0, 21, 0, 4, 0, 25, 0, 0, 0, 32, 0, 0, 0, 1, 0, 0, 0, 43, 0, 4, 0, 25, 0, 0, 0, 26, 0, 0, 0, 0, 0, 0, 0, 24, 0, 4, 0, 27, 0, 0, 0, 7, 0, 0, 0, 4, 0, 0, 0, 30, 0, 3, 0, 28, 0, 0, 0, 27, 0, 0, 0, 32, 0, 4, 0, 29, 0, 0, 0, 9, 0, 0, 0, 28, 0, 0, 0, 59, 0, 4, 0, 29, 0, 0, 0, 30, 0, 0, 0, 9, 0, 0, 0, 32, 0, 4, 0, 31, 0, 0, 0, 9, 0, 0, 0, 27, 0, 0, 0, 59, 0, 4, 0, 16, 0, 0, 0, 34, 0, 0, 0, 1, 0, 0, 0, 43, 0, 4, 0, 6, 0, 0, 0, 36, 0, 0, 0, 0, 0, 0, 0, 43, 0, 4, 0, 6, 0, 0, 0, 37, 0, 0, 0, 0, 0, 128, 63, 32, 0, 4, 0, 43, 0, 0, 0, 3, 0, 0, 0, 6, 0, 0, 0, 54, 0, 5, 0, 2, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 248, 0, 2, 0, 5, 0, 0, 0, 61, 0, 4, 0, 7, 0, 0, 0, 12, 0, 0, 0, 11, 0, 0, 0, 62, 0, 3, 0, 9, 0, 0, 0, 12, 0, 0, 0, 61, 0, 4, 0, 13, 0, 0, 0, 18, 0, 0, 0, 17, 0, 0, 0, 62, 0, 3, 0, 15, 0, 0, 0, 18, 0, 0, 0, 65, 0, 5, 0, 31, 0, 0, 0, 32, 0, 0, 0, 30, 0, 0, 0, 26, 0, 0, 0, 61, 0, 4, 0, 27, 0, 0, 0, 33, 0, 0, 0, 32, 0, 0, 0, 61, 0, 4, 0, 13, 0, 0, 0, 35, 0, 0, 0, 34, 0, 0, 0, 81, 0, 5, 0, 6, 0, 0, 0, 38, 0, 0, 0, 35, 0, 0, 0, 0, 0, 0, 0, 81, 0, 5, 0, 6, 0, 0, 0, 39, 0, 0, 0, 35, 0, 0, 0, 1, 0, 0, 0, 80, 0, 7, 0, 7, 0, 0, 0, 40, 0, 0, 0, 38, 0, 0, 0, 39, 0, 0, 0, 36, 0, 0, 0, 37, 0, 0, 0, 145, 0, 5, 0, 7, 0, 0, 0, 41, 0, 0, 0, 33, 0, 0, 0, 40, 0, 0, 0, 65, 0, 5, 0, 8, 0, 0, 0, 42, 0, 0, 0, 24, 0, 0, 0, 26, 0, 0, 0, 62, 0, 3, 0, 42, 0, 0, 0, 41, 0, 0, 0, 65, 0, 6, 0, 43, 0, 0, 0, 44, 0, 0, 0, 24, 0, 0, 0, 26, 0, 0, 0, 20, 0, 0, 0, 61, 0, 4, 0, 6, 0, 0, 0, 45, 0, 0, 0, 44, 0, 0, 0, 127, 0, 4, 0, 6, 0, 0, 0, 46, 0, 0, 0, 45, 0, 0, 0, 65, 0, 6, 0, 43, 0, 0, 0, 47, 0, 0, 0, 24, 0, 0, 0, 26, 0, 0, 0, 20, 0, 0, 0, 62, 0, 3, 0, 47, 0, 0, 0, 46, 0, 0, 0, 253, 0, 1, 0, 56, 0, 1, 0 };
constexpr CodeRed::Byte VkImGuiPixelShaderCode[] = { 3, 2, 35, 7, 0, 0, 1, 0, 7, 0, 13, 0, 29, 0, 0, 0, 0, 0, 0, 0, 17, 0, 2, 0, 1, 0, 0, 0, 11, 0, 6, 0, 1, 0, 0, 0, 71, 76, 83, 76, 46, 115, 116, 100, 46, 52, 53, 48, 0, 0, 0, 0, 14, 0, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 15, 0, 8, 0, 4, 0, 0, 0, 4, 0, 0, 0, 109, 97, 105, 110, 0, 0, 0, 0, 9, 0, 0, 0, 11, 0, 0, 0, 25, 0, 0, 0, 16, 0, 3, 0, 4, 0, 0, 0, 7, 0, 0, 0, 3, 0, 3, 0, 2, 0, 0, 0, 194, 1, 0, 0, 4, 0, 9, 0, 71, 76, 95, 65, 82, 66, 95, 115, 101, 112, 97, 114, 97, 116, 101, 95, 115, 104, 97, 100, 101, 114, 95, 111, 98, 106, 101, 99, 116, 115, 0, 0, 4, 0, 10, 0, 71, 76, 95, 71, 79, 79, 71, 76, 69, 95, 99, 112, 112, 95, 115, 116, 121, 108, 101, 95, 108, 105, 110, 101, 95, 100, 105, 114, 101, 99, 116, 105, 118, 101, 0, 0, 4, 0, 8, 0, 71, 76, 95, 71, 79, 79, 71, 76, 69, 95, 105, 110, 99, 108, 117, 100, 101, 95, 100, 105, 114, 101, 99, 116, 105, 118, 101, 0, 5, 0, 4, 0, 4, 0, 0, 0, 109, 97, 105, 110, 0, 0, 0, 0, 5, 0, 5, 0, 9, 0, 0, 0, 111, 117, 116, 67, 111, 108, 111, 114, 0, 0, 0, 0, 5, 0, 4, 0, 11, 0, 0, 0, 105, 110, 67, 111, 108, 111, 114, 0, 5, 0, 5, 0, 15, 0, 0, 0, 116, 101, 120, 116, 117, 114, 101, 48, 0, 0, 0, 0, 5, 0, 5, 0, 19, 0, 0, 0, 115, 97, 109, 112, 108, 101, 114, 48, 0, 0, 0, 0, 5, 0, 4, 0, 25, 0, 0, 0, 105, 110, 85, 118, 0, 0, 0, 0, 71, 0, 4, 0, 9, 0, 0, 0, 30, 0, 0, 0, 0, 0, 0, 0, 71, 0, 4, 0, 11, 0, 0, 0, 30, 0, 0, 0, 0, 0, 0, 0, 71, 0, 4, 0, 15, 0, 0, 0, 34, 0, 0, 0, 0, 0, 0, 0, 71, 0, 4, 0, 15, 0, 0, 0, 33, 0, 0, 0, 0, 0, 0, 0, 71, 0, 4, 0, 19, 0, 0, 0, 34, 0, 0, 0, 0, 0, 0, 0, 71, 0, 4, 0, 19, 0, 0, 0, 33, 0, 0, 0, 1, 0, 0, 0, 71, 0, 4, 0, 25, 0, 0, 0, 30, 0, 0, 0, 1, 0, 0, 0, 19, 0, 2, 0, 2, 0, 0, 0, 33, 0, 3, 0, 3, 0, 0, 0, 2, 0, 0, 0, 22, 0, 3, 0, 6, 0, 0, 0, 32, 0, 0, 0, 23, 0, 4, 0, 7, 0, 0, 0, 6, 0, 0, 0, 4, 0, 0, 0, 32, 0, 4, 0, 8, 0, 0, 0, 3, 0, 0, 0, 7, 0, 0, 0, 59, 0, 4, 0, 8, 0, 0, 0, 9, 0, 0, 0, 3, 0, 0, 0, 32, 0, 4, 0, 10, 0, 0, 0, 1, 0, 0, 0, 7, 0, 0, 0, 59, 0, 4, 0, 10, 0, 0, 0, 11, 0, 0, 0, 1, 0, 0, 0, 25, 0, 9, 0, 13, 0, 0, 0, 6, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 32, 0, 4, 0, 14, 0, 0, 0, 0, 0, 0, 0, 13, 0, 0, 0, 59, 0, 4, 0, 14, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 26, 0, 2, 0, 17, 0, 0, 0, 32, 0, 4, 0, 18, 0, 0, 0, 0, 0, 0, 0, 17, 0, 0, 0, 59, 0, 4, 0, 18, 0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 27, 0, 3, 0, 21, 0, 0, 0, 13, 0, 0, 0, 23, 0, 4, 0, 23, 0, 0, 0, 6, 0, 0, 0, 2, 0, 0, 0, 32, 0, 4, 0, 24, 0, 0, 0, 1, 0, 0, 0, 23, 0, 0, 0, 59, 0, 4, 0, 24, 0, 0, 0, 25, 0, 0, 0, 1, 0, 0, 0, 54, 0, 5, 0, 2, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 248, 0, 2, 0, 5, 0, 0, 0, 61, 0, 4, 0, 7, 0, 0, 0, 12, 0, 0, 0, 11, 0, 0, 0, 61, 0, 4, 0, 13, 0, 0, 0, 16, 0, 0, 0, 15, 0, 0, 0, 61, 0, 4, 0, 17, 0, 0, 0, 20, 0, 0, 0, 19, 0, 0, 0, 86, 0, 5, 0, 21, 0, 0, 0, 22, 0, 0, 0, 16, 0, 0, 0, 20, 0, 0, 0, 61, 0, 4, 0, 23, 0, 0, 0, 26, 0, 0, 0, 25, 0, 0, 0, 87, 0, 5, 0, 7, 0, 0, 0, 27, 0, 0, 0, 22, 0, 0, 0, 26, 0, 0, 0, 133, 0, 5, 0, 7, 0, 0, 0, 28, 0, 0, 0, 12, 0, 0, 0, 27, 0, 0, 0, 62, 0, 3, 0, 9, 0, 0, 0, 28, 0, 0, 0, 253, 0, 1, 0, 56, 0, 1, 0 };

CodeRed::ImGuiFrameResources::ImGuiFrameResources(
	const std::shared_ptr<GpuLogicalDevice>& device) :
	mDevice(device)
{
	mVertexBuffer = mDevice->createBuffer(
		ResourceInfo::VertexBuffer(sizeof(ImDrawVert), mVertexBufferIncrease,
			MemoryHeap::Upload)
	);

	mIndexBuffer = mDevice->createBuffer(
		ResourceInfo::IndexBuffer(sizeof(ImDrawIdx), mIndexBufferIncrease,
			MemoryHeap::Upload)
	);
}

void CodeRed::ImGuiFrameResources::update(ImDrawData* drawData)
{
	CODE_RED_TRY_EXECUTE(
		mVertexBuffer->count() < drawData->TotalVtxCount,
		mVertexBuffer = mDevice->createBuffer(
			ResourceInfo::VertexBuffer(sizeof(ImDrawVert), mVertexBuffer->count() + mVertexBufferIncrease,
				MemoryHeap::Upload)
		)
	);

	CODE_RED_TRY_EXECUTE(
		mIndexBuffer->count() < drawData->TotalIdxCount,
		mIndexBuffer = mDevice->createBuffer(
			ResourceInfo::IndexBuffer(sizeof(ImDrawIdx), mIndexBuffer->count() + mIndexBufferIncrease,
				MemoryHeap::Upload)
		)
	);

	auto vtxMemory = static_cast<ImDrawVert*>(mVertexBuffer->mapMemory());
	auto idxMemory = static_cast<ImDrawIdx*>(mIndexBuffer->mapMemory());
	
	for (auto index = 0; index < drawData->CmdListsCount; index++) {
		const auto commandList = drawData->CmdLists[index];

		std::memcpy(vtxMemory, commandList->VtxBuffer.Data, commandList->VtxBuffer.size_in_bytes());
		std::memcpy(idxMemory, commandList->IdxBuffer.Data, commandList->IdxBuffer.size_in_bytes());

		vtxMemory = vtxMemory + commandList->VtxBuffer.Size;
		idxMemory = idxMemory + commandList->IdxBuffer.Size;
	}

	mVertexBuffer->unmapMemory();
	mIndexBuffer->unmapMemory();
}

CodeRed::ImGuiContext::ImGuiContext(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuRenderPass>& renderPass, 
	const size_t numFrameResources) :
	mDevice(device), mRenderPass(renderPass)
{
	auto& io = ImGui::GetIO();

	io.BackendRendererName = "CodeRed";
	io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
	
	mSampler = mDevice->createSampler(
		SamplerInfo(FilterOptions::MinLinearMagLinearMipLinear)
	);
	
	mResourceLayout = mDevice->createResourceLayout(
		{
			ResourceLayoutElement(ResourceType::Texture, 0)
		},
		{
			SamplerLayoutElement(mSampler, 1)
		},
		Constant32Bits(16, 2)
	);

	if (mDevice->apiVersion() == APIVersion::DirectX12) {
		mVertexShaderCode = std::vector<Byte>(sizeof(DxImGuiVertexShaderCode) / sizeof(Byte));
		mPixelShaderCode = std::vector<Byte>(sizeof(DxImGuiPixelShaderCode) / sizeof(Byte));
		
		std::memcpy(mVertexShaderCode.data(), DxImGuiVertexShaderCode, sizeof(DxImGuiVertexShaderCode));
		std::memcpy(mPixelShaderCode.data(), DxImGuiPixelShaderCode, sizeof(DxImGuiPixelShaderCode));
	}else {
		mVertexShaderCode = std::vector<Byte>(sizeof(VkImGuiVertexShaderCode) / sizeof(Byte));
		mPixelShaderCode = std::vector<Byte>(sizeof(VkImGuiPixelShaderCode) / sizeof(Byte));

		std::memcpy(mVertexShaderCode.data(), VkImGuiVertexShaderCode, sizeof(VkImGuiVertexShaderCode));
		std::memcpy(mPixelShaderCode.data(), VkImGuiPixelShaderCode, sizeof(VkImGuiPixelShaderCode));
	}
	
	auto pipelineFactory = mDevice->createPipelineFactory();
	
	mPipeline = mDevice->createGraphicsPipeline(
		mRenderPass,
		mResourceLayout,
		pipelineFactory->createInputAssemblyState(
			{
				InputLayoutElement("POSITION", PixelFormat::RedGreen32BitFloat),
				InputLayoutElement("TEXCOORD", PixelFormat::RedGreen32BitFloat),
				InputLayoutElement("COLOR", PixelFormat::RedGreenBlueAlpha8BitUnknown)
			},
			PrimitiveTopology::TriangleList
		),
		pipelineFactory->createShaderState(ShaderType::Vertex, mVertexShaderCode),
		pipelineFactory->createShaderState(ShaderType::Pixel, mPixelShaderCode),
		pipelineFactory->createDetphStencilState(false, false),
		pipelineFactory->createBlendState(BlendProperty(
			true,
			BlendOperator::Add,
			BlendOperator::Add,
			BlendFactor::Zero,
			BlendFactor::InvSrcAlpha,
			BlendFactor::InvSrcAlpha,
			BlendFactor::SrcAlpha
		)),
		pipelineFactory->createRasterizationState(
			FrontFace::Clockwise,
			CullMode::None,
			FillMode::Solid
		)
	);

	for (size_t index = 0; index < numFrameResources; index++) 
		mFrameResouces.push_back(ImGuiFrameResources(mDevice));

	mDescriptorHeap = mDevice->createDescriptorHeap(mResourceLayout);
	
	initializeFontsTexture();
}

void CodeRed::ImGuiContext::setRenderState(
	const std::shared_ptr<GpuGraphicsCommandList>& ctx,
	const ImGuiFrameResources& resources, 
	ImDrawData* drawData) const
{
	//copy from https://github.com/ocornut/imgui/blob/master/examples/imgui_impl_dx12.cpp#L68
	const auto L = drawData->DisplayPos.x;
	const auto R = drawData->DisplayPos.x + drawData->DisplaySize.x;
	const auto T = drawData->DisplayPos.y;
	const auto B = drawData->DisplayPos.y + drawData->DisplaySize.y;
	
	float mvp[4][4] =
	{
		{ 2.0f / (R - L),   0.0f,           0.0f,       0.0f },
		{ 0.0f,         2.0f / (T - B),     0.0f,       0.0f },
		{ 0.0f,         0.0f,           0.5f,       0.0f },
		{ (R + L) / (L - R),  (T + B) / (B - T),    0.5f,       1.0f },
	};

	std::vector<Value32Bit> value32Bits(16);

	std::memcpy(value32Bits.data(), mvp, sizeof(mvp));
	
	ctx->setViewPort({
		0,0,
		drawData->DisplaySize.x,
		drawData->DisplaySize.y,
		0.0f,1.0f
		});

	ctx->setVertexBuffer(resources.vertex());
	ctx->setIndexBuffer(resources.index(),
		sizeof(ImDrawIdx) == 2 ? IndexType::UInt16 : IndexType::UInt32);

	ctx->setGraphicsPipeline(mPipeline);
	ctx->setResourceLayout(mResourceLayout);
	ctx->setDescriptorHeap(mDescriptorHeap);
	ctx->setConstant32Bits(value32Bits);
}

void CodeRed::ImGuiContext::draw(
	const std::shared_ptr<GpuGraphicsCommandList>& ctx, 
	ImDrawData* drawData)
{
	//copy from https://github.com/ocornut/imgui/blob/master/examples/imgui_impl_dx12.cpp#L123
	// Avoid rendering when minimized
	if (drawData->DisplaySize.x <= 0.0f || drawData->DisplaySize.y <= 0.0f)
		return;

	auto &frameResources = mFrameResouces[mCurrentFrameIndex];

	frameResources.update(drawData);

	setRenderState(ctx, frameResources, drawData);

	size_t globalVtxOffset = 0;
	size_t globalIdxOffset = 0;

	const auto clipOff = drawData->DisplayPos;

	for (int nCommnadList = 0; nCommnadList < drawData->CmdListsCount; nCommnadList++) {
		const ImDrawList* commandList = drawData->CmdLists[nCommnadList];

		for (int nCommand = 0; nCommand < commandList->CmdBuffer.Size; nCommand++) {
			const auto command = &commandList->CmdBuffer[nCommand];

			if (command->UserCallback != nullptr) {
				if (command->UserCallback == ImDrawCallback_ResetRenderState)
					setRenderState(ctx, frameResources, drawData);
				else
					command->UserCallback(commandList, command);
			}else {
				//draw call commands
				ScissorRect rect = {
					static_cast<size_t>(command->ClipRect.x - clipOff.x),
					static_cast<size_t>(command->ClipRect.y - clipOff.y),
					static_cast<size_t>(command->ClipRect.z - clipOff.x),
					static_cast<size_t>(command->ClipRect.w - clipOff.y)
				};

				ctx->setScissorRect(rect);
				ctx->drawIndexed(command->ElemCount, 1,
					command->IdxOffset + globalIdxOffset,
					command->VtxOffset + globalVtxOffset, 0);
			}
		}
		
		globalIdxOffset = globalIdxOffset + commandList->IdxBuffer.Size;
		globalVtxOffset = globalVtxOffset + commandList->VtxBuffer.Size;
	}

	mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mFrameResouces.size();
}

void CodeRed::ImGuiContext::initializeFontsTexture()
{
	auto& io = ImGui::GetIO();

	Byte* pixels = nullptr;

	auto width = 0;
	auto height = 0;

	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

	mFontsTexture = mDevice->createTexture(
		ResourceInfo::Texture2D(width, height, PixelFormat::RedGreenBlueAlpha8BitUnknown)
	);

	const auto commandAllocator = mDevice->createCommandAllocator();
	const auto commandQueue = mDevice->createCommandQueue();
	const auto commandList = mDevice->createGraphicsCommandList(commandAllocator);

	commandList->beginRecording();
	commandList->layoutTransition(mFontsTexture, mFontsTexture->layout(), ResourceLayout::CopyDestination);
	
	commandList->copyMemoryToTexture(mFontsTexture, pixels);

	commandList->layoutTransition(mFontsTexture, mFontsTexture->layout(), ResourceLayout::GeneralRead);
	commandList->endRecording();
	
	commandQueue->execute({ commandList });
	commandQueue->waitIdle();

	mDescriptorHeap->bindTexture(mFontsTexture, 0);

	//we set it to nullptr, because we only need one font texture
	io.Fonts->TexID = nullptr;
}
