import sys

def CreateCharArray(dirName, fileName, suffixName):
    shaderFile = open(dirName + fileName + suffixName, "r")
    
    data = shaderFile.readlines()
    
    shaderFile.close()

    result = "constexpr char "+ fileName + "ShaderCode[] = \""

    for item in data:
        item = item.replace('\n', ' ')
        result += item + r'\n'
    pass

    result+="\";"

    return result

result = "#pragma once\n\n"

result += "namespace CodeRed {\n"

result += "\t" + CreateCharArray("Effects/Shaders/GeneralEffectPass/", "DxGeneralEffectPassVertex", ".hlsl") + "\n"
result += "\t" + CreateCharArray("Effects/Shaders/GeneralEffectPass/", "VkGeneralEffectPassVertex", ".vert") + "\n"
result += "\t" + CreateCharArray("Effects/Shaders/GeneralEffectPass/", "DxGeneralEffectPassPixel", ".hlsl") + "\n"
result += "\t" + CreateCharArray("Effects/Shaders/GeneralEffectPass/", "VkGeneralEffectPassPixel", ".frag") + "\n"

result += "\t" + CreateCharArray("Effects/Shaders/PhysicallyBasedEffectPass/", "DxPhysicallyBasedEffectPassVertex", ".hlsl") + "\n"
result += "\t" + CreateCharArray("Effects/Shaders/PhysicallyBasedEffectPass/", "VkPhysicallyBasedEffectPassVertex", ".vert") + "\n"
result += "\t" + CreateCharArray("Effects/Shaders/PhysicallyBasedEffectPass/", "DxPhysicallyBasedEffectPassPixel", ".hlsl") + "\n"
result += "\t" + CreateCharArray("Effects/Shaders/PhysicallyBasedEffectPass/", "VkPhysicallyBasedEffectPassPixel", ".frag") + "\n"

result += "\n}"

hppFile = open("Shaders/ShaderResources.hpp", "w")

hppFile.write(result)

hppFile.close()