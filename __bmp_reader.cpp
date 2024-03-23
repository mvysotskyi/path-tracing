class BMPTexture {
private:
	std::ifstream* ifile;

	const char* file_name;
	size_t bpp;
	size_t color_planes;

	size_t size_x;
	size_t size_y;

	char* data;
	size_t data_size;

private:
	uint16_t format_hex = 0x4d42; // B(0x42), M(0x4d)

	size_t image_data_offset = 54;
	size_t size_x_offset = 18;
	size_t size_y_offset = 22;
	size_t image_bpp_offset = 28;

private:
	bool check_format(uint16_t* format_buffer);
	void inverse_color_mode();

public:
	BMPTexture(const char* file_name, size_t bpp, size_t color_planes);
	~BMPTexture();

	void load_data();
	const char* texture_data() const;

	size_t width() const;
	size_t height() const;
};





BMPTexture::BMPTexture(const char* file_name, size_t bpp, size_t color_planes) {
	this->file_name = file_name;
	this->bpp = bpp;
	this->color_planes = color_planes;
	this->ifile = nullptr;
}

BMPTexture::~BMPTexture() {
	if (ifile != nullptr) {
		delete ifile;
	}
}

bool BMPTexture::check_format(uint16_t* format_buffer) {
	if (*format_buffer == format_hex) return true;
	return false;
}

void BMPTexture::inverse_color_mode() {
	char temp;

	for (size_t i = 0; i < data_size; i += 3) {
		temp = data[i];
		data[i] = data[i + 2];
		data[i + 2] = temp;
	}
}

void BMPTexture::load_data() {
	ifile = new std::ifstream(file_name, std::ifstream::binary);

	uint16_t format_buffer[1];
	uint16_t bpp_buffer[1];
	uint16_t color_planes_buffer[1];

	if (!*ifile) {
		std::cerr << file_name << ": cannon open file\n";
		return;
	}

	ifile->read((char*)format_buffer, sizeof(*format_buffer)); // check file format
	if (!check_format(format_buffer)) {
		std::cerr << file_name << ": not a .bmp file\n";
		ifile->close();
		return;
	}

	ifile->seekg(image_bpp_offset, ifile->beg); // check color depth
	ifile->read((char*)bpp_buffer, sizeof(*bpp_buffer));
	if (*bpp_buffer != bpp) {
		std::cerr << file_name << ": invalid color depth(must be " << bpp << ")\n";
		ifile->close();
		return;
	}

	ifile->seekg(size_x_offset, ifile->beg);
	ifile->read((char*)& size_x, sizeof(size_x)); // read image width
	ifile->read((char*)& size_y, sizeof(size_y)); // read image height

	ifile->read((char*)& color_planes_buffer, sizeof(*color_planes_buffer)); // read color planes count
	if (*color_planes_buffer != color_planes) {
		std::cerr << file_name << ": invalid color planes count(must be " << color_planes << ")\n";
		ifile->close();
		return;
	}

	ifile->seekg(image_data_offset, ifile->beg);

	data_size = size_x * size_y * (bpp >> 3);
	data = new char[data_size];
	ifile->read(data, data_size);

	inverse_color_mode(); // convert from BGR to RGB

	ifile->close();
}

const char* BMPTexture::texture_data() const {
	return data;
}

size_t BMPTexture::width() const {
	return size_x;
}

size_t BMPTexture::height() const {
	return size_y;
}