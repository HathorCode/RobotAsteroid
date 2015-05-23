#pragma once

#include <cstdio>
#include <iostream>
#include <vector>

#include "platform\windows\Log.hpp"

#include <AL\al.h>
#include <AL\alc.h>
#include <AL\alext.h>

#include <ogg\ogg.h>
#include <vorbis\vorbisfile.h>

#define BUFFER_SIZE 32768

namespace tc {
	void ExitProgram();

	struct AudioTrack {
		ALint state;
		ALuint bufferID;
		ALuint sourceID;
		ALenum format;
		ALsizei freq;
		std::vector<char> bufferData;

		void InitAL() {
			ALCdevice* device;
			ALCcontext* context;

			device = alcOpenDevice(NULL);
			if (!device) {
				log.Write("[OpenAL] Could not open a device.\n");
			}

			context = alcCreateContext(device, NULL);
			alcMakeContextCurrent(context);
			if (context == NULL || alcMakeContextCurrent(context) == ALC_FALSE) {
				if (context != NULL) {
					alcDestroyContext(context);
					alcCloseDevice(device);
					log.Write("[OpenAL] Could not set a context.\n");
				}
			}
		}

		// Call for any audio source
		void PlayFile(char*fileName) {
			alGenBuffers(1, &bufferID);
			alGenSources(1, &sourceID);

			LoadOGG(fileName, bufferData, format, freq);

			alBufferData(bufferID, format, &bufferData[0], static_cast<ALsizei>(bufferData.size()), freq);
			alSourceQueueBuffers(sourceID, 1, &bufferID);
			alSourcePlay(sourceID);
		}

		void CloseAL() {
			alDeleteBuffers(1, &bufferID);
			alDeleteSources(1, &sourceID);

			ALCdevice* device;
			ALCcontext* context;

			context = alcGetCurrentContext();
			if (context == NULL) {
				return;
			}

			device = alcGetContextsDevice(context);

			alcMakeContextCurrent(NULL);
			alcDestroyContext(context);
			alcCloseDevice(device);
		}

		void LoadOGG(char* fileName, std::vector<char> &buffer, ALenum &format, ALsizei &freq) {
			int endian = 0;
			int bitStream;
			long bytes;
			char array[BUFFER_SIZE];
			FILE *f;

			f = fopen(fileName, "rb");
			if (f == nullptr) {
				log.Write("[File Error] Audio file not found.\n");
				log.Write("Crucial game file not found.");
				ExitProgram();
			}

			vorbis_info *pInfo;
			OggVorbis_File oggFile;
			ov_open(f, &oggFile, NULL, 0);

			pInfo = ov_info(&oggFile, -1);
			if (pInfo == nullptr) {
				log.Write("[File Error] Audio file info cannot be loaded from Audio File.");
				log.Write("Crucial game file not found.");
				ExitProgram();
			}

			if (pInfo->channels == 1) {
				format = AL_FORMAT_MONO16;
			}
			else {
				format = AL_FORMAT_STEREO16;
			}

			freq = pInfo->rate;

			do {
				bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);
				buffer.insert(buffer.end(), array, array + bytes);
			} while (bytes > 0);
			ov_clear(&oggFile);
		}
	}audio;
}