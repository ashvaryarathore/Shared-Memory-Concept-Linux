#include <stdio.h>
#include <queue>

using namespace std;

/************************************************************
Class name : CQueue.
Purpose : The class will have the queue required to store the
	  packet addresses. The class will be singleton.
*************************************************************/
class CQueue
{

	public:

	//reference of type CQueue.	
	static CQueue *objCQueue;

	//variable to store number of elements in the queue.
	int nQueueSize;

	//A queue to hold the packet addresses.
	queue <char *> queuePacketAddr;
		
	/***************************************************************************
	Private Default constructor.
	****************************************************************************/
	CQueue();

	
	/***************************************************************************
	FunctionName : GetInstance
	Description : The function will return the object of the singleton class.
	Return Type : CSessionManager*.
	Paramter : 	None.
	*****************************************************************************/
	static CQueue* GetInstance();



};
