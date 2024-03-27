/*
PP. encrypt and decrypt files and messages 
    Copyright (C) 2023  Sacha Bazin

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "../headers/crypt.h"
#include "crypt.h"
std::vector<unsigned char> Rxt::cipherBlockChainig(std::vector<unsigned char> original, std::vector<unsigned char>  key, std::vector<unsigned char> mainKey){
    int totalSize = original.size();
    int keySize = key.size();
    int numberOfBlocks = totalSize/keySize +1;
    std::vector<unsigned char> firstBlock = {};
    for (int i = 0; i < keySize; i++){
        firstBlock.push_back(original[i]);
    }
    firstBlock = singleBlockCipher(firstBlock, key, mainKey);
    std::vector<unsigned char> final = firstBlock;
    std::vector<unsigned char> lastBlock = firstBlock;
    firstBlock.~vector();
    for (int i = 1; i < numberOfBlocks; i++){
        std::vector<unsigned char> tmp = {};
        for (int n = 0; n < keySize; n++){
            tmp.push_back(original[i*keySize+n]);
        }
        
        lastBlock = singleBlockCipher(tmp, lastBlock, mainKey);
        for (int n = 0; n < lastBlock.size(); n++){
            final.push_back(lastBlock[n]);
        }
        
    }
    
    return final;
}

std::vector<unsigned char> Rxt::uniquePadCipher(std::vector<unsigned char> original,  std::vector<unsigned char>  key){
    std::vector<unsigned char> final = {};
    for (int i = 0; i < key.size() & i< original.size(); i++){
        final.push_back(original[i]+key[i]);
    }
    return final;
}

std::vector<unsigned char> Rxt::singleBlockCipher(std::vector<unsigned char> original,  std::vector<unsigned char>  key, std::vector<unsigned char> mainKey){
    std::vector<unsigned char> tmp = {};
    for (int i = 0; i < original.size(); i++){
        tmp.push_back(original[i] ^ key[i]);
    } 
    std::vector<unsigned char> final = uniquePadCipher(tmp,mainKey);
    return final;
}

std::vector<unsigned char> Rxt::decipherBlockChainig(std::vector<unsigned char> original, std::vector<unsigned char>  key, std::vector<unsigned char> mainKey){
    int totalSize = original.size();
    int keySize = key.size();
    int numberOfBlocks = totalSize/keySize +1;
    std::vector<unsigned char> firstBlock = {};
    for (int i = 0; i < keySize; i++){
        firstBlock.push_back(original[i]);
    }
    std::vector<unsigned char> lastBlock = firstBlock;
    firstBlock = singleBlockDecipher(firstBlock, key, mainKey);
    std::vector<unsigned char> final = firstBlock;
    firstBlock.~vector();
    for (int i = 1; i < numberOfBlocks; i++){
        std::vector<unsigned char> tmp = {};
        for (int n = 0; n < keySize; n++){
            tmp.push_back(original[i*keySize+n]);
        }
        std::vector<unsigned char> decryptedBlock = singleBlockDecipher(tmp, lastBlock, mainKey);
        lastBlock = tmp;
        for (int n = 0; n < decryptedBlock.size(); n++){
            final.push_back(decryptedBlock[n]);
        }
        
    }
    
    return final;
    return std::vector<unsigned char>();
}



std::vector<unsigned char> Rxt::singleBlockDecipher(std::vector<unsigned char> original, std::vector<unsigned char>  key, std::vector<unsigned char> mainKey){
    std::vector<unsigned char> tmp = uniquePadDecipher(tmp,mainKey);
    std::vector<unsigned char> final ={};
    for (int i = 0; i < tmp.size(); i++){
        final.push_back(tmp[i] ^ key[i]);
    }
    return final;
}

std::vector<unsigned char> Rxt::uniquePadDecipher(std::vector<unsigned char> original, std::vector<unsigned char> key){
    std::vector<unsigned char> final = {};
    for (int i = 0; i < key.size() & i< original.size(); i++){
        final.push_back(original[i]-key[i]);
    }
    return final;
}
