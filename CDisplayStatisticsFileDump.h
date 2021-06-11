#pragma once

#include<stdio.h>
#include<stdlib.h>
#include "CDisplayStatistics.h"
#include "CParsePackets.h"
#include <iostream>
#include <fstream>
#include <netinet/in.h>

/********************************************************************************
Class Name : CDisplayStatisticsFileDump
Function : The class will contain code to dump the packet statistics to a text file.
		   The class will inherit the abstract class CDisplayStatistics.
*********************************************************************************/



class CDisplayStatisticsFileDump : public CDisplayStatistics
{
	//static int nCheckVariable;

	public:
			
    	/******************************************************************************
	Function Name : DisplayPacketsStatistics
	Purpose : The function will be used to display the statictics of the captured 
		      packets.
	Return Type : void.
	Parameters : stEthernetHeader *stpEthernetHeader,
			stIPheader *stpIPHeader,
			stTCPheader *stpTCPheader
	*******************************************************************************/
    	void DisplayPacketStatistics(stEthernetHeader *stpEthernetHeader, 
						stIPheader *stpIPHeader, 
						stTCPheader *stpTCPheader, 
						char *chHttpHeader,
						unsigned short HeaderLength,
						unsigned short usTcpPayload);

	
		
};






