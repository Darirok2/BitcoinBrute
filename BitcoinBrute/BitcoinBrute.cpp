﻿#include <ctime>
#include <random>
#include <string>
#include <openssl/evp.h>
#include <iostream>
#include "WordsEng.h"

class Mnemonic {
private:
    size_t _entropy_size;
    size_t _word_count;
    size_t _byte_str;
    std::string* _words;
    std::string _sha256_str;
    std::string _entropy;
    std::string _sha256_binary;
    unsigned char* _bytes;
    //seed
public:
    Mnemonic(size_t word_count) : _word_count(word_count), _entropy_size(word_count == 12 ? 128 : 256) {
        _words = new std::string[word_count];
        _entropy.reserve(_entropy_size);
        _sha256_str.reserve(64);
        _sha256_binary.reserve(256);
        GenerateEntropy();
        to_sha256();
        BinaryToStringWords();
    }
    void GenerateEntropy() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(0, 1);
        for (size_t i = 0; i < _entropy_size; i++) {
            _entropy += std::to_string(distr(gen));
        }
        //std::cout << "Entropy: " << _entropy << std::endl;
    }
    size_t BinaryToDecimal(const std::string& binary) {
        size_t decimal = 0;
        size_t power = 1;
        for (int i = binary.length() - 1; i >= 0; --i) {
            decimal += (binary[i] - '0') * power;
            power *= 2;
        }
        return decimal;
    }
    void BinaryToStringWords() {
        for (size_t i = 0; i < _word_count; ++i) {
            if (i == _word_count - 1) {
                std::string temp;
                for (size_t j = 0; j < 11 - (_entropy_size - (_word_count - 1) * 11); ++j) {
                    temp += _sha256_binary[j];
                }
                _words[i] = WordList[BinaryToDecimal(_entropy.substr(i * 11, (_entropy_size - (_word_count - 1) * 11)) + temp)];
            }
            else
                _words[i] = WordList[BinaryToDecimal(_entropy.substr(i * 11, 11))];
            //std::cout << _words[i] << " " << _entropy.substr(i * 11, 11) << std::endl;
        }
    }
    std::string HexToBinary(const std::string& hex) {
        std::string bin;
        for (char c : hex) {
            switch (c) {
            case '0':
                bin += "0000";
                break;
            case '1':
                bin += "0001";
                break;
            case '2':
                bin += "0010";
                break;
            case '3':
                bin += "0011";
                break;
            case '4':
                bin += "0100";
                break;
            case '5':
                bin += "0101";
                break;
            case '6':
                bin += "0110";
                break;
            case '7':
                bin += "0111";
                break;
            case '8':
                bin += "1000";
                break;
            case '9':
                bin += "1001";
                break;
            case 'A':
            case 'a':
                bin += "1010";
                break;
            case 'B':
            case 'b':
                bin += "1011";
                break;
            case 'C':
            case 'c':
                bin += "1100";
                break;
            case 'D':
            case 'd':
                bin += "1101";
                break;
            case 'E':
            case 'e':
                bin += "1110";
                break;
            case 'F':
            case 'f':
                bin += "1111";
                break;
            }
        }
        return bin;
    }
    void to_sha256() {
        // 1. Convert entropy string to byte array
        _byte_str = (_entropy_size) / 8;
        _bytes = new unsigned char[_byte_str];
        switch (_entropy_size) {
        case 128:
            for (int i = 0; i < _byte_str; ++i) {
                _bytes[i] = 0;
                for (int j = 0; j < 8; j++) {
                    _bytes[i] |= (_entropy[i * 8 + j] - '0') << (7 - j);
                }
            }
            break;
        case 256:
            for (int i = 0; i < _byte_str; ++i) {
                _bytes[i] = (_entropy[i * 8] - '0') * 128;
                _bytes[i] += (_entropy[i * 8 + 1] - '0') * 64;
                _bytes[i] += (_entropy[i * 8 + 2] - '0') * 32;
                _bytes[i] += (_entropy[i * 8 + 3] - '0') * 16;
                _bytes[i] += (_entropy[i * 8 + 4] - '0') * 8;
                _bytes[i] += (_entropy[i * 8 + 5] - '0') * 4;
                _bytes[i] += (_entropy[i * 8 + 6] - '0') * 2;
                _bytes[i] += (_entropy[i * 8 + 7] - '0');
            }
        default:
            break;
        }
        EVP_MD_CTX* mdctx = EVP_MD_CTX_create();
        EVP_DigestInit(mdctx, EVP_sha256());
        EVP_DigestUpdate(mdctx, _bytes, _byte_str);
        unsigned char digest[EVP_MAX_MD_SIZE];
        unsigned int digest_len;
        EVP_DigestFinal(mdctx, digest, &digest_len);
        EVP_MD_CTX_destroy(mdctx);
        _sha256_str = "";
        for (int i = 0; i < digest_len; ++i) {
            char hex_str[3];
            snprintf(hex_str, sizeof(hex_str), "%02x", digest[i]);
            _sha256_str += hex_str;
            _sha256_binary += HexToBinary(hex_str);
        }
        //std::cout << "SHA256: " << _sha256_str << std::endl;
        //std::cout << "SHA256: " << _sha256_binary << std::endl;
        delete[] _bytes;
    }
    std::string GetString() {
        std::string temp = _words[0];
        for (size_t i = 1; i < _word_count; i++) {
            temp += " ";
            temp += _words[i];
        }
        return temp;
    }

    ~Mnemonic() {
        delete[] _words;
    }
};
int main() {
    clock_t start = clock();
    for (size_t i = 0; i < 50000; ++i) {
        Mnemonic gen(12);
        //std::cout << gen.GetString() << std::endl;
    }
    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    printf("The time: %f seconds\n", seconds);
    return 0;
}