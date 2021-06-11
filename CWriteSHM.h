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
#include "CQueue.h"
#include "CSHMStatistics.h"
#include <pcap.h>
#define SHM_SIZE 16640
#define CHUNK_SIZE 1664
#define CHUNK_NUMBER 10
#define ST_BUFFER 4


/******************************************************
Class Name: CWrite
Purpose : The class will have the code to write the
	  contents on the SHM.
*******************************************************/
class CWriteSHM{


	public:

	static int nChunkCounter;
	static int nPacketCounter;
	static int nInitializeCounter;	
	/********************************************************
	Function Name: WriteSHM
	Purpose: The function will have the code to write 
		 data on the SHM.
	Return Type: void.
	Parameters: key_t key_tSHM, key_t key_tSHMStructureCounter,
	char *pchPacketAddr	
	*********************************************************/
	void WriteSHM(key_t key_tSHM, key_t key_tSHMStructureCounter,char *pchPacketAddr);


	/********************************************************
	Function Name: InitializeWriteSHM
	Purpose: The function will have the code to create the
         SHM.
	Return Type: void.
	Parameters: key_t key_tSHM, key_t key_tSHMStructureCounter
	*********************************************************/
	void InitializeWriteSHM(key_t key_tSHM, key_t key_tSHMStructureCounter);


};//End of class.

