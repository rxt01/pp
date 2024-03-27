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

#include <iostream>
#include "../headers/file.h"
#include "../headers/crypt.h"
#include <algorithm>
#include <cctype>

enum lang{fr,en};


int encryptFile(std::vector<unsigned char> passphrase, const char* origin, const char* destination);
int encryptFile(std::vector<unsigned char> passphrase, const char* origin, const char* destination, std::vector<unsigned char*> permutations);
int decryptFile(std::vector<unsigned char> passphrase, const char *origin, const char *destination);
int decryptFile(std::vector<unsigned char> passphrase, const char* origin, const char* destination, std::vector<unsigned char*> permutations);


std::vector<unsigned char> encryptMsg(std::vector<unsigned char> passphrase, std::vector<unsigned char> msg);
std::vector<unsigned char> encryptMsg(std::vector<unsigned char> passphrase, std::vector<unsigned char> msg, std::vector<unsigned char*> permutations);
std::vector<unsigned char> decryptMsg(std::vector<unsigned char> passphrase, std::vector<unsigned char> msg);
std::vector<unsigned char> decryptMsg(std::vector<unsigned char> passphrase, std::vector<unsigned char> msg, std::vector<unsigned char*> permutations);


void help(lang language);
void allong();
int main(int argc, char** argv){
    //help
    if (argc == 1){
        allong();
        return 0;
    }
    else if ((argv[1]=="-h"||argv[1]=="--help")&argc == 3){
        switch (((const char *)argv[2])[0]){
        case 'f':
            help(fr);
            return 0;
            break;
        case 'e':
            help(en);
            return 0;
            break;
        
        default:
            std::cerr << "Error : Language not recognized.\n";
            return 0;
            break;
        }
    } else if (argv[1] == "-h" || argv[1] == "--help"){
        help(fr);
        return 0;
    }
    
    

    bool msgEncryption = false;
    bool usePermutations = false;
    bool encrypt = true;
    const char * origin;
    const char * destination;
    std::vector<unsigned char> passphrase;
    std::vector<unsigned char> msg;
    std::vector<unsigned char*> permutations = {};

    bool gettingOrigin = false;
    bool gettingDestination = false;
    bool gettingPassphrase = false;
    bool gettingMessage = false;
    bool gettingPermutations = false;
    /* Parse arguments */
    for (int i = 0; i < argc; i++){
        if (argv[i]=="-o"|| argv[i]=="--origin"){
            gettingOrigin == true;
            gettingDestination = false;
            gettingPassphrase = false;
            gettingMessage = false;
            gettingPermutations = false;
        }else if (argv[i]=="-d"|| argv[i]=="--destination"){
            gettingOrigin == false;
            gettingDestination = true;
            gettingPassphrase = false;
            gettingMessage = false;
            gettingPermutations = false;
        }else if (argv[i]=="-p"|| argv[i]=="--passphrase"){
            gettingOrigin == true;
            gettingDestination = false;
            gettingPassphrase = false;
            gettingMessage = false;
            gettingPermutations = false;
        }else if (argv[i]=="-m"|| argv[i]=="--message"){
            gettingOrigin == false;
            gettingDestination = false;
            gettingPassphrase = false;
            gettingMessage = true;
            gettingPermutations = false;
            msgEncryption = true;
        }else if (argv[i]=="-p"|| argv[i]=="--permutations"){
            gettingOrigin == false;
            gettingDestination = false;
            gettingPassphrase = false;
            gettingMessage = false;
            gettingPermutations = true;
            usePermutations = true;
        }else if(argv[i]=="-d"||argv[i]=="--decrypt"){
            encrypt = false;
        }else{
            if(gettingOrigin){
                origin = argv[i];
            }else if(gettingDestination){
                destination = argv[i];
            }else if (gettingPassphrase){
                passphrase = {};
                for (int j = 0; j < sizeof(argv[1]); j++){
                    passphrase.push_back(argv[i][j]);
                }
                
            }else if (gettingMessage){
                msg = {};
                for (int j = 0; j < sizeof(argv[1]); j++){
                    msg.push_back(argv[i][j]);
                }
            }else if (gettingPermutations){
                permutations.push_back((unsigned char *)argv[i]);
            }
        }
    }
    char flags = msgEncryption <<2 +usePermutations <<1 + encrypt ^0b00000001;
    std::vector<unsigned char> encrypted;
    std::vector<unsigned char> decrypted;
    switch (flags){
    case 0x00:
        encryptFile(passphrase,origin,destination);
        return 0;
        break;
    case 0x01:
        decryptFile(passphrase, origin, destination);
        return 0;
        break;
    case 0x02:
        encryptFile(passphrase,origin,destination,permutations);
        return 0;
        break;
    case 0x03:
        decryptFile(passphrase,origin,destination,permutations);
        return 0;
        break;
    case 0x04:
        encrypted = encryptMsg(passphrase,msg);
        for (int i = 0; i < encrypted.size(); i++){
            std::cout << encrypted[1];
        }
        std::cout <<"\n";
        return 0;
        break;
    case 0x05:
        decrypted = decryptMsg(passphrase,msg);
        for (int i = 0; i < decrypted.size(); i++){
            std::cout << decrypted[1];
        }
        std::cout <<"\n";
        return 0;
        break;
    case 0x06:
        encrypted = encryptMsg(passphrase,msg, permutations);
        for (int i = 0; i < encrypted.size(); i++){
            std::cout << encrypted[1];
        }
        std::cout <<"\n";
        return 0;
        break;
    case 0x07:
        decrypted = decryptMsg(passphrase,msg, permutations);
        for (int i = 0; i < decrypted.size(); i++){
            std::cout << decrypted[1];
        }
        std::cout <<"\n";
        return 0;
        break;
    default:
        break;
    }
    return 0;
}

