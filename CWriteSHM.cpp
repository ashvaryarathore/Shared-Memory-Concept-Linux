#include "CWriteSHM.h"

#define MAX_PACKET_LENGTH 1664
#define ETHERNET_HEADER_LEN 14
#define IP_FIRST_TWO_BYTES 2

//Initialize static variable.
int CWriteSHM::nChunkCounter = 0;
int CWriteSHM::nPacketCounter = 0;
int CWriteSHM::nInitializeCounter = 0;

/********************************************************
Function Name: InitializeWriteSHM
Purpose: The function will have the code to create the
	 SHM.
Return Type: void.
Parameters: key_t key_tSHM, key_t key_tSHMStructureCounter,
*********************************************************/
void CWriteSHM::InitializeWriteSHM(key_t key_tSHM, key_t key_tSHMStructureCounter)
{

	nInitializeCounter = nInitializeCounter +1;
	printf("\n\nInitialization CALLED: %d",nInitializeCounter);

	//Variables to hold Data to be written on shared memory.
        char chArrSharedMemoryData[MAX_PACKET_LENGTH];
        char chArrStructureCounter[ST_BUFFER];
        unsigned short usLengthSharedMemoryData = 0;

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
        Code to assign the Shared memory for the reading writing of pcap.
        ***********************************************************************/
        nSHMid = shmget(key_tSHM, size_tSHMSize, IPC_CREAT);

        //Loop to wait until successfully getting the SHM.
        while (nSHMid < 0)
        {
                printf("\nFailed to create shm, failed in : shmget in CWriteSHM.cpp\n");
                nSHMid = shmget(key_tSHM, size_tSHMSize, IPC_CREAT);
        }//While closed.

        //Get the address of the SHM.
        pchSHMBaseAddress = (char*) shmat(nSHMid, NULL, 0);
        //Store the base address.
        pchSHMAddress = pchSHMBaseAddress;

        //Calculate last address.
        char *pchLastAddress = pchSHMAddress + SHM_SIZE;

        //Initialize shared memory with w.
        memset(pchSHMAddress,'z',size_tSHMSize);
	printf("\nINITIALIZATION POINTER : %p\n\n",pchSHMAddress);

}


