/*
	Logs data to log.txt on error.
*/
#pragma once
#include "Enum.hpp"

#include <Windows.h>

namespace tc {
	struct Log {
		HANDLE logFile;
		//Opens the log file
		void Init() {
			logFile = CreateFile("log.txt",
			                     GENERIC_WRITE,
			                     FILE_SHARE_READ,
			                     nullptr,
			                     CREATE_ALWAYS,
			                     0,
			                     nullptr);
			/*If log failed to open it will be nullptr,
			  and won't be written too, so errors can be ignored*/
		}
		void Flush() {
			BOOL worked = FlushFileBuffers(logFile);
			assert(worked);
		}
		//Closes the log file
		void Cleanup() {
			CloseHandle(logFile);
		}
		void Write(const Str msg) {
			if (logFile != nullptr) {
				DWORD bytesWritten;
				WriteFile(logFile,
				          msg.str,
				          msg.len,
				          &bytesWritten,
				          nullptr);
			}
			Flush();
		}		
		void Write(char *str) {
			Write(Str(str));
		}
		void Write(uint32 num) {
			assert(*(int32*)(&num) >= 0);    //TODO: add support for negative numbers

			if (num == 0) {
				Write("0");
				return;
			}

			Str toWrite;
			char reverse[10];
			char forward[10];
			toWrite.len = 0;
			toWrite.str = reverse;

			while (num) {
				assert(toWrite.len < 10);
				toWrite.str[toWrite.len++] = num % 10;
				num /= 10;
			}

			for (uint32 startIndex = 0, endIndex = toWrite.len-1; startIndex < toWrite.len; startIndex++) {
				forward[endIndex] = reverse[startIndex];
				forward[endIndex] += '0';
				--endIndex;
			}
			toWrite.str = forward;
			
			Write(toWrite);
		}
		
		//Defined in FileIO.hpp
		void WriteFileError(const FileError error, const Str fileName);
	} log;
}
