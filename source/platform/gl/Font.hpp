/*
	Defines a font class for printing text.
*/

#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

namespace robitRabit {
	struct Font {
		//Creates an atlas from the given font file.
		uint32 Init(const uint8* fontFileData, const uintptr fileDataSize, const uint32 pxFontSize) {
			assert(pxFontSize > 0);
			assert(fontFileData);
			assert(fileDataSize > 0);
			FT_Face ftFont;
			FT_Error e;
			uint32 cmin = 0x21; //Minimum character glyph value to load
			uint32 cmax = 0x7e; //Maximum character glyph value to load
			e = FT_New_Memory_Face(ftLibrary,
			                       fontFileData,
			                       fileDataSize,
			                       0,		  
			                       &ftFont);
			if (e) {return e;}
			e = FT_Set_Pixel_Sizes(ftFont,
			                       0,	
			                       pxFontSize);
			if (e) {return e;}
			
		}

		//Freetype initialization
		static void InitFreeType() {
			//TODO: integrate with allocator
			FT_Error e = FT_Init_FreeType(&ftLibrary);
			assert(!e);
		}
	private:
		static FT_Library ftLibrary;
		GLuint atlas;
	};

	FT_Library Font::ftLibrary;
}
