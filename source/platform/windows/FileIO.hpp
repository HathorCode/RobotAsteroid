#pragma once
#include "Log.hpp"
#include "Allocator.hpp"
#include "GLHeader.hpp"
#include "Platform.hpp"

#include <engine/renderer/Texture.hpp>
#include <engine/renderer/Shader.hpp>

#include <app/App.hpp>

#include <platform/Enum.hpp>

#include <Windows.h>
#include <vector>

namespace robitRabit {
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
				*dataBuffer = alloc(*sizeOut);    //TODO: allocate this elsewhere
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
