/*
	Allocates memory for the game on a stack allocator.
	This will be used for storing files and data for the levels.
*/
#pragma once
#include "Platform.hpp"
#include "Options.hpp"

#include <cstdio>

namespace tc {

	struct AllocatorType {		
		//Empty functions kept for future combination with old allocator
		void Init() {
		}
		void CommitMem(intptr newAmount) {
		}

		uint8* Alloc(intptr amount) {
			assert(amount > 0);
			return (uint8*)malloc(amount);
		}
		uint8* Realloc(uint8* bytes, intptr newAmount) {
			assert(newAmount > 0);
			return (uint8*)realloc((void*)bytes, newAmount);
		}
		void Free(uint8* bytes) {
			free((void*)bytes);
		}

		//Special cases for last stretches of memory to be implemented
	} allocator;
	/*This is the old allocator; it is stack based and will be incorporated into the current allocator
	  when there is time for optimisation.*/
#if 0
	struct AllocatorType {
		static const intptr countExtraPages = 2;	//Number of pages extra to commit when we run out of commited memory
		intptr memLen = 0;
		uint8* mem;                                 //The entire games memory
		uint8* nextFree;                            //Next free chunk of memory

		void Init() {
			mem = (uint8*)VirtualAlloc(nullptr,
			                           options.bytesMaxMem,
			                           MEM_RESERVE,
			                           PAGE_READWRITE);
			//TODO: fail to initialize and allow options to reduce bytesMaxMem
			assert(mem != nullptr);
			nextFree = mem;
		}
		//Commit the actual memory to RAM
		void CommitMem(intptr newAmount) {
			assert(newAmount > 0);

			//Sizes must be divisible by page size
			assert(memLen % bytesPageSize == 0);
			assert(newAmount % bytesPageSize == 0);
			
			if (newAmount <= options.bytesMaxMem) {
				if (memLen > newAmount) {                       //Need less
					intptr bytesToFree = memLen - newAmount;
					uint8* memEnd = mem + memLen;
					uint8* freePoint = memEnd - bytesToFree;
					BOOL worked = VirtualFree(freePoint,
					                          bytesToFree,
					                          MEM_DECOMMIT);    //We never release pages, only decommit
					assert(worked);
					memLen = newAmount;
				} else {                                        //Need more
					intptr newMemLen = newAmount;
					uint8* newMem = (uint8*)VirtualAlloc(mem + memLen,
					                                     newMemLen - memLen,
					                                     MEM_COMMIT,
					                                     PAGE_READWRITE);
					assert(newMem == mem + memLen);
					memLen = newMemLen;
				}
			} else {
				log.Write("Out of memory, tried to allocate ");
				char newAmountStr[11];
				sprintf_s<11>(newAmountStr, "%ul", newAmount);
				log.Write(Str(newAmountStr));
				log.Write(" bytes of memory. GetLastError reports:");
				sprintf_s<11>(newAmountStr, "%d", GetLastError());
				log.Write(Str(newAmountStr));
				log.Write("\nTry increasing the max memory option.");
				ExitProgram();
			}
		}

		/*Stores the size of the allocation for debugging in two intptrs above and below the memory
		  Only called in debug mode*/
		inline uint8* DebugMakeTrackedSizeAlloc(intptr memSize, uint8* alloc) {
			assert(memSize > 0);
			//Header
			uint8* actualMem = alloc + sizeof(intptr);
			*((intptr*)alloc) = memSize;
			
			//Footer
			uint8* memEnd = actualMem + memSize;
			*((intptr*)memEnd) = memSize;
			
			return actualMem;
		}
		//This gets the size in the header
		inline intptr DebugGetTrackedSizeOfAlloc(uint8* mem) {
			return *(intptr*)(mem - sizeof(intptr));
		}
		//This gets the size in the footer for the last allocation
		inline intptr DebugGetTrackedSizeOfLastAlloc() {
			return *(intptr*)(nextFree - sizeof(intptr));
		}
		//This gets the size in the header
		inline void DebugSetTrackedSizeOfAlloc(uint8* mem, intptr newSize) {
			assert(newSize > 0);
			*(intptr*)(mem - sizeof(intptr)) = newSize;
		}
		//This gets the size in the footer for the last allocation
		inline void DebugSetTrackedSizeOfLastAlloc(intptr newSize) {
			assert(newSize > 0);
			*(intptr*)(nextFree - sizeof(intptr)) = newSize;
		}