int encryptFile(std::vector<unsigned char> passphrase, const char *origin, const char *destination){
    std::vector<unsigned char> toEncrypt = Rxt::read(origin);
    std::vector<unsigned char> encrypted = Rxt::cipherBlockChainig(toEncrypt,passphrase,passphrase);
    Rxt::write(destination, encrypted);
    return 0;
}

int encryptFile(std::vector<unsigned char> passphrase, const char *origin, const char *destination, std::vector<unsigned char *> permutations){
    std::vector<unsigned char> toEncrypt = Rxt::read(origin);
    for (int i = 0; i < permutations.size(); i++){
        unsigned char a = permutations[i][0];
        unsigned char b = permutations[i][1];
        std::vector<long long> as = {};
        std::vector<long long> bs = {};
        for (int j = 0; j < toEncrypt.size(); j++) {
            if (toEncrypt[j] == a) as.push_back(j);
            else if (toEncrypt[j] == b) bs.push_back(j);
        }
        for (int j = 0; j <as.size(); j++){
            toEncrypt[as[j]]=b;
        }
        for (int j = 0; j < bs.size(); j++){
            toEncrypt[bs[j]]=a;
        }
    }
    std::vector<unsigned char> encrypted = Rxt::cipherBlockChainig(toEncrypt,passphrase,passphrase);
    Rxt::write(destination, encrypted);
    return 0;
}

int decryptFile(std::vector<unsigned char> passphrase, const char *origin, const char *destination){
    std::vector<unsigned char> toDecrypt = Rxt::read(origin);
    std::vector<unsigned char> decrypted = Rxt::decipherBlockChainig(toDecrypt,passphrase,passphrase);
    Rxt::write(destination, decrypted);
    return 0;
}

int decryptFile(std::vector<unsigned char> passphrase, const char *origin, const char *destination, std::vector<unsigned char *> permutations){
    std::vector<unsigned char> toDecrypt = Rxt::read(origin);
    std::vector<unsigned char> decrypted = Rxt::cipherBlockChainig(toDecrypt,passphrase,passphrase);
    for (int i = 0; i < permutations.size(); i++){
        unsigned char a = permutations[i][0];
        unsigned char b = permutations[i][1];
        std::vector<long long> as = {};
        std::vector<long long> bs = {};
        for (int j = 0; j < decrypted.size(); j++) {
            if (decrypted[j] == a) as.push_back(j);
            else if (decrypted[j] == b) bs.push_back(j);
        }
        for (int j = 0; j <as.size(); j++){
            decrypted[as[j]]=b;
        }
        for (int j = 0; j < bs.size(); j++){
            decrypted[bs[j]]=a;
        }
    }
    
    Rxt::write(destination, decrypted);
    return 0;
}

