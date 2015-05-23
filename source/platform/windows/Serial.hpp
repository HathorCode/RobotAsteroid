#pragma once

#include <Windows.h>

namespace robitRabit {
	struct Serial {
		DCB dcb;
		HANDLE dataHandle;

		void Init() {
			dataHandle = CreateFile("COM3", GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
			
			dcb.DCBlength = sizeof(DCB);
			dcb.BaudRate = CBR_9600;
			dcb.ByteSize = 8;
			dcb.Parity = NOPARITY;
			dcb.StopBits = ONESTOPBIT;
		}

		void SendData() {
			if (!SetCommState(dataHandle, &dcb)) {

			}
		}
	};
}