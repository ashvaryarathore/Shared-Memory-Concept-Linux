#include <pcap.h>
#include "CReadPacketDump.h"
//#include <remote-ext.h>

/***************************************************************************
FunctionName : SetAdapterList
Description : The function will use the pcap library function to get the packets 
              from a dump file.The function will initialize the pcappPacketToParse 
	      with the pointer to the packet.
Return Type : void.
Paramters : None.
******************************************************************************/
void CReadPacketDump :: ReadPacketsFromDump()
{

	char chpFileName[] = "Sample.pcap";
	char chpErrorBuffer[ PCAP_ERRBUF_SIZE ]; //required to allocate the buffer of default size PCAP_ERRBUF_SIZE if error occurs.

	/*************************************************************
	Code to Open the capture file.
	*************************************************************/
	if ( (pcappPacketToParse = pcap_open_offline(chpFileName, chpErrorBuffer)) == NULL)
        {
                fprintf(stderr,"\nUnable to open the file %s.\n", chpFileName);
        }


	printf("\n In pcap readfrom dump...\n");  		
		 	
}//Funtion close.



/************************************************************************
FunctionName : GetPacketToParse
Description : The function will return the value of the pcappPacketToParse variable, 
		which will have the packets, which is to be parsed.
Return Type : pcap_if_t
Parameters : None.
************************************************************************/
pcap_t* CReadPacketDump :: GetPacketToParse()
{
	return pcappPacketToParse;

}//Function end.
	
