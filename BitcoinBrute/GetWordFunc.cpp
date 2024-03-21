#include <random>
#include <iostream>
#include <string>
#include "WordsEng.h"


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

std::string GenerateMnemonic(size_t words_count = 12) {
	int size = 0;
	switch (words_count)
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
	std::string* entropy_words = new std::string[words_count];
	for (size_t i = 0; i < words_count; ++i)
	{
		size_t byte_words = 11 * i;
		std::cout << std::endl << byte_words << " ";
		if (i == words_count - 1)
			entropy_words[i] = entropy.substr(byte_words, 7);
		else
			entropy_words[i] = WordsALL::WordList[BinaryToDecimal(entropy.substr(byte_words, 11))];
		std::cout << entropy_words[i] << " " << std::endl;
	}
	return "OK";
}