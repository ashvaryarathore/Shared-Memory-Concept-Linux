#include "CReadSHM.h"
#include "CSHMStatistics.h"
#include "CParsePackets.h"
#include "CQueue.h"

#define MAX_PACKET_LENGTH 1664


/********************************************************
Function Name: ReadSHM
Purpose: The function will have the code to write 
                data on the SHM.
Return Type: void.
Parameters: key_t key_tSHM, key_t key_tSHMStructureCounter
*********************************************************/
void CReadSHM::ReadSHM(key_t key_tSHM, key_t key_tSHMStructureCounter)
{
	int nPacketCounter = 0;

	//Variables to hold Data to be written on shared memory.
        char chArrStructureCounter[ST_BUFFER];
        unsigned short usLengthSharedMemoryData;

        //Variables to store the SHMid.
        int nSHMid;
        int nSHMidStructureCounter;

        //Variables for the SHM size.
        char chArray[SHM_SIZE];
        size_t size_tSHMSize = sizeof(chArray);
        size_t size_tSHMStructureCounter = sizeof(STStructureCounter);

        //Variables to hold the SHM Address.
        char *pchSHMAddress;
        char *pchSHMAddressStructureCounter;
        char *pchSHMBaseAddress;

	/***********************************************************************
        Code to assign the Shared memory for the reading.
        ***********************************************************************/
        nSHMid = shmget(key_tSHM, size_tSHMSize, S_IWOTH);

        //Loop to wait until successfully getting the SHM.
        while (nSHMid < 0)
        {
                printf("\nFailed to get shm, failed in : shmget in CReadSHM.cpp");
                nSHMid = shmget(key_tSHM, size_tSHMSize, S_IWOTH);
        }//While closed.

        //Get the address of the SHM.
        pchSHMBaseAddress = (char*) shmat(nSHMid, NULL, 0);
        //Store the base address.
        pchSHMAddress = pchSHMBaseAddress;

        //Calculate last address.
        char *pchLastAddress = pchSHMAddress + SHM_SIZE;


        /*********************************************************************
        Code to assign the Shared memory for the structure pointer.
        **********************************************************************/
        nSHMidStructureCounter = shmget(key_tSHMStructureCounter,size_tSHMStructureCounter,S_IWOTH);

        //Loop to wait until successfully getting the SHM.
        while (nSHMidStructureCounter < 0)
        {
                printf("failed to create shm segment\n");
                nSHMidStructureCounter = shmget(key_tSHMStructureCounter,size_tSHMStructureCounter,S_IWOTH);

        }

        //Structure object 
        STStructureCounter *pSTStructureCounter;
        pSTStructureCounter = (STStructureCounter*)(shmat(nSHMidStructureCounter, NULL, 0));


	//Variable to keep track of words read from shared memory.
        int nWordsRead = 0;
        int nChunkCounter = 0;
	char chValue;
        char chValueData;
	int nLoopVariable = 1000;
	

        //Resetting loop variable.
        nLoopVariable = 0;

	

	/***********************************************************************
        Code to read from the SHM.
        ************************************************************************/
        //Loop will run until all the words are written on shared memory.
	
	while(1)
        {

		//Increment the loop variable.
		nLoopVariable++;
		
 		//Check if the end of shared memory is reached. 
                if( nChunkCounter == CHUNK_NUMBER)
                {
                        //Reset counter to assign start address again.
                        nChunkCounter = 0;

                }//if closed.
		
                //Assigning pointer the address of chunk.
                pchSHMAddress = pchSHMBaseAddress +(CHUNK_SIZE *(nChunkCounter));

                //assigning the value of data at shared memory in the variable.                 
                chValue = *pchSHMAddress;
                
		//check if the value is 'r'( for read.)
                if(chValue == 'r')
                {
			
			//Increment packet counter.
			nPacketCounter = nPacketCounter +1;

			//buffer to hold the packet data.
			char *pchPacketData;

			//Store the flag address.
                	char *pchFlagAddress = pchSHMAddress;
			
                       	//Increment the pointer.
			pchSHMAddress = pchSHMAddress + 1;


			//Obtaining the length of the data.
			unsigned short usDataLength = (*((unsigned short*)pchSHMAddress));

			//increment pointer to next position.
                        pchSHMAddress = pchSHMAddress + 2;
			

			//Assign memory to buffer.
        		pchPacketData =(char *) malloc(usDataLength*sizeof(char));
			
			//Assigning to the memory.
			memcpy(pchPacketData,pchSHMAddress,usDataLength);
			
			//Increase the read counter.
			pSTStructureCounter->unReadPackets = pSTStructureCounter->unReadPackets + 1;
			
			//Decrease packets in memory.
                        pSTStructureCounter->unPacketsInMemory = pSTStructureCounter->unPacketsInMemory - 1;

			/***************************************
			Code to send the packet to parsing.
			***************************************/
			//Object creation for the CParsePackets class, to call its method.
                        CParsePackets objCParsePackets;
                        //method call using the CParsePackets class.
                        objCParsePackets.ParsePacketData((const char*)pchPacketData);
		
                        //Set flag pointer to d to indicate memory is free to write.
                        memset(pchFlagAddress,'d',1);
			
			//*(pchFlagAddress) = 'd';
			
			//Increment the chunk address to next location.
                        nChunkCounter = nChunkCounter + 1;

                }//if case closed.

                //Reset word count.
                nWordsRead = 0;
		
        }//while closed.

        
}//End of function ReadSHM.
                          
