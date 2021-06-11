#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include "CSHMStatistics.h"
#include "CReadSHM.h"
#include "CWriteSHM.h"
#include "CReadPacketDump.h"
#include <pcap.h>
#include "CParsePackets.h"
#include "CGetPackets.h"

#define ETHERNET_HEADER_LEN 14
#define IP_FIRST_TWO_BYTES 2

//Global Object of class CWriteSHM
CWriteSHM objCWriteSHM;

//Global Key Variables to store the Key for the shared memory.
key_t key_tSHM = 0x01020304;
key_t key_tSHMStructureCounter = 0x01020305;
int nNumberOfPackets = 0;

/*********************************************************************************
Function Declaration.
Callback Function defined in the winpcap library. Called by the pcap_loop method. 
when reading from a file.
*********************************************************************************/
void dispatcher_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);


/*********************************************************************************
Function Declaration.
Callback Function defined in the winpcap library. Called by the pcap_loop method. 
when capturing packets from adapter.
*********************************************************************************/
void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);



/*********************************************
Function : main()
Purpose : The execution will start from this
          point.
Return type : int.
Parameters  : None.
**********************************************/
int main()
{

	//Create the SHM for read,write.
        objCWriteSHM.InitializeWriteSHM(key_tSHM,key_tSHMStructureCounter);

	//Variables to hold the user choice, and pcap adapter.
	int nUserChoice;
	int nNumberOfAdapters;
	pcap_t *pcappPacketToParse;

	//Take user input.
        printf("\n Do you wish to: \n 1)Capture packets from adapter \n 2)Read Packets from a dump.  \nEnter your choice ( 1, 2):");
        scanf(" %d", &nUserChoice);

	switch(nUserChoice)
        {

        		case 1:
                                /***************************************************
                                Code when user chooses to capture from adapter.
                                ***************************************************/
                                //Object of class CGetPackets.
                                CGetPackets objCGetPackets;
                                objCGetPackets.SetAdapterList();
                                nNumberOfAdapters = objCGetPackets.GetNumberOfAdapters();
                                printf(" \n\n Pease enter your choice of adapter to be parsed. \n Enter the number of the adapter : ");
                                scanf(" %d", &nUserChoice);

                                //Loop to reach the adapter user requested.
                                if(nUserChoice <= nNumberOfAdapters)
                                {
                                        printf("\n Your choice : %d", nUserChoice);

                                        objCGetPackets.SetAdapterToParse(nUserChoice);

                                        pcappPacketToParse = objCGetPackets.GetPacketToParse();

                                }

                                else
                                {
                                        printf("\n You entered an invalid choice. Please select the adapters from 1 - %d", nNumberOfAdapters );

                                }

                                break;

                        case 2:
                                /***************************************************
                                Code when user chooses to read from PCAP.
                                ***************************************************/

                                //Object of CReadPacketDump.
                                CReadPacketDump objReadPacketDump;
                                objReadPacketDump.ReadPacketsFromDump();
                                pcappPacketToParse = objReadPacketDump.GetPacketToParse();

                                break;


                        default :
                                printf("\n You entered a wrong option. Please select a valid choice.");
                                break;

                }//End of switch case.


	//Initialize the process id's
	pid_t pid_tProcess2 = -1;
	pid_t pid_tProcess3 = -1;

	//Fork the multiple processes.
	pid_tProcess3  = fork();
		       
	//Condition to ensure that only parent forks other process.
        if((pid_tProcess3 > 0))
        {
		//Fork another process.
		pid_tProcess2  = fork();
        }


	/*********************************************************
        Code for the writing the statistics.
        PROCESS 3
        *********************************************************/
        if((pid_tProcess3==0))
        {
		printf("\n\n In process 3 condition......... \n"); 
		
		//Object of CSHMStatistics
		CSHMStatistics objCSHMStatistics;
		objCSHMStatistics.DumpSHMStatistics(key_tSHMStructureCounter);

	}//End of it for PROCESS 3.


	/*********************************************************
        Code for the reading case.
        PROCESS 2
        *********************************************************/
        if((pid_tProcess2==0))
        {
                usleep(2000000);
		printf("\n\n In process 2 condition......... \n");

                //Object of class CReadSHM
                CReadSHM objCReadSHM;
                objCReadSHM.ReadSHM( key_tSHM, key_tSHMStructureCounter);
        }//End of if for PROCESS2.


	/***********************************************************
        Code for the PARENT PROCESS.
        Waiting for all child process to finish.
        ***********************************************************/
        if((pid_tProcess2>0)&&(pid_tProcess3>0))
        {

		usleep(2000000);
        	
		switch(nUserChoice)
        	{

                	case 1:
                                /***************************************************
                                Code when user chooses to capture from adapter.
                                ***************************************************/
                                //Loop to reach the adapter user requested.
                                if(nUserChoice <= nNumberOfAdapters)
                                {
                                        //Predefined function of Winpcap Library, used to start capturing the packets.
                                        pcap_loop(pcappPacketToParse, 0, packet_handler, NULL);

                                        printf("successs");
                                }

                                else
                                {
                                        printf("\n You entered an invalid choice. Please select the adapters from 1 - %d", nNumberOfAdapters );

                                }

                                break;

                	case 2:
                                /***************************************************
                                Code when user chooses to read from PCAP.
                                ***************************************************/
                                printf("\n Executing Case :2");

                                //read and dispatch packets until EOF is reached
                                pcap_loop(pcappPacketToParse, 0, dispatcher_handler, NULL);
                                break;


                	default :
                                printf("\n You entered a wrong option. Please select a valid choice.");
                                break;

        	}//End of switch case.
   
	
	     	/*wait for the  child process2 which is reading to finish*/
                waitpid(pid_tProcess2, NULL, 0);

                /*wait for the  child process3 which is reading to finish*/
                waitpid(pid_tProcess3, NULL, 0);

                printf("\nTerminating main parent\n");
		

        }//End of if for Parent Process.

	return 0;

}//End of main().