std::vector<unsigned char> encryptMsg(std::vector<unsigned char> passphrase, std::vector<unsigned char> msg){
    std::vector<unsigned char>encrypted= Rxt::cipherBlockChainig(msg,passphrase,passphrase);
    std::vector<unsigned char> result = {};
    for (int i = 0; i < encrypted.size(); i++){
        unsigned char number = encrypted[i] & 0b11110000;
        number += 48;
        number = number < 58 ? number : number + 7;
        result.push_back(number);
        number = encrypted[i] & 0b00001111;
        number += 48;
        number = number < 58 ? number : number + 7;
        result.push_back(number);
    }
    return result;
}

std::vector<unsigned char> encryptMsg(std::vector<unsigned char> passphrase, std::vector<unsigned char> msg, std::vector<unsigned char *> permutations){
    for (int i = 0; i < permutations.size(); i++){
        unsigned char a = permutations[i][0];
        unsigned char b = permutations[i][1];
        std::vector<long long> as = {};
        std::vector<long long> bs = {};
        for (int j = 0; j < msg.size(); j++) {
            if (msg[j] == a) as.push_back(j);
            else if (msg[j] == b) bs.push_back(j);
        }
        for (int j = 0; j <as.size(); j++){
            msg[as[j]]=b;
        }
        for (int j = 0; j < bs.size(); j++){
            msg[bs[j]]=a;
        }
    }
    std::vector<unsigned char> encrypted = Rxt::cipherBlockChainig(msg,passphrase,passphrase);
    std::vector<unsigned char> result = {};
    for (int i = 0; i < encrypted.size(); i++){
        unsigned char number = encrypted[i] & 0b11110000;
        number += 48;
        number = number < 58 ? number : number + 7;
        result.push_back(number);
        number = encrypted[i] & 0b00001111;
        number += 48;
        number = number < 58 ? number : number + 7;
        result.push_back(number);
    }
    return result;
}

std::vector<unsigned char> decryptMsg(std::vector<unsigned char> passphrase, std::vector<unsigned char> msg){
    std::vector<unsigned char> toDecrypt = {};
    for (int i = 0; i < msg.size(); i+=2){
        unsigned char number1 = msg[i];
        number1 = number1 < 58 ? number1: number1 - 7;
        number1 -= 48;
        
        unsigned char number2 = msg[i];
        number2 = number2 < 58 ? number2: number2 - 7;
        number2 -= 48;
        toDecrypt.push_back((number1 << 4)|number2);
    }
    return Rxt::decipherBlockChainig(toDecrypt, passphrase, passphrase);
}

std::vector<unsigned char> decryptMsg(std::vector<unsigned char> passphrase, std::vector<unsigned char> msg, std::vector<unsigned char *> permutations){
    std::vector<unsigned char> toDecrypt = {};
    for (int i = 0; i < msg.size(); i+=2){
        unsigned char number1 = msg[i];
        number1 = number1 < 58 ? number1: number1 - 7;
        number1 -= 48;
        
        unsigned char number2 = msg[i];
        number2 = number2 < 58 ? number2: number2 - 7;
        number2 -= 48;
        toDecrypt.push_back((number1 << 4)|number2);
    }
    std::vector<unsigned char> decrypted = Rxt::cipherBlockChainig(toDecrypt,passphrase,passphrase);
    for (int i = 0; i < permutations.size(); i++){
        unsigned char a = permutations[i][0];
        unsigned char b = permutations[i][1];
        std::vector<long long> as = {};
        std::vector<long long> bs = {};
        for (int j = 0; j < decrypted.size(); j++) {
            if (decrypted[j] == a) as.push_back(j);
            else if (decrypted[j] == b) bs.push_back(j);
        }
        for (int j = 0; j <as.size(); j++){
            decrypted[as[j]]=b;
        }
        for (int j = 0; j < bs.size(); j++){
            decrypted[bs[j]]=a;
        }
    }
    
    return decrypted;
}

void help(lang language)
{//todo
}

