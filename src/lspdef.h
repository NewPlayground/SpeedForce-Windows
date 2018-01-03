#ifndef _PROVIDER_H_
#define _PROVIDER_H_ 

#include <ws2spi.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <mstcpip.h>

#include "lspcommon.h"

typedef struct _SOCKET_CONTEXT
{
    SOCKET              Socket;     // Lower provider socket handle
    PROVIDER           *Provider;   // Pointer to the provider from which socket was created

    SOCKADDR_STORAGE    ProxiedAddress;
    SOCKADDR_STORAGE    OriginalAddress;

    int                 AddressLength;

    BOOL                Proxied;

    LIST_ENTRY          Link;

} SOCKET_CONTEXT;

////////////////////////////////////////////////////////////////////////////////
//
// Spi.cpp prototypes
//
////////////////////////////////////////////////////////////////////////////////

void
FreeLspProviders(
        PROVIDER   *lspProvider,
        int         lspProviderCount,
        int        *lpErrno
        );

void
FindDestinationAddress( 
        SOCKET_CONTEXT *context, 
        const SOCKADDR *destAddr, 
        int             destLen,
        SOCKADDR      **proxyAddr, 
        int            *proxyLen
        );


int WSPAPI 
WSPCloseSocket(
        SOCKET s,        
        LPINT  lpErrno
        );

int WSPAPI 
WSPConnect(
        SOCKET                s,
        const struct sockaddr FAR * name,
        int                   namelen,
        LPWSABUF              lpCallerData,
        LPWSABUF              lpCalleeData,
        LPQOS                 lpSQOS,
        LPQOS                 lpGQOS,
        LPINT                 lpErrno
        );

int WSPAPI
WSPCleanup(
	LPINT lpErrno
	);

int WSPAPI
WSPSend(
	SOCKET          s,
	LPWSABUF        lpBuffers,
	DWORD           dwBufferCount,
	LPDWORD         lpNumberOfBytesSent,
	DWORD           dwFlags,
	LPWSAOVERLAPPED lpOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
	LPWSATHREADID   lpThreadId,
	LPINT           lpErrno
	);

int WSPAPI
WSPStartup(
	WORD                wVersion,
	LPWSPDATA           lpWSPData,
	LPWSAPROTOCOL_INFOW lpProtocolInfo,
	WSPUPCALLTABLE      UpCallTable,
	LPWSPPROC_TABLE     lpProcTable
	);

////////////////////////////////////////////////////////////////////////////////
//
// Sockinfo.cpp prototypes
//
////////////////////////////////////////////////////////////////////////////////

// Looks up the SOCKET_CONTEXT structure belonging to the given socket handle
SOCKET_CONTEXT *
FindSocketContext(
        SOCKET  s,
        BOOL    Remove = FALSE
        );

// Allocates a SOCKET_CONTEXT structure, initializes it, and inserts into the provider list
SOCKET_CONTEXT *
CreateSocketContext(
        PROVIDER  *Provider, 
        SOCKET     Socket, 
        int       *lpErrno
        );

// Frees a previously allocated SOCKET_CONTEXT structure
void 
FreeSocketContext(
        PROVIDER       *Provider,
        SOCKET_CONTEXT *Context
        );

// Frees all socket context strcutures belonging to the provider
void
FreeSocketContextList(
        PROVIDER   *Provider
        );

////////////////////////////////////////////////////////////////////////////////
//
// Extension.cpp prototypes
//
////////////////////////////////////////////////////////////////////////////////

BOOL PASCAL FAR 
ExtConnectEx(
    IN SOCKET s,
    IN const struct sockaddr FAR *name,
    IN int namelen,
    IN PVOID lpSendBuffer OPTIONAL,
    IN DWORD dwSendDataLength,
    OUT LPDWORD lpdwBytesSent,
    IN LPOVERLAPPED lpOverlapped
    );

////////////////////////////////////////////////////////////////////////////////
//
// External variable definitions
//
////////////////////////////////////////////////////////////////////////////////

extern CRITICAL_SECTION gCriticalSection;   // Critical section for initialization and 
extern INT              gLayerCount;        // Number of layered protocol entries for LSP
extern PROVIDER        *gLayerInfo;         // Provider structures for each layered protocol
extern WSPUPCALLTABLE   gMainUpCallTable;   // Upcall functions given to us by Winsock
extern GUID             gProviderGuid;      // GUID of our dummy hidden entry

#endif
