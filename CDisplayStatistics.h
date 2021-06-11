#pragma once

#include "CParsePackets.h"



/********************************************************
Class Name : CDisplayStatistics 
Type : Abstract Class
Function : The class will contain display function abstract method
		   which will be used to display the statistics in desired form
		   (as per inherited classes.).
*********************************************************/
class CDisplayStatistics
{
	public:
     
	/******************************************************************************
	Function Name : DisplayPacketsStatistics
	Type : Abstract function , (Pure virtual function is used to
		   implement the abstract class property in c++. Indicated by 
		   'virtual' keyword.
	Purpose : The function will be used to display the statictics of the captured 
		      packets.
	Return Type : void.
	Parameters :stEthernetHeader *stpEthernetHeader,
			stIPheader *stpIPHeader,
			stTCPheader *stpTCPheader
	*******************************************************************************/
    virtual void DisplayPacketStatistics(stEthernetHeader *stpEthernetHeader, 
									stIPheader *stpIPHeader, 
									stTCPheader *stpTCPheader, 
									char *chHttpHeader,
									unsigned short HeaderLength,
									unsigned short usTcpPayload) = 0;
	
};


