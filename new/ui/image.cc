#include "defines.h"

#include <cstdio>
using namespace std;

#include <png.h>

Image* 
Image::LoadImage(string filename)
{
	// find directory
	string fullpath = "./data/" + filename; // TODO

	// find out file type
	if(!filename.compare(filename.length() - 4, 4, ".png")) {
		return LoadPNG(fullpath);
	} else {
		throw ui_error("Unsupported format of " + filename);
	}
}


Image*
Image::LoadPNG(string filename)
{
	Image* image;

	// open file
	FILE *f = fopen(filename.c_str(), "rb");
	if(!f) {
		throw ui_error("Error opening " + filename + ".");
	}

	// read PNG header
	uint8_t sig[8];
	(void) fread(sig, 1, 8, f);
	if(!png_check_sig(sig, 8)) {
		throw ui_error(filename + ": not a valid PNG file.");
	}

	// prepare
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
			NULL, NULL, NULL);
	if(!png_ptr) {
		throw ui_error("Error loading " + filename + ".");
	}
	png_infop info_ptr = png_create_info_struct(png_ptr);

	// handle errors
	if(setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		throw ui_error("something went wrong while reading " + 
				filename);
	}

	// read PNG file info
	int color_type, w, h;
	png_init_io(png_ptr, f);
	png_set_sig_bytes(png_ptr, 8);
	png_read_info(png_ptr, info_ptr);
	h = png_get_image_height(png_ptr, info_ptr);
	w = png_get_image_width(png_ptr, info_ptr);
	//bitdepth = png_get_bit_depth(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);
	
	// create Image
	if(color_type == PNG_COLOR_TYPE_PALETTE) {
		image = new ImageIndexed(w, h);
	} else {
		throw ui_error("Image type is currently unsupported: " +
				filename);
	}

	// get palette
	int num_trans = 0;
	png_bytep trans_alpha = NULL;
	int n_col = 0;

	if(color_type == PNG_COLOR_TYPE_PALETTE)
	{
		// read palette
		png_color* p;
		png_get_PLTE(png_ptr, info_ptr, &p, &n_col);

		// copy colors
		for(int i=0; i<n_col; i++) {
			((ImageIndexed*)image)->SetPaletteColor(i, palette[i]);
		}

		// get transparent color
		if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
			png_get_tRNS(png_ptr, info_ptr, &trans_alpha, 
					&num_trans, NULL);
			if(num_trans > 0) {
				((ImageIndexed*)image)->SetTransparentIndex(
					trans_alpha[0]);
			}
		}
	}
	

	return image;
}
