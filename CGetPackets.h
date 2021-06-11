#pragma once

#include<stdio.h>
#include<pcap.h>
#include "CParsePackets.h"


class CGetPackets 
{
	pcap_if_t *pcappAdapterList;
	pcap_if_t *pcappAdapterToParse;
	pcap_t  *pcappPacketToParse;
	char chpErrorBuffer[ PCAP_ERRBUF_SIZE ]; //required to allocate the buffer of default size PCAP_ERRBUF_SIZE if error occurs.
	int nNumberOfAdapters;
	

	public:

	/***************************************************************************
	FunctionName : SetAdapterList
	Description : The function will use the pcap library function to get the list 
				  of all the adapters on the source which is provided by the user. 
				  The function will initialize the pcappAdapterList with the pointer 
				  to the set of all adapter list.
	Return Type : void.
	Paramters : None.
	******************************************************************************/
	void SetAdapterList();



	/*****************************************************************************
	FunctionName : SetAdapterToParse
	Description : The function will set the value of the pcappAdapterToParse variable, 
				  which will have the adapter chosen by the user, which is to be parsed.
	Return Type : void.
	Paramters : int nUserChoiceOfAdapter.
	********************************************************************************/
	void SetAdapterToParse(int nUserChoiceOfAdapter);



	/********************************************************************************
	FunctionName : GetAdapterToParse
	Description : The function will return the value of the pcappAdapterToParse variable, 
				  which will have the adapter chosen by the user, which is to be parsed.
	Return Type : pcap_if_t
	Parameters : None.
	*********************************************************************************/
	pcap_if_t* GetAdapterToParse();
	


	
	/********************************************************************************
	FunctionName : GetPacketToParse
	Description : The function will open the adapter and return the captured packets which
				  we can forward to be parsed.
	Return Type : pcap_t*
	Parameters : None.
	*********************************************************************************/
	void OpenSessionToParse();





	/********************************************************************************
	FunctionName : GetPackets
	Description : The function will capture packets needed to parse.
	Return Type : void
	Parameters : None.
	*********************************************************************************/
	void GetPackets();




	/********************************************************************************
	Funciton name: packet_handler 
	Description : Callback function which is invoked by the pcap_loop method.
	Return type : void
	Parameters : u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data
	*******************************************************************************/
	void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);




	
	/************************************************************************
	FunctionName : GetPacketToParse
	Description : The function will return the value of the pcappPacketToParse variable, 
					which will have the packets, which is to be parsed.
	Return Type : pcap_if_t
	Parameters : None.
	************************************************************************/
	pcap_t* GetPacketToParse();
	


	/************************************************************************
	FunctionName : GetNumberOfAdapters
	Description : The function will return the value of the pcappPacketToParse variable, 
					which will have the packets, which is to be parsed.
	Return Type : int
	Parameters : None.
	************************************************************************/
	int GetNumberOfAdapters();


};


