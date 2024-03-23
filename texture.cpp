#include "texture.h"

PNGImage::PNGImage(const char* file_path) {
	this->file_path = file_path;
	this->i_data = nullptr;
}

PNGImage::~PNGImage() {
	delete[] i_data;
}

void PNGImage::read() {
	FILE* png_fp = NULL;
	fopen_s(&png_fp, file_path, "rb");
	if (!png_fp) {
		std::cerr << "Cannot open " << file_path << std::endl;
		return;
	}

	png_bytep header_buffer = new png_byte[header_size];
	fread_s(header_buffer, header_size, 1, header_size, png_fp);

	if (png_sig_cmp(header_buffer, 0, header_size)) {
		std::cout << "Invalid signature\n";
		return;
	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		png_error(png_ptr, "Can not create read struct");
		return;
	};

	png_infop png_info_ptr = png_create_info_struct(png_ptr);
	if (!png_info_ptr) {
		png_error(png_ptr, "Can not create info struct");
		return;
	}

	png_init_io(png_ptr, png_fp);
	png_set_sig_bytes(png_ptr, header_size);

	png_read_info(png_ptr, png_info_ptr);

	i_width = png_get_image_width(png_ptr, png_info_ptr);
	i_height = png_get_image_height(png_ptr, png_info_ptr);

	// Unused parameters                                             // LOOK FOR ME
	//char bit_depth = png_get_bit_depth(png_ptr, png_info_ptr);
	//int number_of_passes = png_set_interlace_handling(png_ptr);

	char color_type = png_get_color_type(png_ptr, png_info_ptr);
	i_color_format = (PNG_COLOR_TYPE_RGB == color_type) ? (GL_RGB) : (GL_RGBA);

	png_read_update_info(png_ptr, png_info_ptr);

	size_t row_size = png_get_rowbytes(png_ptr, png_info_ptr);
	i_data = new unsigned char[i_height * row_size];
	unsigned char** row_pointers = new unsigned char*[i_height];

	for (size_t y = 0; y < i_height; y++) row_pointers[y] = &i_data[y * row_size];
	png_read_image(png_ptr, row_pointers);

	fclose(png_fp);
	png_destroy_read_struct(&png_ptr, &png_info_ptr, NULL);
}

const unsigned char* PNGImage::texture_data() const { return i_data; }

GLenum PNGImage::color_format() const { return i_color_format; };

size_t PNGImage::width() const { return i_width; }

size_t PNGImage::height() const { return i_height; }


Texture::Texture(const char* texture_resource_path) {
	this->texture_resource_path = texture_resource_path;
	this->texture_id = 0;
	this->texture_resource = nullptr;
	this->tga_res = nullptr;
}

Texture::~Texture() {
	delete texture_resource;
}

void Texture::make_texture() {
	if (tga_res == nullptr) {
		if (strlen(texture_resource_path) == 0) {
			texture_id = 0;
			return;
		}

		//texture_resource = new PNGImage(texture_resource_path);
		//texture_resource->read();

		tga_res = new TGAReader(texture_resource_path);
		tga_res->read();

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &texture_id);

		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, 3, tga_res->get_width(), tga_res->get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, tga_res->get_pixels_data());
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		
		// Need to be tested
		if (tga_res) delete[] tga_res->get_pixels_data();
		if(texture_resource) delete texture_resource;
	}
}