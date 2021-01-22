/**
 * Copyright (C) 2021 ToMe25.
 * This project is licensed under the MIT License.
 * The MIT license can be found in the project root and at https://opensource.org/licenses/MIT.
 */
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <sstream>
#include <cmath>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char **argv) {
	string input_file = "hex-input.txt";
	uint hex_length = 6;

	uint width = 0;
	uint height = 10;

	uint channels = 3;
	if (hex_length % 4 == 0) {
		channels = 4;
	}

	input_file = (fs::current_path() / input_file).string();
	ifstream hex_input(input_file);

	if (hex_input.is_open()) {
		string line;
		uint max_len = 0;

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

		cout << "Output image size: " << width << " x " << height << " pixels" << endl;

		uint8_t *pixel_colors = new uint8_t[width * height * channels];

		for (uint i = 0; i < width * height * channels; i++) {
			pixel_colors[i] = 0;
		}

		string temp;
		istringstream stream;
		uint array_pos = 0;
		uint parsed;
		for (uint y = 0; y < lines.size(); y++) {
			string &line = lines[y];

			if (ignore_linebreaks && temp.length() > 0) {
				line = temp + line;
				temp = "";
			}

			while (line.length() > 0) {
				for (uint x = 0; x < width; x++) {
					if (line.length() < (x + 1) * hex_length) {
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

					stream.str(line.substr(x * hex_length, hex_length));
					stream >> hex >> parsed;

					if (stream.rdbuf()->in_avail() > 0) {
						cerr << line.substr(x * hex_length, hex_length) << " can't be parsed as hex input!" << endl;
					} else {
						pixel_colors[array_pos] = (parsed >> 16) & 0xFF;
						pixel_colors[array_pos + 1] = (parsed >> 8) & 0xFF;
						pixel_colors[array_pos + 2] = parsed & 0xFF;
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

		if (FILE *file = fopen("hex-to-image.png", "r")) {
			fclose(file);
			uint16_t file_nr = 1;
			while (FILE *file = fopen(string("hex-to-image-").append(to_string(file_nr)).append(".png").c_str(), "r")) {
				fclose(file);
				file_nr++;
				if (file_nr == 0) {
					cerr << "Couldn't write image to file, all possible file names already exist." << endl;
					delete pixel_colors;
					return 2;
				}
			}

			stbi_write_png(string("hex-to-image-").append(to_string(file_nr)).append(".png").c_str(),
					width, height, channels, pixel_colors, width * channels);
			printf("Image saved as \"hex-to-image-%d.png\".\n", file_nr);
		} else {
			stbi_write_png("hex-to-image.png", width, height, channels, pixel_colors, width * channels);
			cout << "Image saved as \"hex-to-image.png\"." << endl;
		}
		delete pixel_colors;

		return 0;
	} else {
		printf("Can't open input file %s.\n", input_file.c_str());
		return 1;
	}
}
