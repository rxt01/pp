#pragma once

#include <vector>

namespace Rxt{
    std::vector<unsigned char> cipherBlockChainig(std::vector<unsigned char> original, std::vector<unsigned char> key, std::vector<unsigned char> mainKey);
    std::vector<unsigned char> uniquePadCipher(std::vector<unsigned char> original, std::vector<unsigned char>  key);
    std::vector<unsigned char> singleBlockCipher(std::vector<unsigned char> original, std::vector<unsigned char>  key, std::vector<unsigned char> mainKey);
    std::vector<unsigned char> decipherBlockChainig(std::vector<unsigned char> original, std::vector<unsigned char>  key, std::vector<unsigned char> mainKey);
    std::vector<unsigned char> singleBlockDecipher(std::vector<unsigned char> original, std::vector<unsigned char>  key, std::vector<unsigned char> mainKey);
    std::vector<unsigned char> uniquePadDecipher(std::vector<unsigned char> original, std::vector<unsigned char>  key);
} // namespace Rxt