/********************************************************
Function Name: WriteSHM
Purpose: The function will have the code to write 
         data on the SHM.
Return Type: void.
Parameters: key_t key_tSHM, key_t key_tSHMStructureCounter,
char *pchPacketAddr
*********************************************************/
void CWriteSHM::WriteSHM(key_t key_tSHM, key_t key_tSHMStructureCounter,char *pchPacketAddr)
{
	//Variables to hold Data to be written on shared memory.
        char chArrSharedMemoryData[MAX_PACKET_LENGTH];
        char chArrStructureCounter[ST_BUFFER];
        unsigned short usLengthSharedMemoryData = 0;

	nPacketCounter = nPacketCounter + 1;

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
        Code to assign the Shared memory for the reading writing of pcap.
        ***********************************************************************/
        nSHMid = shmget(key_tSHM, size_tSHMSize, S_IWOTH);

	//Loop to wait until successfully getting the SHM.
        while (nSHMid < 0) 
	{
		printf("\nFailed to access shm, failed in : shmget in CWriteSHM.cpp\n");
                nSHMid = shmget(key_tSHM, size_tSHMSize, IPC_CREAT);
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
        	//printf("failed to create shm segment\n");
                nSHMidStructureCounter = shmget(key_tSHMStructureCounter,size_tSHMStructureCounter,S_IWOTH);

        }

	//Structure object 
        STStructureCounter *pSTStructureCounter;
        pSTStructureCounter = (STStructureCounter*)(shmat(nSHMidStructureCounter, NULL, 0));
	
	/***********************************************************************
	Code to write on the SHM.
	************************************************************************/
	//Variable to store number of characters written on shared memory.
        int nWordsWritten = 0;
        char chValue;	
	int nLoopVariable = 0; 
	int nWhileFlag = 1;


       //Loop will run until all the words are written on shared memory.
        while(nWhileFlag == 1)
        {

		//Check if the end of shared memory is reached. 
                if( nChunkCounter == CHUNK_NUMBER)
                {
                	//printf("\nThe data pointer is reset in write block.\n");
                	//Reset counter to assign start address again.
                	nChunkCounter = 0;
                
		}//if closed.
	
		//Assigning pointer the address of chunk.
		pchSHMAddress = pchSHMBaseAddress +(CHUNK_SIZE *(nChunkCounter));

                //assigning the value of data at shared memory in the variable.                 
		chValue = *pchSHMAddress;

		if(nPacketCounter<0)
                {
			//printf("\n----------------------------------------------------------------------------------");
                        printf("\nWRITE(wait) nCHUNK ADDRESS: %p , packet counter: %d ",pchSHMAddress,nPacketCounter);
                	printf("value of chValue: %c , Chunk Number: %d ",chValue,nChunkCounter);
			printf("Packets in memory: %d ",pSTStructureCounter->unPacketsInMemory);
			printf("Packets Read: %d", pSTStructureCounter->unReadPackets);
			printf("Packets Written: %d", pSTStructureCounter->unWrittenPackets);
			//printf("\n----------------------------------------------------------------------------------");
		}
		
               	//check if the value is not 'r'( for write.)
                if(chValue != 'r')
                {
                   
			nWhileFlag = 0;
	
			if(nPacketCounter<0)
                	{
                        	//printf("\n----------------------------------------------------------------------------------");
				printf("\nWRITE, nCHUNK ADDRESS: %p , packet counter: %d ",pchSHMAddress,nPacketCounter);
				printf("value of chValue:%c , Chunk Number:%d ",chValue,nChunkCounter);
                       		printf("Packets in memory: %d ",pSTStructureCounter->unPacketsInMemory);
				printf("Packets Read:%d", pSTStructureCounter->unReadPackets);
                        	printf("Packets Written:%d",pSTStructureCounter->unWrittenPackets);
				//printf("\n----------------------------------------------------------------------------------");	
			}			


			//Store the flag address.
                	char *pchFlagAddress = pchSHMAddress;
		
			nLoopVariable++;

                        int nInnerLoopVariable = 3 ;

                        //increment pointer to next position.
                       	pchSHMAddress = pchSHMAddress + 1;                  
			
			/*************************************************
                         Code to obtain the write length of the data to be written.
                        ****************************************************/

                        //Append the counter...
                        //memcpy(chArrSharedMemoryData+usLengthSharedMemoryData,(unsigned char*)(&nLoopVariable),2);

                   	//Calculate new length.
                        unsigned short usLengthSharedMemoryData = ntohs(*((unsigned short*)(pchPacketAddr + ETHERNET_HEADER_LEN + IP_FIRST_TWO_BYTES)));

        		usLengthSharedMemoryData = usLengthSharedMemoryData + ETHERNET_HEADER_LEN;

                        unsigned char *tmp = (unsigned char*)(&usLengthSharedMemoryData);
                        //printf("\nLength to write: %02x, %02x\n", tmp[0], tmp[1]);
                        memcpy(pchSHMAddress,(unsigned char*)&usLengthSharedMemoryData,2);
			//printf("\nWRITE wrote length inner !=r %s:%d",__FILE__,__LINE__); 
			//Increment the pointer to the next value.
                        pchSHMAddress = pchSHMAddress + 2;

			//Set pointer to zero.
                	nWordsWritten = 0;
			
			//Copy the data into the SHM
			memcpy(pchSHMAddress,pchPacketAddr,usLengthSharedMemoryData);		
			
			/********************************
                         Increasing the write counter.
                        ********************************/
			pSTStructureCounter->unWrittenPackets = pSTStructureCounter->unWrittenPackets + 1;
			//Increase packets in memory.
                        pSTStructureCounter->unPacketsInMemory = pSTStructureCounter->unPacketsInMemory + 1;
			printf("\nAfter memcpy WRITE.. increased write counters....... %s:%d",__FILE__,__LINE__);
			//Set flag pointer to r to indicate memory is free to read.
                        memset(pchFlagAddress,'r',1);
                							
                        //Increment the chunk address to next location.
                        nChunkCounter = nChunkCounter + 1;

		}//if case closed.

		//Reset word count.
                nWordsWritten = 0;

	}//while closed.


}//End of function WriteSHM.

