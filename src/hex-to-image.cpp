/**
 * Copyright (C) 2021 ToMe25.
 * This project is licensed under the MIT License.
 * The MIT license can be found in the project root and at https://opensource.org/licenses/MIT.
 */
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

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

void printHelp() {
	cout << "Hex to image Help" << endl;
	cout << "=================" << endl;
	cout << "Hex to image is a simple program to convert text files with hex codes to images." << endl;
	cout << "The Source code can be found at https://www.github.com/ToMe25/hex-to-image/" << endl;
	cout << "Report issues here: https://www.github.com/ToMe25/hex-to-image/issues/" << endl << endl;
	cout << "argument   |short  |default            |explanation" << endl;
	cout << "-----------|-------|-------------------|-----------" << endl;
	cout << "--help     |       |                   |Prints this help text and then quits." << endl;
	cout << "--info     |       |                   |Prints some info about this program and then quits." << endl;
	cout << "--input    |-i     |hex-input.txt      |Sets the file with the hex values to convert to an image." << endl;
	cout << "--width    |-w     |0                  |Sets the width of the output image. Set to 0 to use the length of the lines in the input file." << endl;
	cout << "--height   |-h     |0                  |Sets the height of the output image. Set to 0 to use the line count from the input file." << endl;
	cout << "--output   |-o     |hex-to-image.png   |Sets the location of the output image." << endl;
	cout << "--override |       |                   |Makes this program override the output file. If not specified it will append a number instead if the output already exists." << endl;
	cout << "--digits   |-d     |6                  |Sets the number of hex digits to use for each pixel. Valid values are 3, 4, 6, and 8. If the values is 4 or 8 the output image will include transparency." << endl;
}

void printInfo() {
	cout << "Hex to image is a simple program to convert text files with hex codes to images." << endl;
	cout << "Copyright (C) 2021 ToMe25." << endl;
	cout << "License: MIT License(https://opensource.org/licenses/MIT)." << endl;
	cout << "This is free software: you are free to change and redistribute it." << endl;
	cout << "There is NO WARRANTY, to the extent permitted by law." << endl;
}

