#pragma once
#include "Log.hpp"
#include "Allocator.hpp"
#include "GLHeader.hpp"
#include "Platform.hpp"

#include <engine/renderer/Texture.hpp>
#include <engine/renderer/Shader.hpp>

#include <game/Game.hpp>
#include <game/Trailer.hpp>

#include <platform/Enum.hpp>

#include <Windows.h>
#include <vector>

namespace tc {
	typedef uint32 fileId;

	//ID of last lifetime file to be loaded
	//To be used when file system is transitioned to archive
	const fileId lastLifetimeFileID = 3;
	const uintptr pagesApproxStartupFilesSize = 1;
	
	struct File {
		uint8* data;
		fileId id;
		uint32 size;
	};
	struct FileManager {
		File* files;
		uint32 countFiles;
		
		File GetFile(fileId id);
	} fileManager;

	FileError ReadFileData(const char* name, uint8** dataBuffer, uint32* sizeOut) {
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
				*dataBuffer = allocator.Alloc(*sizeOut);    //TODO: allocate this elsewhere
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
	void LoadStartupFiles() {
		/*TODO: lifetime files will be loaded from the archive file.
		  Identified by ids 1..lastLifetimeFileID*/
		FileError e;
		Str startupFileNames[] = {
			Str("assets/shaders/main.vert"),
			Str("assets/shaders/main.frag"),
			Str("assets/sprites/toastcat.tga"),
			Str("assets/sprites/planetEarth.tga"), 
			Str("assets/sprites/planetJam.tga"),
			Str("assets/sprites/backgroundTrailer.tga"),
			Str("assets/sprites/animatedSpriteTest.tga"),
		};
		const uint32 countStartupFiles = sizeof(startupFileNames) / sizeof(startupFileNames[0]);
		File startupFiles[sizeof(startupFileNames) / sizeof(startupFileNames[0])];
		const uint32 firstSprite = 2;
		const uint32 countSprites = 4;
		Sprite* sprites[] = {
			&game.player.drawable.sprite,
			&trailer.earthPlanetSprite,
			&trailer.jamPlanetSprite,
			&trailer.world.background
		};
		float32 spriteScaleFactors[] = {
			320.0f,         //ToastCat
			150.0f,         //Earth planet
			120.0f,         //Jam planet
			3840.0f / 30.0f, //Trailer Background
			100.0f
		};

		//Read files from disk
		allocator.CommitMem(bytesPageSize * pagesApproxStartupFilesSize);
		for (uint32 fileIndex = 0; fileIndex < countStartupFiles; ++fileIndex) {
			if((FileError)0 != (e = ReadFileData(startupFileNames[fileIndex].str,
			                                     &startupFiles[fileIndex].data,
			                                     &startupFiles[fileIndex].size))) {
				log.WriteFileError(e, startupFileNames[fileIndex]);
				log.Write("[File IO] Crucial file could not be found.");
				ExitProgram();
			}
		}

		//Initialize various things from file data
		if ((ShaderInitError)0 != mainShader.Init(startupFiles[0].size,
			                                      (char*)startupFiles[0].data,
			                                      startupFiles[1].size,
			                                      (char*)startupFiles[1].data)) {
			log.Write("[Shaders] Shader compilation error, try redownloading game assets.");
			ExitProgram();
		}
		
		for (uint32 spriteIndex = firstSprite; spriteIndex < countSprites + firstSprite; ++spriteIndex) {
			auto spriteZeroIndex = spriteIndex - firstSprite;
			Sprite& sprite = *sprites[spriteZeroIndex];
			sprite.texture.Init();
			Texture::TextureFileReturn tfr = Texture::FillTextureFromFileData(sprite.texture, startupFiles[spriteIndex].data);
			if (!tfr.success) {
				log.Write("[File Error] Sprite ");
				log.Write(startupFileNames[spriteIndex]);
				log.Write(" is invalid, try redownloading game assets.");
				ExitProgram();
			}
			sprite.Init(spriteScaleFactors[spriteZeroIndex],
			            tfr.pxLenX,
			            tfr.pxLenY,
			            (void*)(((uintptr)bytesPlayerSpriteBufferOffset) + bytesSpriteVerticesSize * spriteZeroIndex));
		}

		trailer.animatedSprite.Init(2,
		                            1.0f / 1000.0f * mspt,
		                            spriteScaleFactors[4],
		                            100,
		                            100,
		                            (void*)(((uintptr)bytesPlayerSpriteBufferOffset) + bytesSpriteVerticesSize * 5));
		
		//Free unneeded file data (must do so in reverse order)
		for (int32 fileIndex = countStartupFiles - 1; fileIndex >= 0; --fileIndex) {
			allocator.Free(startupFiles[fileIndex].data);
		}

		allocator.CommitMem(bytesPageSize);
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
