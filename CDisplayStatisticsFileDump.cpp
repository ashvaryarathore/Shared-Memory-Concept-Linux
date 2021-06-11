#include "CDisplayStatisticsFileDump.h"
#include<stdio.h>
using namespace std;


/******************************************************************************
Function Name : DisplayPacketsStatistics
Purpose : The function will be used to display the statictics of the captured 
		    packets.
Return Type : void.
Parameters : stEthernetHeader *stpEthernetHeader,
			stIPheader *stpIPHeader,
			stTCPheader *stpTCPheader
*******************************************************************************/
void CDisplayStatisticsFileDump :: DisplayPacketStatistics(stEthernetHeader *stpEthernetHeader, 
									stIPheader *stpIPHeader, 
									stTCPheader *stpTCPheader, 
									char *chHttpHeader, 
									unsigned short HeaderLength,
									unsigned short usTcpPayload)
{

	FILE *pFile;

	//check if file is existing or not.
	if (pFile = fopen("stats.txt", "rb+"))
    	{
        	fclose(pFile);
        
    	}	

	//signifies file is not existing.
	else
	{
		//creating a new file.
		pFile = fopen("stats.txt", "wb+");
		fclose(pFile);
	}
	
	
	//Opening the file, a+ signifies append in existing file.
	pFile = fopen("stats.txt", "ab+"); 
	

	
	if(pFile == NULL)
	{
		printf("\n cannot open file in the function CDisplayStatisticsFileDump :: DisplayPacketStatistics \n");
	}

	
	else
	{
		/**********************************************************
		Code to get the Source and Destination MAC address (Parsing
		Ethernet Header)
		***********************************************************/
		fprintf(pFile,"\n -------------------------------------------\n ");
		fprintf(pFile,"\n Source MAC Address : ");

		// Loop to print the MAC Addresses.
		for( int nLoopVariable = 0; nLoopVariable < ETH_MAC_ADDRESS_LEN  ; nLoopVariable++)
		{
			// if to adjust the formatting used in printing the MAC Adress (Avoiding the : at the end).
			if( nLoopVariable == (ETH_MAC_ADDRESS_LEN-1) )
			{
				//%02X is used to convert one character to a hexadecimal string.
				fprintf(pFile,"%02x", stpEthernetHeader->SourceMacAddress[nLoopVariable]); 
		
			}

			else
			{
				fprintf(pFile,"%02x:", stpEthernetHeader->SourceMacAddress[nLoopVariable]);
			}
		
		}

		fprintf(pFile,"\n Destination MAC Address : ");

		// loop to iterate the character array of length ETH_MAC_ADDRESS_LEN used to store and print the mac address.
		for( int nLoopVariable=0; nLoopVariable < ETH_MAC_ADDRESS_LEN ; nLoopVariable++)
		{
			//printf("\n Inside Loop to print the MAC Addresses.");
			// if to adjust the formatting used in printing the MAC Adress (Avoiding the : at the end).
			if( nLoopVariable == (ETH_MAC_ADDRESS_LEN-1) )
			{
				//%02X is used to convert one character to a hexadecimal string.
				fprintf(pFile,"%02x", stpEthernetHeader->DestinationMacAddress[nLoopVariable]); 
		
			}

			else
			{
				fprintf(pFile,"%02x:", stpEthernetHeader->DestinationMacAddress[nLoopVariable]);
			}
	
		}


		//code to print the ethernet type.
		fprintf(pFile,"\n Ethernet type:  %04x:", ntohs(stpEthernetHeader->EthernetType));



		// Code to print ip addresses 
		fprintf(pFile,"\n Source IP Address : %d.%d.%d.%d \n Destination IP Address: %d.%d.%d.%d \n IP Packet Header Length : %d",
				stpIPHeader->SourceIPAddress.cIPAddressByte1,
				stpIPHeader->SourceIPAddress.cIPAddressByte2,
				stpIPHeader->SourceIPAddress.cIPAddressByte3,
				stpIPHeader->SourceIPAddress.cIPAddressByte4,
				stpIPHeader->DestinationIPAddress.cIPAddressByte1,
				stpIPHeader->DestinationIPAddress.cIPAddressByte2,
				stpIPHeader->DestinationIPAddress.cIPAddressByte3,
				stpIPHeader->DestinationIPAddress.cIPAddressByte4,
				(HeaderLength*4));


		//Printing the TCP Packet Header contents.
		fprintf(pFile,"\n Source Port : %d \n Destination Port: %d \n",
				ntohs(stpTCPheader->usSourcePort), // converting the port number in ntohs.
				ntohs(stpTCPheader->usDestinationPort));

		
		//Printing the TCP Payload.
		fprintf(pFile,"\n TCP payload length : %d", usTcpPayload);


		/***********************************
		Code to print the HTTP header.
		**************************************/
		//Printing the HTTP header contents.
		char *chTempHttpHeader;
		chTempHttpHeader = chHttpHeader;
				
		fprintf(pFile, "\n  HTTP Header Data :\n");


		//outer loop to detect the end of header.				
		while ((*(chTempHttpHeader) != '\r') && (*(chTempHttpHeader+1) != '\n'))
		{
				
			//inner loop to detect the end of line.		
			while((*(chTempHttpHeader) != '\r') && (*(chTempHttpHeader+1) != '\n'))
			{
				fprintf(pFile,"%c", *chTempHttpHeader);
				chTempHttpHeader = chTempHttpHeader + 1;
				
			}

			fprintf(pFile,"\n");
			chTempHttpHeader = chTempHttpHeader + 2;
			
		}
				



		fprintf(pFile,"\n -------------------------------------------\n ");

		fclose(pFile);
	}//else closed


}