/*********************************************************************************
Callback Function defined in the winpcap library. Called by the pcap_loop method. 
when reading from a file.
*********************************************************************************/
void dispatcher_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
{

	nNumberOfPackets = nNumberOfPackets + 1;
	//printf("\n\nDISPATCHER HANDLER called.... Number of packets :%d",nNumberOfPackets);
	//Char buffer to hold the packet.
 	char *chArrBuff;
	
	unsigned short usLengthSharedMemoryData = ntohs(*((unsigned short*)(pkt_data + ETHERNET_HEADER_LEN + IP_FIRST_TWO_BYTES)));

	usLengthSharedMemoryData = usLengthSharedMemoryData + ETHERNET_HEADER_LEN;


	//Assign memory to buffer.
	chArrBuff =(char *) malloc(usLengthSharedMemoryData*sizeof(char));

	//Copy the packet data into the buffer.
        if(chArrBuff!=NULL)
        {
                memcpy(chArrBuff,pkt_data,usLengthSharedMemoryData);
        }

	//Object of writer method. Call funtion to write in SHM.	
	objCWriteSHM.WriteSHM(key_tSHM, key_tSHMStructureCounter,chArrBuff);

}



/*******************************************************************************************************
Callback Function defined in the winpcap library. Called by the pcap_loop method. when capturing packets
from adapter.
********************************************************************************************************/
void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
	nNumberOfPackets = nNumberOfPackets + 1;
        printf("\n\nPACKET HANDLER called. Number of packets :%d",nNumberOfPackets);	
        
	//Char buffer to hold the packet.
        char *chArrBuff;

        //Object of CQueue to hold the packets.
        //CQueue *objCQueue = CQueue::GetInstance(); 

        unsigned short usLengthSharedMemoryData = ntohs(*((unsigned short*)(pkt_data + ETHERNET_HEADER_LEN + IP_FIRST_TWO_BYTES)));

        usLengthSharedMemoryData = usLengthSharedMemoryData + ETHERNET_HEADER_LEN;


        printf("\n The pcap file legth IN DISPATCHER HANDLER is: %d", usLengthSharedMemoryData);

        //Assign memory to buffer.
        chArrBuff =(char *) malloc(usLengthSharedMemoryData*sizeof(char));

        //Copy the packet data into the buffer.
        if(chArrBuff!=NULL)
        {
                memcpy(chArrBuff,pkt_data,usLengthSharedMemoryData);
        }

	//Object of writer method. Call funtion to write in SHM.      
 	objCWriteSHM.WriteSHM(key_tSHM, key_tSHMStructureCounter,chArrBuff);
         printf("\n------------------END--------------------------------\n\n");

     
}






