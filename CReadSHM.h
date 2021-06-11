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

#define SHM_SIZE 16640
#define CHUNK_SIZE 1664
#define CHUNK_NUMBER 10
#define ST_BUFFER 4


/******************************************************
Class Name: CReadSHM
Purpose : The class will have the code to read the
          contents on the SHM and write on a file.
*******************************************************/
class CReadSHM{


        public:

        /********************************************************
        Function Name: CReadSHM
        Purpose: The function will have the code to write 
                 data on the SHM.
        Return Type: void.
        Parameters: key_t key_tSHM, key_t key_tSHMStructureCounter
        *********************************************************/
        void ReadSHM(key_t key_tSHM, key_t key_tSHMStructureCounter);


};//End of class.
