#pragma once
#include "Log.hpp"
#include "Allocator.hpp"
#include "GLHeader.hpp"
#include "Platform.hpp"

#include <engine/renderer/Texture.hpp>
#include <engine/renderer/Shader.hpp>

#include <app/App.hpp>
#include <app/AssetManager.hpp>

#include <platform/Enum.hpp>

#include <Windows.h>
#include <vector>

namespace robitRabit {
	struct File {
		uint8* data;
		uint64 size;
	};
	
	FileError ReadFileData(const char* name, File& f) {
		uint8** dataBuffer = &f.data;
		uint64* sizeOut = &f.size;
		HANDLE file = CreateFile(name,			//File name
		                         GENERIC_READ,  //Read only
		                         0,             //unused
		                         nullptr,		//unused
		                         OPEN_EXISTING, //Fail to open if file does not already exist
		                         0,				//unused
		                         nullptr);  	//unused
		if (file != INVALID_HANDLE_VALUE) {
			*sizeOut = GetFileSize(file, nullptr);
			if (*sizeOut != 0) {
				*dataBuffer = Alloc(*sizeOut);    //TODO: allocate this elsewhere
				if (dataBuffer) {
					uint32 bytesRead;
					if (ReadFile(file, *dataBuffer, *sizeOut, (DWORD*)&bytesRead, nullptr) && bytesRead == *sizeOut) {
						return (FileError)noError;
					} else {
						return FE_readError;
					}
				} else {
					ExitProgram();
					return (FileError)noError;
				}
			} else {
				return FE_fileEmpty;
			}
		} else {
			DWORD lastError = GetLastError();
			if (lastError == ERROR_FILE_NOT_FOUND || lastError == ERROR_PATH_NOT_FOUND) {
				return FE_fileNotFound;
			} else if (lastError == ERROR_ACCESS_DENIED) {
				return FE_accessDenied;
			} else {
				return FE_unknownError;
			}
		}
	}
	void LoadFiles() {
		FileError e;
		Str fileNames[] = {
			Str("assets/main.vert"),
			Str("assets/main.frag"),
			Str("assets/hotsinglesinyourarea.tga"),
			Str("assets/free_Viagra.tga"),
			Str("assets/KnifeSalesmanship.jpg.tga"),
		};
		const uint32 countFiles = sizeof(fileNames) / sizeof(fileNames[0]);
		File files[sizeof(fileNames) / sizeof(fileNames[0])];   //Because god forbid C++ allow using constants in array sizes.

		//Read files from disk
		for (uint32 fileIndex = 0; fileIndex < countFiles; ++fileIndex) {
			if ((FileError)0 != (e = ReadFileData(fileNames[fileIndex].str,
			                                      files[fileIndex]))) {
				log.WriteFileError(e, fileNames[fileIndex]);
				log.Write("[File IO] Crucial file could not be found.");
				ExitProgram();
			}
		}

		//Initialize various things from file data
		if ((ShaderInitError)0 != mainShader.Init(files[0].size,
			                                      (char*)files[0].data,
			                                      files[1].size,
			                                      (char*)files[1].data)) {
			log.Write("[Shaders] Shader compilation error, try redownloading game assets.");
			ExitProgram();
		}

		bool resInd[] = {
			false,
			true,
			false
		};
		
		float32 spriteScaleFactors[] = {
			1920,
			AssetManager::pxObsResX,
			1
		};
		uint32 countSprites = sizeof(spriteScaleFactors) / sizeof(spriteScaleFactors[0]);
		uint32 firstSprite = 2;
		Sprite* sprites[] = {
			&assets.background,
			&assets.obs,
			&assets.sidebar.sprite, //Sidebar is a drawable not a sprite
		};
		assert(countSprites == sizeof(sprites) / sizeof(sprites[0]));
		assert(countSprites == sizeof(resInd) / sizeof(resInd[0]));
		for (uint32 spriteIndex = firstSprite; spriteIndex < countSprites + firstSprite; ++spriteIndex) {
			auto spriteZeroIndex = spriteIndex - firstSprite;
			Sprite& sprite = *sprites[spriteZeroIndex];
			sprite.texture.Init();
			Texture::TextureFileReturn tfr = Texture::FillTextureFromFileData(sprite.texture, files[spriteIndex].data);
			if (!tfr.success) {
				log.Write("[File Error] Sprite ");
				log.Write(fileNames[spriteIndex]);
				log.Write(" is invalid, try redownloading game assets.");
				ExitProgram();
			}
			if (!resInd[spriteZeroIndex]) {
				sprite.Init(spriteScaleFactors[spriteZeroIndex],
							tfr.pxLenX,
							tfr.pxLenY,
							(void*)(((uintptr)bytesPlayerSpriteBufferOffset) + bytesSpriteVerticesSize * spriteZeroIndex));
			} else {
				sprite.InitResIndependent((void*)(((uintptr)bytesPlayerSpriteBufferOffset) + bytesSpriteVerticesSize * spriteZeroIndex));
			}
		}
		
		for (uint32 fileIndex = 0; fileIndex < countFiles; ++fileIndex) {
			Free(files[fileIndex].data);
		}
	}

	void Log::WriteFileError(const FileError error, const Str fileName) {
		switch (error) {
		case FE_accessDenied:
			Write("Access denied opening file ");
			Write(fileName);
			Write("\n");
			break;
		case FE_fileEmpty:
			Write("File ");
			Write(fileName);
			Write(" is empty.\n");
			break;
		case FE_fileNotFound:
			Write("File ");
			Write(fileName);
			Write(" not found.\n");				
			break;
		case FE_readError:
			Write("Read error in file ");
			Write(fileName);
			Write("\n");
			break;
		case FE_unknownError:
			Write("Unknown error using file ");
			Write(fileName);
			Write("\n");
			break;
		default:
			assert(false);
		}
		Flush();
	}
}
