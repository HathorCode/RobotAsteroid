/*
	Defines a font class for printing text.
*/

#pragma once

#define maxWorkaround(a,b) (((a) > (b)) ? (a) : (b))

#include <algorithm>
#include <platform\windows\Log.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H


namespace robitRabit {
	struct Font {
		//Freetype initialization
		static void InitFreeType() {
			//TODO: integrate with allocator
			FT_Error e = FT_Init_FreeType(&ftLibrary);
			assert(!e);
		}

		//Creates an atlas from the given font file.
		uint32 Init(const char* pathToFont, const uint32 pxFontSize) {
			assert(pxFontSize > 0);
			assert(pathToFont);

			FT_Face ftFont;
			FT_Error error;
			error = FT_New_Face(ftLibrary, pathToFont, 0, &ftFont);
			if (error) { return error; }

			error = FT_Set_Pixel_Sizes(ftFont, 0, pxFontSize);
			if (error) { return error; }

			FT_GlyphSlot glyphSlot = ftFont->glyph;

			uint32 width = 0;
			uint32 height = 0;
			uint32 atlasWidth;

			uint32 cmin = 0x20; //Minimum character glyph value to load
			uint32 cmax = 0x81; //Maximum character glyph value to load
			for (int charIterator = cmin; charIterator < cmax; charIterator++) {
				if (FT_Load_Char(ftFont, charIterator, FT_LOAD_RENDER)) {
					log.Write("[FreeType2] Character that should be acessible can't be loaded");
				}

				width += glyphSlot->bitmap.width;
				height = maxWorkaround(height, glyphSlot->bitmap.rows);

				atlasWidth = width;
			}
		}

	private:
		static FT_Library ftLibrary;
		GLuint atlas;
	};

	FT_Library Font::ftLibrary;
}
