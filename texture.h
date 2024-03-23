#pragma once

#include <Windows.h>
#include <gl/GL.h> 
#include "glm/GLM/glm.hpp"

#include "tgareader.h"

#include <png.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>

class PNGImage {
private:
	const char* file_path;
	size_t i_width = 0;
	size_t i_height = 0;

	unsigned char* i_data;
	GLenum i_color_format = 0; 

private:
	const size_t header_size = 8;

public:
	PNGImage(const char* file_path);
	~PNGImage();

	void read();
	const unsigned char* texture_data() const;

	GLenum color_format() const;
	size_t width() const;
	size_t height() const;
};

class Texture {
public:
	unsigned int texture_id;

private:
	const char* texture_resource_path;
	PNGImage* texture_resource;
	TGAReader* tga_res;

public:
	Texture(const char* texture_file_name);
	~Texture();

	void set_texture_file_name(const char* texture_file_name);
	void make_texture();
};