#include "CGetPackets.h"

/*****************************************************************
Name : CGetPacket Class Function Definitions
Description : The class will include the code required for getting
			  the adapter list using the Winpcap Library functions.

*******************************************************************/



/******************************************************************
FunctionName : SetAdapterList
Description : The function will use the pcap library function to get the list 
			  of all the adapters on the source which is provided by the user. 
			  The function will initialize the pcappAdapterList with the pointer 
			  to the set of all adapter list.
Return Type : void.
Paramters: None.
******************************************************************/
void CGetPackets::SetAdapterList()
{		
	nNumberOfAdapters = 0;
	/*pcap_findalldevs_ex is predefined function in WinPCap Library, used to obtain pointer 
	to set of all adapters. It returns 0 if everything runs fine without error. 
	It returns -1 if an error orrucs.*/
	if( pcap_findalldevs(&pcappAdapterList, chpErrorBuffer ) == 0 ) 
	{
		printf("successfully ran the pcap_findalldevs_ex function \n");
			
		// to check the condition that if the AdapterList is empty.
		if(pcappAdapterList!=NULL)
		{
			printf("The adapter list is as follows:: \n");

			//To print the entire adapter List.
			for( pcappAdapterToParse = pcappAdapterList; pcappAdapterToParse != NULL; pcappAdapterToParse = pcappAdapterToParse->next)
			{
					nNumberOfAdapters++;
					printf( "\n %d.%s ", nNumberOfAdapters, pcappAdapterToParse->name );
					printf( "\n" );
					printf( "-- %s\n", pcappAdapterToParse->description );
			}

			printf("The total number of adapters are %d", nNumberOfAdapters);
		}
		
		else //if the pcappAdapterList is NULL.
		{
			printf("The adapter list is empty! Please use a different Source and try again.");
		}

	}
		
	else //if the pcap_findalldevs_ex function returned -1 
	{	
		fprintf(stderr,"An error occured in pcap_findalldevs_ex function: %s\n", chpErrorBuffer);
	}
		
		
		
}

	

	
/*****************************************************************************************
FunctionName : SetAdapterToParse
Description : The function will set the value of the pcappAdapterToParse variable, 
				which will have the adapter chosen by the user, which is to be parsed.
Return Type : void.
Paramters : int nUserChoiceOfAdapter.
******************************************************************************************/
void CGetPackets::SetAdapterToParse(int nUserChoiceOfAdapter)
{
	int nLoopVariable;

	pcappAdapterToParse = pcappAdapterList; //assigning the initial list adapter list value.
		
	//Loop to reach the user required adapter in the list.
	for( nLoopVariable = 0; nLoopVariable < nUserChoiceOfAdapter - 1; nLoopVariable++ )
	{
		pcappAdapterToParse = pcappAdapterToParse->next;
	}

	
	//Calling to OpenSessionToParse to open the adapter.
	OpenSessionToParse();
}

	

/************************************************************************
FunctionName : GetAdapterToParse
Description : The function will return the value of the pcappAdapterToParse variable, 
				which will have the adapter chosen by the user, which is to be parsed.
Return Type : pcap_if_t
Parameters : None.
************************************************************************/
pcap_if_t* CGetPackets::GetAdapterToParse()
{
	return pcappAdapterToParse;

}



/************************************************************************
FunctionName : GetPacketToParse
Description : The function will return the value of the pcappPacketToParse variable, 
				which will have the packets, which is to be parsed.
Return Type : pcap_if_t
Parameters : None.
************************************************************************/
pcap_t* CGetPackets::GetPacketToParse()
{
	return pcappPacketToParse;

}



/********************************************************************************
FunctionName : OpenSessionToParse
Description : The function will open the adapter and return the opened session pointer which
				we can forward to capture packets and parse method.
Return Type : void
Parameters : None.
*********************************************************************************/
void CGetPackets::OpenSessionToParse()
{
	//predefined function to obtain the packets from an adapter.
	pcappPacketToParse = pcap_open_live( pcappAdapterToParse->name, 65536, 0, 1000,chpErrorBuffer);
		
	//Free all the adapters list, as now we have the specific adapter.
	pcap_freealldevs(pcappAdapterList);

	if(pcappPacketToParse != NULL)
	{
		printf("successs");
		
		//function call next.
		//GetPackets();
		
		//(Removed) objCParsePackets.ParsePacketData(pcappPacketToParse);
	}

	else
	{
		fprintf(stderr,"An error occured in pcap_findalldevs_ex function: %s\n", chpErrorBuffer);
	}

}





/********************************************************************************
FunctionName : GetPackets
Description : The function will capture packets needed to parse.
Return Type : void
Parameters : None.
*********************************************************************************/
void CGetPackets::GetPackets()
{

	// start to capture packets. pcap_loop is predifined method in the Winpcap Library.
    //pcap_loop(pcappPacketToParse, 0, packet_handler, NULL);
}



/************************************************************************
FunctionName : GetNumberOfAdapters
Description : The function will return the value of the pcappPacketToParse variable, 
				which will have the packets, which is to be parsed.
Return Type : int
Parameters : None.
************************************************************************/
int CGetPackets::GetNumberOfAdapters()
{
	return nNumberOfAdapters;
}


