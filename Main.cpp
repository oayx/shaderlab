#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

#include "ShaderParser.h"

namespace fs = std::filesystem;
using namespace shaderlab;

void parseFolder(const fs::path& path);
int parseOneFile(const std::string& filePath, const std::string& fileName);
void traverseASTNode(std::shared_ptr<ASTNode> parentNode, int depth);
int main()
{
	// 使用二进制模式打开文件
	parseFolder(R"(.\Shaders)");
	//parseOneFile(R"(.\Shaders\DefaultResourcesExtra\Cubemaps\CubeBlend.shader)","CubeBlend");
	//parseOneFile(R"(.\Shaders\test.shader)","test");
	getchar();
	return 0;
}

void parseFolder(const fs::path& path) {
	try {
		// 递归目录迭代器
		for (const auto& entry : fs::recursive_directory_iterator(path)) {
			// 过滤掉目录项，只保留文件
			if (entry.is_regular_file() && entry.path().extension() == ".shader") {
				parseOneFile(entry.path().string(), entry.path().filename().stem().string());
			}
		}
	}
	catch (const fs::filesystem_error& e) {
		std::cerr << "Error accessing path: " << e.what() << '\n';
	}
}

int parseOneFile(const std::string& filePath, const std::string& fileName)
{
	std::cout << "*************************" << fileName << "*************************" << std::endl;
	// 使用二进制模式打开文件
	std::ifstream file(filePath, std::ios::binary);

	if (!file.is_open()) {
		std::cerr << "Error: Could not open shader file!" << std::endl;
		return 1;
	}

	// 读取文件内容到字符串（保留所有字节）
	std::string shaderCode(
		(std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>()
	);

	// 检查并移除UTF-8 BOM（可选）
	if (shaderCode.size() >= 3 &&
		static_cast<unsigned char>(shaderCode[0]) == 0xEF &&
		static_cast<unsigned char>(shaderCode[1]) == 0xBB &&
		static_cast<unsigned char>(shaderCode[2]) == 0xBF) {
		shaderCode.erase(0, 3);
	}

	//词法分析
	ShaderLexer lexer(fileName, shaderCode);
	auto tokens = lexer.tokenize();

	//语法分析
	ShaderParser parser(fileName, tokens);
	std::shared_ptr<ASTNode> root = parser.parse();
	traverseASTNode(root, 0);

	return 0;
}

void traverseASTNode(std::shared_ptr<ASTNode> parentNode, int depth)
{
	std::cout << std::string(depth, '\t') << (int)parentNode->Type << "," << parentNode->Value << std::endl;
	for (auto node : parentNode->children)
	{
		traverseASTNode(node, depth + 1);
	}
}