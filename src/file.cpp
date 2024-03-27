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

#include "../headers/file.h"
#include <fstream>
#include <iterator>
#include <stdexcept>

std::vector<unsigned char> Rxt::read(const char* filepath){
    std::ifstream file(filepath, std::ios::binary);
    if (file.is_open()){
        // Stop eating new lines in binary mode!
        file.unsetf(std::ios::skipws);

        // Get its size
        std::streampos fileSize;

        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        // Reserve capacity.
        std::vector<unsigned char> content;
            content.reserve(fileSize);

        // Read the data.
        content.insert(content.begin(),std::istream_iterator<unsigned char>(file),std::istream_iterator<unsigned char>());
        file.close();
        return content;
    }
    throw std::runtime_error("Couldn't read the input file.\n");
}

void Rxt::write(const char* filepath, std::vector<unsigned char> content){
    std::ofstream file(filepath, std::ios::out | std::ios::binary );
    if (file.is_open()){
        std::copy(content.cbegin(), content.cend(), std::ostream_iterator<unsigned char>(file));
    }else{
        throw std::runtime_error("Couldn't open the output file.\n");
    }
    
}