		void CommitMemToNextFree() {
			intptr allocSize = (intptr)(nextFree - mem);
			allocSize += (bytesPageSize - allocSize % bytesPageSize);   //Round up to page
			allocSize += countExtraPages * bytesPageSize;               //Commit extra pages, so we don't have to do it too often
			CommitMem(allocSize);                                       /* Optimize: since we know this will have to commit extra memory
			                                                               make it not go through the branches which check for it*/
		}
		/*Retrieve pointers to the allocated memory
		  This memory is only freed when memory is entirely recommited*/
		uint8* Alloc(intptr amount) {
			uint8* toReturn = nextFree;
			nextFree += amount;
			nextFree += debug ? sizeof(intptr) * 2 : 0;    //Account for tracked length
			uint8* memEnd = mem + memLen;

			//See if we need to reserve more memory
			if (nextFree > memEnd) {
				CommitMemToNextFree();
			}
			if (debug) {
				return DebugMakeTrackedSizeAlloc(amount, toReturn);
			} else {
				return toReturn;
			}
		}
		//Resize the last allocated block of memory to be oldSize + extraSize
		inline void ExpandLastMem(intptr extraSize) {
			if (debug) {
				intptr oldSize = DebugGetTrackedSizeOfLastAlloc();
				intptr newSize = oldSize + extraSize;
				nextFree += extraSize;
				uint8* memEnd = mem + memLen;
				if (nextFree > memEnd) {
					CommitMemToNextFree();
				}
				
				//Set footer
				DebugSetTrackedSizeOfLastAlloc(newSize);
				//And header
				DebugSetTrackedSizeOfAlloc(nextFree - newSize - sizeof(intptr), newSize);
			} else {
				Alloc(extraSize);
			}
		}
		inline void TruncLastMem(intptr lostSize) {
			if (debug) {
				assert(lostSize > 0);
				intptr oldSize = DebugGetTrackedSizeOfLastAlloc();
				intptr newSize = oldSize - lostSize;
				assert(newSize > 0);

				//Set footer
				DebugSetTrackedSizeOfLastAlloc(newSize);
				//And header
				DebugSetTrackedSizeOfAlloc(nextFree - newSize - sizeof(intptr), newSize);				
			} else {
				FreeLastMem(lostSize);
			}
		}
		
		//Free the last allocated block of memory which must be memSize
		inline void FreeLastMem(intptr memSize) {
			if (debug){
				assert(DebugGetTrackedSizeOfLastAlloc() == memSize);
				nextFree -= memSize;
				assert(nextFree >= mem);           //Assert we didn't free more than we allocated
				nextFree -= sizeof(intptr)*2; //Account for tracked length
			} else {
				Alloc(-memSize);
			}
		}
	} allocator;

#ifdef _DEBUG
	namespace test {
		struct AllocatorTest {
			AllocatorTest() {
				//TODO: test expandLastMem and trunclastmem
				SYSTEM_INFO si;
				GetSystemInfo(&si);
				bytesPageSize = si.dwPageSize;
				bytesAllocGran = si.dwAllocationGranularity;
				Options::Init();

				AllocatorType testAllocator;
				testAllocator.Init();
				testAllocator.CommitMem(bytesPageSize);
				
				uint8* mem = testAllocator.Alloc(64);
				assert(testAllocator.DebugGetTrackedSizeOfAlloc(mem) == 64);
				assert(testAllocator.DebugGetTrackedSizeOfLastAlloc() == 64);

				uint8* mem1 = testAllocator.Alloc(32);
				assert((uint8*)(mem + 64 + sizeof(intptr)*2) == mem1);
				assert(testAllocator.DebugGetTrackedSizeOfLastAlloc() == 32);

				testAllocator.Alloc(bytesPageSize*3);
				assert(testAllocator.memLen == bytesPageSize*4 + bytesPageSize*AllocatorType::countExtraPages);
				testAllocator.FreeLastMem(bytesPageSize*3);
				assert(testAllocator.DebugGetTrackedSizeOfLastAlloc() == 32);

				testAllocator.FreeLastMem(32);
				testAllocator.FreeLastMem(64);
				assert(testAllocator.nextFree == testAllocator.mem);

				testAllocator.CommitMem(bytesPageSize * 10);
				assert(testAllocator.memLen == bytesPageSize * 10);

				testAllocator.CommitMem(bytesPageSize);
				assert(testAllocator.memLen == bytesPageSize);
			}
		} allocatorTest;
	}
#endif
#endif
}
