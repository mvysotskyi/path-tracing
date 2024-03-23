#pragma once

class TGAReader {
private:
	using BYTE = unsigned char;
	using WORD = unsigned short;
	using DWORD = unsigned __int32;
	
private:
	struct _TgaHeader
	{
		TGAReader::BYTE IDLength;        /* 00h  Size of Image ID field */
		TGAReader::BYTE ColorMapType;    /* 01h  Color map type */
		TGAReader::BYTE ImageType;       /* 02h  Image type code */
		TGAReader::WORD CMapStart;       /* 03h  Color map origin */
		TGAReader::WORD CMapLength;      /* 05h  Color map length */
		TGAReader::BYTE CMapDepth;       /* 07h  Depth of color map entries */
		TGAReader::WORD XOffset;         /* 08h  X origin of image */
		TGAReader::WORD YOffset;         /* 0Ah  Y origin of image */
		TGAReader::WORD Width;           /* 0Ch  Width of image */
		TGAReader::WORD Height;          /* 0Eh  Height of image */
		TGAReader::BYTE PixelDepth;      /* 10h  Image pixel size */
		TGAReader::BYTE ImageDescriptor; /* 11h  Image descriptor byte */
	};

	DWORD tga_header_sz = 18;

private:
	const char* filepath;
	BYTE* pixels_data;

	BYTE pixel_depth;
	WORD width;
	WORD height;

public:
	TGAReader(const char* _filepath);
	~TGAReader();

	void read();
	BYTE* get_pixels_data() const;
	WORD get_width() const;
	WORD get_height() const;
	bool has_alpha_chanel() const;
};