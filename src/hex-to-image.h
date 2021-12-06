/**
 * Copyright (C) 2021 ToMe25.
 * This project is licensed under the MIT License.
 * The MIT license can be found in the project root and at https://opensource.org/licenses/MIT.
 */
#ifndef SRC_HEX_TO_IMAGE_H_
#define SRC_HEX_TO_IMAGE_H_

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
 * Prints the command help output to the standard output.
 */
void printHelp();

/**
 * Prints program info to the standard output.
 */
void printInfo();

#endif /* SRC_HEX_TO_IMAGE_H_ */