int main(int argc, char **argv) {
	string input_file = "hex-input.txt";
	string output_file = "hex-to-image.png";
	uint8_t hex_length = 6;

	uint16_t width = 0;
	uint16_t height = 0;

	bool override_output = false;

	Commands command = Commands::Null;
	for (int i = 0; i < argc; i++) {
		string arg = string(argv[i]);

		while (arg[0] == ' ') {
			cout << "Space" << endl;
			arg = arg.substr(1);
		}

		if (command == Commands::Null && arg[0] == '-') {
			while (arg[0] == '-') {
				arg = arg.substr(1);
			}

			if (arg == "help") {
				printHelp();
				return 0;
			} else if (arg == "info") {
				printInfo();
				return 0;
			} else if (arg == "input" || arg == "i") {
				command = Commands::Input;
			} else if (arg == "width" || arg == "w") {
				command = Commands::Width;
			} else if (arg == "height" || arg == "h") {
				command = Commands::Height;
			} else if (arg == "output" || arg == "o") {
				command = Commands::Output;
			} else if (arg == "override") {
				override_output = true;
			} else if (arg == "digits" || arg == "d") {
				command = Commands::Digits;
			}

		} else if (command != Commands::Null) {
			switch (command) {
			case Commands::Input:
				input_file = arg;
				break;
			case Commands::Width:
				width = stoi(arg);
				break;
			case Commands::Height:
				height = stoi(arg);
				break;
			case Commands::Output:
				output_file = arg;
				break;
			case Commands::Digits:
				hex_length = stoi(arg);
				break;
			default:
				cerr << "Found invalid command value "
						<< static_cast<std::underlying_type<Commands>::type>(command)
						<< "!\nThis should be impossible." << endl;
				return 1;
			}
			command = Commands::Null;
		}
	}

	if (hex_length % 3 != 0 && hex_length % 4 != 0) {
		cerr << "Invalid digits argument!" << endl;
		cerr << "The number of digits has to be divisible by 3 or 4." << endl;
		return 1;
	}

	if (hex_length > 8) {
		cout << "This program currently does not support images with more then 4 channels!" << endl;
		hex_length = 8;
	}

	uint8_t channels = 3;
	if (hex_length % 4 == 0) {
		channels = 4;
	}

	if (output_file.find(".png") != string::npos) {
		output_file = output_file.substr(0, output_file.find(".png"));
	}

	if (output_file.find(".PNG") != string::npos) {
		output_file = output_file.substr(0, output_file.find(".PNG"));
	}

	ifstream hex_input(input_file);

	if (hex_input.is_open()) {
		string line;
		uint16_t max_len = 0;

		vector<string> lines;
		while (getline(hex_input, line)) {
			lines.push_back(line);
			if (line.length() > max_len) {
				max_len = line.length();
			}
		}

		hex_input.close();

		bool ignore_linebreaks = true;
		double pixels_per_line = double(max_len) / hex_length;
		if (width == 0) {
			if (height == 0) {
				width = ceil(pixels_per_line);
				ignore_linebreaks = false;
				height = lines.size();
			} else {
				width = ceil(double(lines.size()) / height * pixels_per_line);
			}
		}

		if (height == 0) {
			height = ceil(lines.size() / (width / pixels_per_line));
		}

		printf("Output image size: %d x %d pixels.\n", width, height);

		uint8_t *pixel_colors = new uint8_t[width * height * channels];

		for (uint32_t i = 0; i < width * height * channels; i++) {
			pixel_colors[i] = 0;
		}

		string temp;
		string color;
		istringstream stream;
		uint32_t array_pos = 0;
		uint32_t parsed;
		for (size_t y = 0; y < lines.size(); y++) {
			line = lines[y];

			if (ignore_linebreaks && temp.length() > 0) {
				line = temp + line;
				temp = "";
			}

			while (line.length() > 0) {
				for (uint16_t x = 0; x < width; x++) {
					if (line.length() < uint32_t((x + 1) * hex_length)) {
						temp = line.substr(x * hex_length, line.length() - x * hex_length);
						line = "";
						break;
					}

					if (ignore_linebreaks) {
						if (y != 0) {
							array_pos += channels;
						}
					} else {
						array_pos = y * width * channels + x * channels;
					}

					color = line.substr(x * hex_length, hex_length);
					if (hex_length <= 4) {
						color.resize(hex_length * 2);
						for (int8_t i = hex_length - 1; i >= 0; i--) {
							color[i * 2 + 1] = color[i * 2] = color[i];
						}
					}

					stream.str(color);
					stream >> hex >> parsed;

					if (stream.rdbuf()->in_avail() > 0) {
						cerr << line.substr(x * hex_length, hex_length) << " can't be parsed as hex input!" << endl;
					} else {
						pixel_colors[array_pos] = (parsed >> 16) & 0xFF;
						pixel_colors[array_pos + 1] = (parsed >> 8) & 0xFF;
						pixel_colors[array_pos + 2] = parsed & 0xFF;
						if (channels == 4) {
							pixel_colors[array_pos + 3] = (parsed >> 24) & 0xFF;
						}
					}

					stream.clear();
				}

				if (line.length() > width * hex_length) {
					line = line.substr(width * hex_length, line.length() - width * hex_length);
				} else {
					line = "";
				}
			}
		}

		if (FILE *file = fopen(string(output_file).append(".png").c_str(), "r")) {
			fclose(file);
			if (override_output) {
				output_file.append(".png");
			} else {
				uint16_t file_nr = 1;
				while (FILE *file = fopen(
						string(output_file).append("-").append(
								to_string(file_nr)).append(".png").c_str(), "r")) {
					fclose(file);
					file_nr++;
					if (file_nr == 0) {
						cerr
								<< "Couldn't write image to file, all possible file names already exist."
								<< endl;
						delete pixel_colors;
						return 1;
					}
				}

				output_file.append("-").append(to_string(file_nr)).append(
						".png");
			}
		} else {
			output_file.append(".png");
		}

		stbi_write_png(output_file.c_str(), width, height, channels, pixel_colors, width * channels);
		cout << "Image saved as \"" << output_file << "\"." << endl;
		delete pixel_colors;

		return 0;
	} else {
		printf("Can't open input file %s.\n", input_file.c_str());
		return 1;
	}
}
