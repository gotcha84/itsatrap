#ifndef NETWORK_H
#define NETWORK_H

// Initialize WSA.
void InitWinsock2()
{
	WSADATA	wsaData;
	WORD version;
	int ret = 0;

	version = (MAKEWORD(2,0));
	ret = WSAStartup(version, &wsaData);
	if (ret != 0)
	{
		ret = WSAGetLastError();
		if (ret == WSANOTINITIALISED)
		{
			fprintf(stderr, "WSAStartup() failed"); 
			exit(1);
		}
	}
};

// Uninitialize WSA
void UnInitWinsock2()
{
	WSACleanup();
}

#endif