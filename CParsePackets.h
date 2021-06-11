#pragma once 

#include <pcap.h>
#include <netinet/in.h>

#define ETH_MAC_ADDRESS_LEN 6
#define ETHERNET_HEADER_LEN 14
#define PROTOCOL_TCP 6
#define PROTOCOL_UDP 17
#define PROTOCOL_TYPE_IPV4 2048
#define IP_HEADER_LENGTH 20
#define DATA_BITS 4


/***************************************************************
Name: Structure ip_address
Purpose: The Structure would contain all the 4 bytes IP address.
****************************************************************/
typedef struct stIPAddress{
   unsigned char cIPAddressByte1;
   unsigned char cIPAddressByte2;
   unsigned char cIPAddressByte3;
   unsigned char cIPAddressByte4;
}stIPAddress;



/***************************************************************************
Name: Structure Ethernet_Header
Purpose: The Structure would contain all the components of a Ethernet Header.
*****************************************************************************/
typedef struct stEthernetHeader{
	unsigned char	DestinationMacAddress[ETH_MAC_ADDRESS_LEN];	// Destination Mac Address
	unsigned char	SourceMacAddress[ETH_MAC_ADDRESS_LEN];		//Source Mac Address
	unsigned short	EthernetType;					//Ethernet Type - Length packet type ID field	*/
}stEthernetHeader;



/**************************************************************
Name: Structure IP_header
Purpose: The Structure would contain all the conponents of IPv4 
		 header
***************************************************************/
typedef struct stIPheader{
    unsigned char  uchVersion_HeaderLength;		// Version (4 bits) + Internet header length (4 bits)
    unsigned char  uchTypeOfService;			// Type of service 
    unsigned short usTotalLength;				// Total length 
    unsigned short usIdentification;			// Identification
    unsigned short usFlags_FragmentOffset;		// Flags (3 bits) + Fragment offset (13 bits)
    unsigned char  uchTimeToLive;				// Time to live
    unsigned char  uchProtocol;					// Protocol
    unsigned short uchHeaderChecksum;			// Header checksum
    stIPAddress  SourceIPAddress;				// Source address
    stIPAddress  DestinationIPAddress;			// Destination address
    unsigned int   unOption_Padding;			// Option + Padding
}stIPheader;




/**************************************************************
Name: Structure TCP_header
Purpose: The Structure would contain all the conponents of TCP
		 header
***************************************************************/
typedef struct stTCPheader{
 	unsigned short usSourcePort;						// Source port ( 16 bits -> 2 bytes)
    	unsigned short usDestinationPort;					// Destination port ( 16 bits -> 2 bytes)
	unsigned int unSequenceNumber;				//Sequence Number (32 bits -> 4 bytes)
	unsigned int unAcknowledgementNumber;			//Acknowledgement Number (32 bits -> 4 bytes) 
	unsigned short usDataOffset_Reserved_Flags;		//Data offset(4 bits) + Reserved (3 bits) + Flags (9 bits) (aka Control bits) ( 16 bits --> 2 bytes)
	unsigned short usWindowSize;						//Window size (16 bits -> 2 bytes)
	unsigned short usChecksum;							//Checksum (16 bits -> 2 bytes)
	unsigned short usUrgentPointer;						//Urgent pointer (16 bits)

}stTCPheader;


class CParsePackets 
{


	public:

	/***************************************************************************
	FunctionName : ParsePacketData
	Description : The function will use the pcap library function to get the packets and  
				  packet data from the adapter that user has decided.
	Return Type : void.
	Paramter : u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data
	*****************************************************************************/
	void ParsePacketData(const char *pkt_data);


	/***************************************************************************
	FunctionName : CheckHttpHeaderInPayload
	Description : The function will check that the payload contains HTTP data or not.
				 The funtion taked initial 10 characters from the payload and will return 
				 1 on success, and 0 otherwise.
	Return Type : int.
	Paramter : char chTempHttpHeaderData[]
	*****************************************************************************/
	int CheckHttpHeaderInPayload(char chTempHttpHeaderData[]);
	
};

