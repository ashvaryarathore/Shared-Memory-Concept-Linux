#include <pcap.h>

#define PCAP_BUF_SIZE 1024


/***************************************************
Class Name : CReadPacketDump
Purpose : The class will have methods to read
	  pcap file and provide the packet to parse.
****************************************************/

class CReadPacketDump 
{
	pcap_t  *pcappPacketToParse;
	char chpErrorBuffer[PCAP_ERRBUF_SIZE]; //required to allocate the buffer of default size PCAP_ERRBUF_SIZE if error occurs.
	//char source[PCAP_BUF_SIZE];// required to create a source stringf
	char chpFileName[15];
	

	public:

	/***************************************************************************
	FunctionName : SetAdapterList
	Description : The function will use the pcap library function to get the packets 
		      from a dump file. The function will initialize the pcappPacketToParse
		      with the pointer to the packet.
	Return Type : void.
	Paramters : None.
	******************************************************************************/
	void ReadPacketsFromDump();


	/************************************************************************
	FunctionName : GetPacketToParse
	Description : The function will return the value of the pcappPacketToParse variable, 
		      which will have the packets, which is to be parsed.
	Return Type : pcap_if_t
	Parameters : None.
	************************************************************************/
	pcap_t* GetPacketToParse();
	
};
