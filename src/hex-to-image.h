/**
 * Copyright (C) 2021 ToMe25.
 * This project is licensed under the MIT License.
 * The MIT license can be found in the project root and at https://opensource.org/licenses/MIT.
 */
#ifndef SRC_HEX_TO_IMAGE_H_
#define SRC_HEX_TO_IMAGE_H_

#include <string>
#include <vector>

enum class Commands {
	Null,
	Help,
	Info,
	Input,
	Width,
	Height,
	Output,
	Override,
	Digits
};

int main(const int argc, char **argv);

/**
 * Converts the content of the input to pixel colors.
 *
 * @param lines		One std::string per line of input hex codes.
 * @param hex_len	The number of hexadecimal digits per pixel.
 * @param width		The width of the output image in pixels. Will be modified depending on the input.
 * @param height	The height of the output image in pixels. Will be modified depending on the input.
 * @return	An uint8_t array containing the color codes. One value per channel per pixel.
 */
uint8_t *convert(const std::vector<std::string> lines, const uint8_t hex_len, uint16_t &width, uint16_t &height);

/**
 * Prints the command help output to the standard output.
 */
void printHelp();

/**
 * Prints program info to the standard output.
 */
void printInfo();

#endif /* SRC_HEX_TO_IMAGE_H_ */
