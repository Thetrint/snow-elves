//
// Created by y1726 on 2024/7/1.
//
#include "utils/FunctionLibrary.h"

#include <vector>

// 辅助函数：将 Unicode 码点转换为 UTF-8
std::string codePointToUTF8(unsigned int cp) {
    std::string result;

    if (cp <= 0x7f) {
        result.resize(1);
        result[0] = static_cast<char>(cp);
    }
    else if (cp <= 0x7FF) {
        result.resize(2);
        result[1] = static_cast<char>(0x80 | (0x3f & cp));
        result[0] = static_cast<char>(0xC0 | (0x1f & (cp >> 6)));
    }
    else if (cp <= 0xFFFF) {
        if ((cp >= 0x4E00 && cp <= 0x9FA5) || (cp >= 0xF900 && cp <= 0xFA2D)) {
            wchar_t src[2] = { static_cast<wchar_t>(cp), 0 };
            char dest[5] = { 0 };
            std::string curLocale = setlocale(LC_ALL, NULL);
            setlocale(LC_ALL, "chs");
            wcstombs(dest, src, 5);
            result = dest;
            setlocale(LC_ALL, curLocale.c_str());
        } else {
            result.resize(3);
            result[2] = static_cast<char>(0x80 | (0x3f & cp));
            result[1] = static_cast<char>(0x80 | (0x3f & (cp >> 6)));
            result[0] = static_cast<char>(0xE0 | (0xf & (cp >> 12)));
        }
    }
    else if (cp <= 0x10FFFF) {
        result.resize(4);
        result[3] = static_cast<char>(0x80 | (0x3f & cp));
        result[2] = static_cast<char>(0x80 | (0x3f & (cp >> 6)));
        result[1] = static_cast<char>(0x80 | (0x3f & (cp >> 12)));
        result[0] = static_cast<char>(0xF0 | (0x7 & (cp >> 18)));
    }

    return result;
}

// 辅助函数：解码包含 Unicode 转义序列的字符串
std::wstring FunctionLibrary::decodeUnicode(const std::string& input) {
    std::wstring outBuffer;
    for (size_t x = 0; x < input.length();) {
        if (char aChar = input[x++]; aChar == '\\' && input[x] == 'u') {
            x++;
            unsigned int value = 0;
            for (int i = 0; i < 4; ++i) {
                aChar = input[x++];
                if (aChar >= '0' && aChar <= '9') {
                    value = (value << 4) + (aChar - '0');
                } else if (aChar >= 'a' && aChar <= 'f') {
                    value = (value << 4) + (10 + aChar - 'a');
                } else if (aChar >= 'A' && aChar <= 'F') {
                    value = (value << 4) + (10 + aChar - 'A');
                }
            }
            outBuffer.push_back(static_cast<wchar_t>(value));
        } else {
            outBuffer.push_back(aChar);
        }
    }
    return outBuffer;
}
