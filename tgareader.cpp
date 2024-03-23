#include "tgareader.h"

#include <iostream>
#include <fstream>

TGAReader::TGAReader(const char* _filepath) {
	filepath = _filepath;
	pixels_data = nullptr;

	pixel_depth = 0;
	width = 0;
	height = 0;
}

TGAReader::~TGAReader() {
	
}

void TGAReader::read() {
	std::ifstream file = std::ifstream(filepath, std::ios::binary);

	if (!file.good()) {
		file.close();
		std::cerr << "Cannot open file: " << filepath << std::endl;
		return;
	}

	BYTE id_size;
	file.read((char*)&id_size, sizeof(id_size)); // read id string size

	file.seekg(0x0c, std::ios::beg);
	file.read((char*)&width, sizeof(width)); // read width
	file.read((char*)&height, sizeof(height)); // read height

	file.seekg(0x10, std::ios::beg);
	file.read((char*)&pixel_depth, sizeof(pixel_depth)); // read pixel depth

	file.seekg(tga_header_sz + id_size, std::ios::beg);

	DWORD data_sz = width * height * (pixel_depth / 8);
	pixels_data = new BYTE[data_sz];
	
	for (int x = 0; x < data_sz; x += (pixel_depth / 8)) {
		if (has_alpha_chanel()) {
			BYTE bgra[4];
			file.read((char*)&bgra, 4);

			pixels_data[x] = bgra[2];
			pixels_data[x + 1] = bgra[1];
			pixels_data[x + 2] = bgra[0];
			pixels_data[x + 3] = bgra[3];
		}
		else {
			BYTE bgr[3];
			file.read((char*)&bgr, 3);

			pixels_data[x] = bgr[2];
			pixels_data[x + 1] = bgr[1];
			pixels_data[x + 2] = bgr[0];
		}
	}
}

TGAReader::BYTE* TGAReader::get_pixels_data() const {
	return pixels_data;
}

TGAReader::WORD TGAReader::get_width() const {
	return width;
}

TGAReader::WORD TGAReader::get_height() const {
	return height;
}

bool TGAReader::has_alpha_chanel() const {
	return pixel_depth == 32;
}