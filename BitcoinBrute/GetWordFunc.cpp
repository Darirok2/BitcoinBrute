#include <random>
#include <iostream>
#include <string>
#include "WordsEng.h"
#include "sha256.h"
#include <bitset>

void StringToChar(const std::string& entropy_str, char*& entropy) {
	std::cout << "Entropy: ";
	for (size_t i = 0; i < entropy_str.length(); ++i) {
		entropy[i] = entropy_str[i];
		std::cout << entropy[i];
	}
	std::cout << std::endl;
}

size_t BinaryToDecimal(std::string binaryString) {
	size_t decimalNumber = 0;
	size_t power = 1;

	for (int i = binaryString.length() - 1; i >= 0; --i) {
		if (binaryString[i] == '1') {
			decimalNumber += power;
		}
		power *= 2;
	}
	//std::cout << "Десятичное число: " << decimalNumber << std::endl;

	return decimalNumber;
}

std::string GenerateEntropy(size_t size = 128) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(0, 1);
	std::string entropy;
	for (size_t i = 0; i < size; i++) {
		entropy += std::to_string(distr(gen));
	}
	return entropy;
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

std::string GetBinaryHash(std::string entropy_str) {
	int count = entropy_str.length();
	//std::cout << entropy_str << " " << count << std::endl;
	char* entropy = new char[count];


	std::string byteString = "0x"+entropy_str;
	/*std::uint32_t bytes1 = std::stoll(byteString, 0, 16);
	std::cout << "Bytes_HEX: " << bytes1 << std::endl;*/


	StringToChar(entropy_str, entropy);
	SHA256 buff;
	buff.update(entropy, count);
	std::cout << "BUFF: " << SHA256::hashString(entropy) << std::endl;
	std::cout << "HEX SHA256: " << buff.hash() << std::endl;
	std::string binary_entropysha256 = HexToBinary(buff.hash());
	std::cout << "BIANRY SHA256: " << binary_entropysha256 << std::endl;
	return binary_entropysha256;
}

std::string BinaryToStringWords(size_t *words_count) {
	int size = 0;
	switch (*words_count)
	{
	case 12:
		size = 128;
		break;
	case 18:
		size = 192;
		break;
	case 24:
		size = 256;
		break;
	default:
		break;
	}
	std::string entropy = GenerateEntropy(size);
	std::string entropy_words;
	for (size_t i = 0; i < *words_count; ++i)
	{
		size_t byte_words = 11 * i;
		//std::cout << std::endl << byte_words << " ";
		if (i == *words_count - 1) {
			std::string binary_hash = GetBinaryHash(entropy);
			entropy_words += WordsALL::WordList[BinaryToDecimal(entropy.substr(byte_words, 7) + binary_hash[0] + binary_hash[1] + binary_hash[2] + binary_hash[3])];
		}
		else
			//std::cout << entropy.substr(byte_words, 11) << " ";
			entropy_words += WordsALL::WordList[BinaryToDecimal(entropy.substr(byte_words, 11))];
		entropy_words += " ";
	}
	std::cout << entropy_words << " " << std::endl;
	//std::cout << entropy << std::endl;
	//std::cout << WordsALL::WordList[0] << std::endl;
	return "OK";
}

std::string GenerateMnemonic(size_t *words_count) {
	BinaryToStringWords(words_count);
	return "OK";
}