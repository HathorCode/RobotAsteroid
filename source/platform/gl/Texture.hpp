/*
  Defines a 2D texture loaded from a tga.
*/

#pragma once
#include "GLProcPtrs.hpp"

namespace tc {
#ifdef _MSC_VER
#pragma pack(push)
#pragma pack(1)
#else
#error Compiler needs pack definition
#endif
	struct tgaHeader {
		uint8	idLength;
		uint8	colorMapType;
		uint8	dataTypeCode;
		uint16	colorMapOrigin;
		uint16	colorMapLength;
		uint8	colorMapDepth;
		uint16	xOrigin;
		uint16	yOrigin;
		uint16	width;
		uint16	height;
		uint8	bitsPerPixel;
		uint8	imageDescriptor;
	};
#ifdef _MSC_VER
#pragma pack(pop)
#endif
	struct Texture {
		GLuint id;
#ifdef _DEBUG
		bool freed = false;
#endif

		struct TextureFileReturn {
			bool success;
			uint32 pxLenX;
			uint32 pxLenY;
		};
		static TextureFileReturn FillTextureFromFileData(Texture initedOut, const uint8* fileData) {
			TextureFileReturn ret = {};
			
			//Validate tgaHeader at top of file
			const tgaHeader* fileHeader = (tgaHeader*) fileData;
			ret.pxLenX = fileHeader->width;
			ret.pxLenY = fileHeader->height;
			if (!Texture::ValidateTexture((tgaHeader*)fileData)) {
				return ret;
			}
			fileData += 18;	//Skip past 18 byte header

			//Read pixel data
			initedOut.CreateFromPixels(fileHeader->width, fileHeader->height, (uint32*)fileData);
			ret.success = true;
			return ret;
		}
		static TextureFileReturn FillTextureArrayFromFileData(uint32 count, Texture initedOut, const uint8* fileDatas) {
			TextureFileReturn ret = {};
			
			//Validate tgaHeader at top of file
			const tgaHeader* fileHeader = (tgaHeader*) fileDatas;
			ret.pxLenX = fileHeader->width;
			ret.pxLenY = fileHeader->height;
			if (!Texture::ValidateTexture((tgaHeader*)fileDatas)) {
				return ret;
			}
			
			fileDatas += 18;	//Skip past 18 byte header; we don't need to memmove the top fileData

			//Read pixel data
			initedOut.CreateArrayFromPixels(count, fileHeader->width, fileHeader->height, (uint32*)fileDatas);
			ret.success = true;
			return ret;
		}
		static bool ValidateTexture(const tgaHeader* header) {
			if (header->idLength != 0) {
				return false;
			}
			if (header->colorMapType != 0) {
				return false;
			}
			//2 is uncompressed RGB
			if (header->dataTypeCode != 2) {
				return false;
			}
			if (header->width == 0) {
				return false;
			}
			if (header->height == 0) {
				return false;
			}
			if (header->bitsPerPixel != 32) {
				return false;
			}
			const uint8 bit03ValidImageDescriptor = (uint8) 8;
			const uint8 bit4ValidImageDescriptor = (uint8)  ~(1 << 4);	//All 1's except bit 4
			const uint8 bit5ValidImageDescriptor = (uint8)  ~(1 << 5);
			const uint8 bit67ValidImageDescriptor = (uint8) ~(3 << 6);	//Ones everywhere except bits 6 and 7
			const uint8 validImageDescriptor = bit03ValidImageDescriptor & bit4ValidImageDescriptor & bit5ValidImageDescriptor & bit67ValidImageDescriptor;
			if (header->imageDescriptor != validImageDescriptor) {
				return false;
			}
			return true;
		}

		//Take advantage of bulk texture creation and deletion
		static inline void CreateTextures(const GLuint countTextures, Texture* const out) {
			if (debug) {
				for (uint32 i = 0; i < countTextures; ++i) {
					glGenTextures(1, &out[i].id);
					assert(out[i].id != 0);
				}
			} else {
				glGenTextures(countTextures, (GLuint*)out);
			}
		}
		static inline void DeleteTextures(const GLuint amount, Texture* const toDelete) {
			if (debug) {
				for (uint32 i = 0; i < amount; ++i) {
					if (!toDelete[i].freed) {
						glDeleteTextures(1, &toDelete[i].id);
						toDelete[i].freed = true;
					} else {
						log.Write("Warning: redundent free ignored.\n");
					}
				}
			} else {
				glDeleteTextures(amount, (GLuint*)toDelete);
			}
		}

		
		void CreateArrayFromPixels(const uint32 count, const uint32 width, const uint32 height, const uint32* pixels) {
			glBindTexture(GL_TEXTURE_2D_ARRAY, this->id);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTexImage3D(GL_TEXTURE_2D_ARRAY,
			             0,
			             GL_RGBA8,
			             (GLsizei)width,
			             (GLsizei)height,
			             (GLsizei)count,
			             0,
			             GL_BGRA,
			             GL_UNSIGNED_BYTE,
			             (GLvoid*)pixels);
		}
		void CreateFromPixels(const uint32 width, const uint32 height, const uint32* pixels) {
			//Create an opengl texture object
			glBindTexture(GL_TEXTURE_2D, this->id);
		
			//Set parameters for linear filtering (we don't have mipmaps at the moment)
			//TODO: option for nearest filtering
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			//Give the texture data
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
		}
		void Init() {
			CreateTextures(1, this);
		}
		void Cleanup() {
			if (debug) {
				if (!freed) {
					glDeleteTextures(1, &this->id);
					freed = true;
				} else {
					log.Write("Warning: redundent free ignored.\n");
				}
			} else {
				glDeleteTextures(1, &this->id);
			}
		}
		GLint GetWidth() const {
			GLint width;
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
			return width;
		}
		GLint GetHeight() const {
			GLint height;
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &height);
			return height;
		}		

		void Bind() const {
			glBindTexture(GL_TEXTURE_2D, id);
		}
		bool IsBound() const {
			GLuint bound;
			glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&bound);
			return bound == id;
		}
	};
}