void allong()
{
    std::cout << "choisissez la langue [Efrlatin]\n";
    std::string lang;
    std::cin >> lang;
    std::transform(lang.begin(), lang.end(), lang.begin(), [](unsigned char c){ return std::tolower(c); });
    bool msgEncryption = false;
    bool usePermutations = false;
    bool encrypt = true;
    char * origin;
     char * destination;
    std::vector<unsigned char> passphrase;
    std::vector<unsigned char> msg;
    std::vector<unsigned char*> permutations = {};


    std::string msgVsFile;
    std::string encryptVsDecrypt;
    std::string originMsg;
    std::string destinationMsg;
    std::string passphraseMsg;
    std::string msgMsg;
    if (lang=="french"||lang=="francais"||lang=="fr"||lang=="f"){
        msgVsFile = "voulez vous chiffrer un message [O:chiffrer un message /non:chiffrer un fichier]:\n";
        encryptVsDecrypt = "voulez vous chiffrer ou dechiffrer le message? [Cdechiffrer]: \n";
        originMsg ="quel est le lien du fichier original:\n";
        destinationMsg="quel est le lien du fichier final:\n";
        passphraseMsg="entrez le mot de passe:\n";
        msgMsg="entrez le message:\n";
    }else if (lang=="latin"||lang=="la"||lang=="l"){
        //TODO
    }else{
        msgVsFile = "do you want to encrypt a message [Y:encrypt message /no:encrypt file]:\n";
        encryptVsDecrypt = "do you want to encrypt or decrypt the message? [Edecrypt]: \n";
        originMsg ="what is the path of the original file:\n";
        destinationMsg="what is the path of the destination file:\n";
        passphraseMsg="enter your password/passphrase:\n";
        msgMsg="enter your message:\n";
    }
    
    std::string msgVsFileAns;
    std::cout << msgVsFile;
    std::cin >> msgVsFileAns;
    std::transform(msgVsFileAns.begin(), msgVsFileAns.end(), msgVsFileAns.begin(), [](unsigned char c){ return std::tolower(c); });
    msgEncryption = (msgVsFileAns=="y"||msgVsFileAns=="yes"||msgVsFileAns=="o"||msgVsFileAns=="oui"||msgVsFileAns=="i"||msgVsFileAns=="ita");

    std::string encryptVsDecryptAns;
    std::cout << encryptVsDecrypt;
    std::cin >> encryptVsDecryptAns;
    std::transform(encryptVsDecryptAns.begin(), encryptVsDecryptAns.end(), encryptVsDecryptAns.begin(), [](unsigned char c){ return std::tolower(c); });
    encrypt = (encryptVsDecryptAns=="e" ||encryptVsDecryptAns=="encrypt"||encryptVsDecryptAns=="c"||encryptVsDecryptAns=="chiffrer");

    std::string passphraseAns;
    std::cout << passphraseMsg;
    std::cin >> passphraseAns;
    for (int i = 0; i < passphraseAns.length(); i++) {
        passphrase.push_back(passphraseAns[i]);
    }
    
    if (msgEncryption){
        std::string  MsgAns;
        std::cout <<  msgMsg;
        std::cin >> MsgAns;
        for (int i = 0; i < MsgAns.length(); i++){
            msg.push_back(MsgAns[i]);
        }
        
        if (encrypt){
            std::vector<unsigned char> result = encryptMsg(passphrase, msg);
            std::cout << "\n";
            for (int i = 0; i < result.size(); i++){
                std::cout << result[i];
            }
            std::cout << "\n";
            return;
        }else{
            std::vector<unsigned char> result = decryptMsg(passphrase, msg);
            std::cout << "\n";
            for (int i = 0; i < result.size(); i++){
                std::cout << result[i];
            }
            std::cout << "\n";
            return;
        }
        
    }
    std::cout << originMsg;
    std::cin >> origin;
    std::cout << destinationMsg;
    std::cin >> destination;
    if (encrypt){ 
        encryptFile(passphrase, const_cast<char*>(origin), const_cast<char*>(destination));
        return;
    }else{
        decryptFile(passphrase, const_cast<char*>(origin), const_cast<char*>(destination));
        return;
    }
}
