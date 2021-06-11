#include "CSHMStatistics.h"


/***********************************************************
Function Name: DumpSHMStatistics
Purpose: The function will have the code to read 
                 data of  the SHMStatictics and write  on a file.
Return Type: void.
Parameters: key_t key_tSHMStructureCounter
*********************************************************/
void CSHMStatistics::DumpSHMStatistics(key_t key_tSHMStructureCounter)
{
	
	//Variables to store the SHMid.
        int nSHMidStructureCounter;
	size_t size_tSHMStructureCounter = sizeof(STStructureCounter);


	/*********************************************************************
        Code to assign the Shared memory for the structure pointer.
        **********************************************************************/
        nSHMidStructureCounter = shmget(key_tSHMStructureCounter,size_tSHMStructureCounter,IPC_CREAT);

        //Loop to wait until successfully getting the SHM.
        while (nSHMidStructureCounter < 0)
        {
                printf("failed to create shm segment\n");
                nSHMidStructureCounter = shmget(key_tSHMStructureCounter,size_tSHMStructureCounter,IPC_CREAT);

        }

        //Structure object 
	STStructureCounter *pSTStructureCounter;
	pSTStructureCounter = (STStructureCounter*)(shmat(nSHMidStructureCounter, NULL, 0));

	printf("\n--------- Initializing counters-----------.\n");
        //Initialize memory with the structure pointer.
        pSTStructureCounter->unWrittenPackets = 0;
        pSTStructureCounter->unReadPackets = 0;
        pSTStructureCounter->unPacketsInMemory = 0;
        printf("Initialized.... Values are: ");
        printf("\n unWrittenPackets: %d\n",pSTStructureCounter->unWrittenPackets);
	printf("\n unReadPackets: %d",pSTStructureCounter->unReadPackets);
	printf("\n unPacketsInMemory: %d\n",pSTStructureCounter->unPacketsInMemory);
	printf("\n--------- Initializing Completed-----------.\n");

	/****************************************************
        Code to open the file in which data has to be written.
        ****************************************************/

        //Creating a file type pointer to file.
        FILE *pFile;

        //check if file is existing or not.
        if (pFile = fopen("DataSTCounter.txt", "rb+"))
        {
        	fclose(pFile);
        }

        //signifies file is not existing.
        else
        {
        	//creating a new file.
                pFile = fopen("DataSTCounter.txt", "wb+");
                fclose(pFile);
       	}


	int nLoopVariable = 1;


        //Open file in append mode.
        pFile = fopen("DataSTCounter.txt", "ab+");

        //Loop to write the values of the counters in the file. 
        while(1)
        {
        	nLoopVariable++;

           	if((nLoopVariable)==5)
		{
			usleep(1000000); 
		}
                
             	//Check if the file pointer is null (File cannot be opened.)
                if(pFile == NULL)
               	{
                	printf("\n cannot open file in the function CSessionManager case 3. \n");
                }

                //Case when file opened successfully.
                else
                {
                	fprintf(pFile,"\n\n--------------------------------\n");
                        fprintf(pFile,"\nLOOP NUMBER : %d",nLoopVariable);
                        fprintf(pFile,"\nWrite Counter : %d",pSTStructureCounter->unWrittenPackets);
                        fprintf(pFile,"\nRead Counter : %d",pSTStructureCounter->unReadPackets);
                        fprintf(pFile,"\n\n--------------------------------\n");
                }


          }//while closed.        

         //Close resource
         fclose(pFile);

}//End of function.
