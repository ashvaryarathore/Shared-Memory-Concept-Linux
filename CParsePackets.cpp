#include "CParsePackets.h"
#include "CDisplayStatisticsFileDump.h"
#include<stdio.h>
using namespace std;


/***********************************************************************************
FunctionName : ParsePacketData
Description : The function will use the pcap library function to get the packets and  
				  packet data from the adapter that user has decided.
Return Type : void.
Paramter : u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data
************************************************************************************/
void CParsePackets::ParsePacketData(const char *pkt_data)
{

	stIPheader *stpIPHeader;
	stEthernetHeader *stpEthernetHeader = new stEthernetHeader;
	stTCPheader *stpTCPheader = new stTCPheader;
	char *chHttpHeader;
	
	/**********************************************************
	Code to get the Source and Destination MAC address (Parsing
	Ethernet Header)
	***********************************************************/
	
	// adding the packet data length to obtain the ethernet header position.
	stpEthernetHeader = (stEthernetHeader *) (pkt_data);
	
	// Adding the length of ethernet header to obtain the position of the ip header 
	stpIPHeader = (stIPheader *) (pkt_data + ETHERNET_HEADER_LEN); //length of ethernet header

	//Code to obtain the 4 bits of Header Length from IP Header
	unsigned short HeaderLength = (stpIPHeader->uchVersion_HeaderLength) & 15;

	//Code to calculate the TCP Header position.
	unsigned short TotalIPHeaderLength = (HeaderLength)*4;

	//Calculating the TCP Header Position.
	stpTCPheader = (stTCPheader *) (pkt_data + ETHERNET_HEADER_LEN + TotalIPHeaderLength );

	

	/**************************************************************************
	Code identifying the IP Protocol, and to process funther only if it is IPV4.
	***************************************************************************/
	
	//the ntohs function will convert the bits in network to host order for comparision.
	if(ntohs(stpEthernetHeader->EthernetType) == PROTOCOL_TYPE_IPV4)
	{
		/****************************************************************************************
		Code for processing the IP Header if IPV4 and obtaining the Source and Destinatin address.
		*****************************************************************************************/
		
		//Code to save the Total Header Length.
		unsigned short usTotalLength= ntohs(stpIPHeader->usTotalLength);


		/**************************************************************
		Code to check if the protocol is TCP.Block contains the code to 
		process the TCP Header if the protocol is TCP 
		****************************************************************/
		
		//Identifying the TCP protocol check.
		if((int)(stpIPHeader->uchProtocol) == PROTOCOL_TCP)
		{
			
			/*****************************************************************
			Code to process the TCP Header.
			******************************************************************/
			//Code to calculate the TCP Offset.
			unsigned short usOffset = ntohs(stpTCPheader->usDataOffset_Reserved_Flags) & 0xF000;
			usOffset = usOffset >> 12;

			//Code to calculate TCP payload.
			unsigned short usTcpPayload = ntohs(stpIPHeader->usTotalLength) - (( HeaderLength + usOffset ) * DATA_BITS);

			//Code to calculate TCP header Length.
			unsigned short usTcpHeaderLength = (usOffset ) * DATA_BITS;



			/**************************************************************
			Code to maintain the session log.
			***************************************************************/
			/*
			unsigned short usSessionFlag = ntohs(stpTCPheader->usDataOffset_Reserved_Flags) & 0x02;
			unsigned short usFinalFlag = ntohs(stpTCPheader->usDataOffset_Reserved_Flags) & 0x01;
			unsigned short usAckFlag =  ntohs(stpTCPheader->usDataOffset_Reserved_Flags) & 0x10;
			//Creating object of CSessionManager.
			CSessionManager *objCSessionManager = CSessionManager::GetInstance();
		
			printf("\nPACKET...........\n");

			if(usSessionFlag == 2 && usAckFlag == 0)
			{
				//insert key...	
				printf("\n\n Recieved Syn......");
				objCSessionManager->SetHashMap(stpIPHeader,stpTCPheader,1,usTcpPayload);
				printf("\n--------------------------------\n");
			}

			if(usFinalFlag == 1)
			{
				printf("\n\n Recieved Fin......");
				objCSessionManager->SetHashMap(stpIPHeader,stpTCPheader,2,usTcpPayload);
				printf("\n--------------------------------\n");
				//close session code...			
			}
			
			//code to identify session for packet and manage it.
			printf("\n\n Recieved a Normal Packet.....");
			objCSessionManager->SetHashMap(stpIPHeader,stpTCPheader,3,usTcpPayload);
			printf("\n--------------------------------\n");
			
			*/

			/*************************************************************************************
			Code to Parse HTTP Header.
			*************************************************************************************/

			//check if protocol is HTTP or not.
			if(((ntohs(stpTCPheader->usSourcePort)==80)||(ntohs(stpTCPheader->usDestinationPort) == 80)) && (usTcpPayload > 0) )
			{
				//printf("\n INSIDE HTTP HEADER PARSER .... \n");
				//Calculate position of http header.
				chHttpHeader = (char *) (pkt_data + ETHERNET_HEADER_LEN + TotalIPHeaderLength + usTcpHeaderLength );
				
				/*********************************************************
				Code to verify that the payload contains HTTP Header or not.
				**********************************************************/
				
				//Getting the first 10 characters of http header
				char chTempHttpHeaderData[10];

				char *chTempHttpHeader;
				chTempHttpHeader = chHttpHeader;

				for(int nLoopVariable = 0; nLoopVariable <10 ; nLoopVariable++)
				{
					chTempHttpHeaderData[nLoopVariable] = chTempHttpHeader[nLoopVariable];
				}

				//Funtion to check the payload contains HTTP Header or not.
				// The funtion will return 1 if the paylod containd HTTP header.
				if(CheckHttpHeaderInPayload(chTempHttpHeaderData))
				{
					//printf("Calling the file writing function.");
				
					//Object for the CDisplayStatisticsFileDump class
				
					//Reference of Base class.
					CDisplayStatistics *objDisplayStatisticsFileDump;


					//Address of derived class.
					objDisplayStatisticsFileDump = new CDisplayStatisticsFileDump;
					objDisplayStatisticsFileDump -> DisplayPacketStatistics(stpEthernetHeader,
												stpIPHeader,
												stpTCPheader,
												chHttpHeader, 
												HeaderLength ,
												usTcpPayload);
				}
			}
		}


		/*Code to check if the protocol is UDP.
		Block contains the code to process the UDP Header 
		if the protocol is UDP */
		if(stpIPHeader->uchProtocol == PROTOCOL_UDP)
		{}



	}// If closed for the IPV4 protocol check.
	
}



