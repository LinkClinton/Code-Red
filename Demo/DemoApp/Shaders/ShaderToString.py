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

result += "\t" + CreateCharArray("Effects/Shaders/", "DxEffectPassVertex", ".hlsl") + "\n"
result += "\t" + CreateCharArray("Effects/Shaders/", "DxEffectPassPixel", ".hlsl") + "\n"

result += "\n}"

hppFile = open("Shaders/ShaderResources.hpp", "w")

hppFile.write(result)

hppFile.close()