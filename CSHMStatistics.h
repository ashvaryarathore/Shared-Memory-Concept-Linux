#pragma once

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#define ST_BUFFER 4


/**********************************************
Name : StructureCounter
Purpose : The structure will store the counters
          for read and write operations.
***********************************************/
typedef struct STStructureCounter
{
        unsigned int unWrittenPackets;
        unsigned int unReadPackets;
	unsigned int unPacketsInMemory;
	
	STStructureCounter()
	{
		printf("\n\n\n\nCalled Constructor..... Initializing counters.\n");
		//Initialize memory with the structure pointer.
        	unWrittenPackets = 0;
        	unReadPackets = 0;
        	unPacketsInMemory = 0;
		printf("\n\n\nConstructors Initialized.... Values are: ");
		printf("\n unWrittenPackets: %d\n unReadPackets: %d\n unPacketsInMemory: %d\n",unWrittenPackets,unReadPackets,unPacketsInMemory);
	}
}*pSTStructureCounter;



/******************************************************
Class Name: CSHMStatistics
Purpose : The class will have the code to write the
          contents on the SHM.
*******************************************************/
class CSHMStatistics{


        public:

        /********************************************************
        Function Name: DumpSHMStatistics
        Purpose: The function will have the code to read 
                 data of  the SHMStatictics and write  on a file.
        Return Type: void.
        Parameters: key_t key_tSHMStructureCounter
        *********************************************************/
        void DumpSHMStatistics(key_t key_tSHMStructureCounter);


};//End of class.

