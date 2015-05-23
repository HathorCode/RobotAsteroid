#pragma once

#include <Windows.h>
#include <assert.h>
#include <vector>

namespace robitRabit {
	struct Serial {
		DCB dcb;
		DWORD bytesWritten;
		HANDLE dataHandle;

		void Init() {
			dataHandle = CreateFile("COM3", GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
			
			dcb.DCBlength = sizeof(DCB);
			dcb.BaudRate = CBR_9600;
			dcb.ByteSize = 8;
			dcb.Parity = NOPARITY;
			dcb.StopBits = ONESTOPBIT;
			//assert(!SetCommState(dataHandle, &dcb));
		}

		inline void SendData(char* dataToSend) {
			bool writtenFile = WriteFile(dataHandle, dataToSend, 6, &bytesWritten, NULL);
		}

		/* Should be the only function used to send instructions to the Carduino. Input the array of characters from the generated path*/
		void SendInstructionSet(std::vector<char*> instructionVector) {
			bool bufferSize = WriteFile(dataHandle, (LPCVOID)instructionVector.size(), sizeof(instructionVector.size()), &bytesWritten, NULL);
			for (int indexOfBuffer = 0; indexOfBuffer < instructionVector.size(); indexOfBuffer++) {
				SendData(instructionVector[indexOfBuffer]);
			}
		}

		void Close() {
			CloseHandle(dataHandle);
		}
	} serial;
}