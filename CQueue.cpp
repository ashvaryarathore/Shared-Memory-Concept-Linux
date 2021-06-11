#include "CQueue.h"

CQueue *CQueue::objCQueue = NULL;

/***************************************************************************
FunctionName : GetInstance
Description : The function will return the object of the singleton class.
Return Type : CQueue*.
Paramter : 	None.
*****************************************************************************/
CQueue* CQueue::GetInstance()
{
	if(objCQueue == NULL)
	{
		objCQueue = new CQueue;
		printf("\n\nCreated a new object for CQueue....%p",objCQueue);
	}

	return objCQueue;

}


/***************************************************************************
Default constructor.
****************************************************************************/
CQueue::CQueue()
{
	//Initialize size variable.
        nQueueSize = 0;
}