/***************************************************************************
FunctionName : CheckHttpHeaderInPayload
Description : The function will check that the payload contains HTTP data or not.
				The funtion taked initial 10 characters from the payload and will return 
				1 on success, and 0 otherwise.
Return Type : int.
Paramter : u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data
*****************************************************************************/
int CParsePackets::CheckHttpHeaderInPayload(char chTempHttpHeaderData[])
{
	//loop to iterate the size 10 array.
	for(int nLoopVariable = 0; nLoopVariable <10 ; nLoopVariable++)
	{
		
		/**************************************
		Code to Check for GET request.
		**************************************/
		if(chTempHttpHeaderData[nLoopVariable] == 'G')
		{
			if(chTempHttpHeaderData[nLoopVariable + 1] == 'E')
			{
				if(chTempHttpHeaderData[nLoopVariable + 2] == 'T')
				{
					return 1;
				}
			
			}
		}
		

		/**************************************
		Code to Check for POST request.
		**************************************/
		if(chTempHttpHeaderData[nLoopVariable] == 'P')
		{
			if(chTempHttpHeaderData[nLoopVariable + 1] == 'O')
			{
				if(chTempHttpHeaderData[nLoopVariable + 2] == 'S')
				{
					if(chTempHttpHeaderData[nLoopVariable + 3] == 'T')
					{
						return 1;
					}
				}
			
			}
		}
		
		

		/**************************************
		Code to Check for HEAD request.
		**************************************/
		if(chTempHttpHeaderData[nLoopVariable] == 'H')
		{
			if(chTempHttpHeaderData[nLoopVariable + 1] == 'E')
			{
				if(chTempHttpHeaderData[nLoopVariable + 2] == 'A')
				{
					if(chTempHttpHeaderData[nLoopVariable + 3] == 'D')
					{
						return 1;
					}
				}
			
			}
		}
		

		/**************************************
		Code to Check for PUT request.
		**************************************/
		if(chTempHttpHeaderData[nLoopVariable] == 'P')
		{
			if(chTempHttpHeaderData[nLoopVariable + 1] == 'U')
			{
				if(chTempHttpHeaderData[nLoopVariable + 2] == 'T')
				{
					return 1;
					
				}
			
			}
		}
		


		/**************************************
		Code to Check for DELETE request.
		**************************************/
		if(chTempHttpHeaderData[nLoopVariable] == 'D')
		{
			if(chTempHttpHeaderData[nLoopVariable + 1] == 'E')
			{
				if(chTempHttpHeaderData[nLoopVariable + 2] == 'L')
				{
					if(chTempHttpHeaderData[nLoopVariable + 3] == 'E')
					{
						if(chTempHttpHeaderData[nLoopVariable + 4] == 'T')
						{
							if(chTempHttpHeaderData[nLoopVariable + 5] == 'E')
							{
								return 1;
							}
						}
					}
				}
			}
		}
		


		/**************************************
		Code to Check for OPTIONS request.
		**************************************/
		if(chTempHttpHeaderData[nLoopVariable] == 'O')
		{
			if(chTempHttpHeaderData[nLoopVariable + 1] == 'P')
			{
				if(chTempHttpHeaderData[nLoopVariable + 2] == 'T')
				{
					if(chTempHttpHeaderData[nLoopVariable + 3] == 'I')
					{
						if(chTempHttpHeaderData[nLoopVariable + 4] == 'O')
						{
							if(chTempHttpHeaderData[nLoopVariable + 5] == 'N')
							{
								if(chTempHttpHeaderData[nLoopVariable + 6] == 'S')
								{
									return 1;
								}
							}
						}
					}
				}
			}
		}



		
		/**************************************
		Code to Check for CONNECT request.
		**************************************/
		if(chTempHttpHeaderData[nLoopVariable] == 'C')
		{
			if(chTempHttpHeaderData[nLoopVariable + 1] == 'O')
			{
				if(chTempHttpHeaderData[nLoopVariable + 2] == 'N')
				{
					if(chTempHttpHeaderData[nLoopVariable + 3] == 'N')
					{
						if(chTempHttpHeaderData[nLoopVariable + 4] == 'E')
						{
							if(chTempHttpHeaderData[nLoopVariable + 5] == 'C')
							{
								if(chTempHttpHeaderData[nLoopVariable + 6] == 'T')
								{
									return 1;
								}
							}
						}
					}
				}
			}
		}




		/**************************************
		Code to Check for HTTP 
		**************************************/
		if(chTempHttpHeaderData[nLoopVariable] == 'H')
		{
			if(chTempHttpHeaderData[nLoopVariable + 1] == 'T')
			{
				if(chTempHttpHeaderData[nLoopVariable + 2] == 'T')
				{
					if(chTempHttpHeaderData[nLoopVariable + 3] == 'P')
					{
						return 1;
					}
				}
			
			}
		}


		//if any case did not match.
		return 0;
		
		
	}

}
