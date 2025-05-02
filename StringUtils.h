#pragma once

#include <algorithm>
#include <array>
#include <cctype>
#include <string>
#include <sstream>

namespace shaderlab {
	class StringUtils
	{
	public:
		inline static bool EqualIgnoreCase(const std::string& s1, const std::string& s2)
		{
			std::string s11 = ToLower(s1);
			std::string s22 = ToLower(s2);
			return s11 == s22;
		}

		// 转换为小写
		inline static std::string ToLower(std::string s)
		{
			std::transform(s.begin(), s.end(), s.begin(),
				[](unsigned char c) { return std::tolower(c); });
			return s;
		}

		// 转换为大写
		inline static std::string ToUpper(std::string s)
		{
			std::transform(s.begin(), s.end(), s.begin(),
				[](unsigned char c) { return std::toupper(c); });
			return s;
		}

		// 去除字符串两端空白
		inline static std::string Trim(const std::string& s) {
			auto start = s.begin();
			while (start != s.end() && std::isspace(*start)) {
				++start;
			}

			auto end = s.end();
			while (end != start && std::isspace(*(end - 1))) {
				--end;
			}

			return std::string(start, end);
		}

		// 去除字符串包裹的引号
		inline static std::string UnwrapString(const std::string& s) {
			if (s.length() >= 2 &&
				((s.front() == '"' && s.back() == '"') ||
					(s.front() == '\'' && s.back() == '\''))) {
				return s.substr(1, s.length() - 2);
			}
			return s;
		}

		// 解析范围参数（例如 "(0.0, 1.0)"）
		inline static std::pair<float, float> ParseRangeParams(const std::string& s) {
			// 去除括号并分割参数
			size_t start = s.find('(');
			size_t end = s.find_last_of(')');
			const std::string content = StringUtils::Trim((start != std::string::npos) ? s.substr(start + 1, end - start - 1) : s);

			// 分割逗号分隔的值
			std::istringstream iss(content);
			std::string first, second;
			std::getline(iss, first, ',');
			std::getline(iss, second);

			// 转换并返回浮点数对
			return { std::stof(StringUtils::Trim(first)),std::stof(StringUtils::Trim(second)) };
		}

		// 解析矢量值（例如 "(1, 0.5, 0, 1)"）
		inline static std::array<float, 4> ParseVector(const std::string& s) {
			std::array<float, 4> result{};
			std::string content = StringUtils::Trim(s.substr(1, s.length() - 2)); // 去掉括号

			std::istringstream iss(content);
			std::string token;
			for (int i = 0; std::getline(iss, token, ',') && i < 4; ++i) {
				result[i] = std::stof(StringUtils::Trim(token));
			}
			return result;
		}

	};
}