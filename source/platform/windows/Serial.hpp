#pragma once

#include <Windows.h>
#include <assert.h>
#include <vector>

#include <platform\windows\Log.hpp>

namespace robitRabit {
	struct Serial {
		DCB dcb;
		DWORD bytesWritten;
		HANDLE dataHandle;

		void Init() {
			dataHandle = CreateFile("COM3", GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, nullptr);
			
			dcb.DCBlength = sizeof(DCB);
			dcb.BaudRate = CBR_9600;
			dcb.ByteSize = 8;
			dcb.Parity = NOPARITY;
			dcb.StopBits = ONESTOPBIT;
			if (!SetCommState(dataHandle, &dcb)) {
				log.Write("[SerialCommunications] The Communications state could not be initialized properly.\n");
			}
		}

		inline void SendData(char* dataToSend) {
			WriteFile(dataHandle, dataToSend, 6, &bytesWritten, nullptr);
		}

		/* Should be the only function used to send instructions to the Carduino. Input the array of characters from the generated path*/
		void SendInstructionSet(std::vector<char*> instructionVector) {
			for (int indexOfBuffer = 0; indexOfBuffer < instructionVector.size(); indexOfBuffer++) {
				SendData(instructionVector[indexOfBuffer]);
			}
		}

		void Close() {
			CloseHandle(dataHandle);
		}
	} serial;
}