//ServerTunnel.cpp
///////////////////////////////////////////////////////////////////////////////
#include "ServerTunnel.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/////////////////////////Callback functions////////////////////////////////////
/*Get the header length according to header struct*/
static void netGetDataBufLenCb(IN SOCKET hSocket, IN void *pHeaderBuf, 
							   OUT unsigned long *nDataLen);

/*net event*/
static void netHandleNetEvent(IN SOCKET hSocket, IN ETransportEvent eEvent, 
							  IN void *pDataBuf, IN unsigned long nDataLen, 
							  IN int nError, IN void *pContext);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

CServerTunnel::CServerTunnel()
{
	m_hSocket = INVALID_SOCKET;

	m_eventHandler = NULL;
	m_pContext = NULL;
}

CServerTunnel::~CServerTunnel()
{
}


int CServerTunnel::net_OpenSocket(IN ETransportType eType, 
									 IN unsigned short usPort, 
									 IN LPTRANSPORT_EVENT eventHandler, 
									 void *pContext)
{
	TransportStatus status = TRANSPORT_OK;
	
	unsigned long ulHeaderLen = MESSAGE_HEADER_LEN;

	/*Use this layer handle event and context, 
	save the application layer handle event and context*/
	m_eventHandler = eventHandler;
	m_pContext = pContext;

	eventHandler = netHandleNetEvent;
	pContext = this;
	
	LPTRANSPORT_GETDATABUFLEN cbHandler = netGetDataBufLenCb;
	SOCKET hSocket = CTransportImpl::net_OpenSocket(eType, usPort, 
											eventHandler, 
											cbHandler, 
											ulHeaderLen, 
											pContext);

	if( INVALID_SOCKET == hSocket )
	{
		return TRANSPORT_ERROR;
	}

	m_hSocket = hSocket;

	return status;
}

int CServerTunnel::net_CloseSocket()
{
	int nResult = CTransportImpl::net_CloseSocket();
	return nResult;
}

int CServerTunnel::net_Connect(IN unsigned long ulIPValue, IN unsigned short usPort)
{
	int nResult = TRANSPORT_OK;

	nResult = CTransportImpl::net_Connect(ulIPValue, usPort);
	
	return nResult;
}

unsigned long CServerTunnel::net_Send(IN SOCKET hSocket, 
									  IN void *pHeader, 
									  IN void *pMessage, 
									  IN unsigned long ulDataLen)
{
	if( INVALID_SOCKET == hSocket )
	{
		return SOCKET_ERROR;
	}

	unsigned long ulSendLen = 0;
	unsigned long ulBufLen = 0;

	TMessageHeader *pSendHeader = (TMessageHeader *)pHeader;

	ulBufLen = MESSAGE_HEADER_LEN + ulDataLen;	
	char *pSendBuf = new char[ulBufLen];
	memset(pSendBuf, 0, ulBufLen);

	TMessageHeader *pMessageHeader = (TMessageHeader *)pSendBuf;
	pMessageHeader->byVersion = 101;
	pMessageHeader->wHeaderFlag = MESSAGE_HEADER_FLAG;
	pMessageHeader->wMessageId = pSendHeader->wMessageId;
	pMessageHeader->wMessageSubId = pSendHeader->wMessageSubId;
	pMessageHeader->dwDataLen = ulDataLen;
	pMessageHeader->wReserve = 0;
	
	memcpy((void *)(pSendBuf+MESSAGE_HEADER_LEN), pMessage, ulDataLen);
	
	/* convert network word */	
	htons(pMessageHeader->wHeaderFlag);
	htons(pMessageHeader->wMessageId);
	htons(pMessageHeader->wMessageSubId);
	htonl(pMessageHeader->dwDataLen);
	htons(pMessageHeader->wCheckSum);
	htonl(pMessageHeader->wReserve);

	/*Send the message*/
	ulSendLen = CTransportImpl::net_Send(hSocket, (void *)pSendBuf, ulBufLen);
	
	/**/
	delete [] pSendBuf;
	pSendBuf = NULL;


	return ulSendLen;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////////////////////////////////callback functions/////////////////////////////
void netGetDataBufLenCb(IN SOCKET hSocket, IN void *pHeaderBuf, 
						OUT unsigned long *nDataLen)
{
	TMessageHeader *pHeader = NULL;
	
	*nDataLen = 0;
	
	pHeader = (TMessageHeader *)pHeaderBuf;
	if( NULL == pHeader )
	{
		return;
	}
	
	if( MESSAGE_HEADER_FLAG != pHeader->wHeaderFlag )
	{
		return;
	}
	
	unsigned long dwDataBufLen = pHeader->dwDataLen;
	ntohl(dwDataBufLen);
	*nDataLen = dwDataBufLen;
	
	return;
}

void netHandleNetEvent(IN SOCKET hSocket, IN ETransportEvent eEvent, 
					   IN void *pDataBuf, IN unsigned long nDataLen, 
					   IN int nError, IN void *pContext)
{
	/*Notify application layer*/
	CServerTunnel *pServerTunnel = (CServerTunnel *)pContext;
	if( NULL == pServerTunnel )
	{
		return;
	}

	void *pUserData = pServerTunnel->m_pContext;

	TransportBool bHandle = TRUE;
	
	if( Transport_ReadEv == eEvent )
	{
		if( nDataLen <= 0 )
		{
			return;
		}
				
				
		/*提取消息头，获得消息ID*/			
		TMessageHeader *pHeader = (TMessageHeader *)pDataBuf;

		/*提取消息内容*/
		char *pMessageBuf = (char *)((char *)pDataBuf + MESSAGE_HEADER_LEN);
		
		/*网络字节顺序的转换*/
		ntohs(pHeader->wHeaderFlag);
		ntohs(pHeader->wMessageId);
		ntohs(pHeader->wMessageSubId);
		ntohl(pHeader->dwDataLen);
		ntohs(pHeader->wCheckSum);
		ntohl(pHeader->wReserve);

		bHandle = (MESSAGE_HEADER_FLAG == pHeader->wHeaderFlag);
	}
	
	if( bHandle )
	{
		if( NULL != pServerTunnel->m_eventHandler )
		{
			pServerTunnel->m_eventHandler(hSocket, eEvent, 
											pDataBuf, nDataLen, 
											nError, pUserData);
		}
	}
}
///////////////////////////////////////////////////////////////////////////////