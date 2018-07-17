#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
	WSADATA wsaData;
	struct sockaddr_in server;
	SOCKET sock = INVALID_SOCKET;
	char buf[256];
	char *deststr = nullptr;
	unsigned int **addrptr;
	u_long val = 1;
	ioctlsocket(sock, FIONBIO, &val);

	// winsock2�̏�����
	if (WSAStartup(MAKEWORD(2, 0), &wsaData)!=0)
	{
		printf("�\�P�b�g�������F���s\n");
		return 1;
	}

	// �\�P�b�g�̍쐬
	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock == INVALID_SOCKET)
	{
		printf("sock : %d\n", WSAGetLastError());
		return 1;
	}

	// �ڑ���w��p�\���̂̏���
	server.sin_family = AF_INET;
	server.sin_port = htons(12345);

	server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	if (server.sin_addr.S_un.S_addr == 0xffffffff) 
	{
		struct hostent *host;

		host = gethostbyname(deststr);
		if (host == NULL) 
		{
			WSAGetLastError() == WSAHOST_NOT_FOUND;

					printf("host not found : %s\n", deststr);
			
			return 1;
		}


		server.sin_addr.S_un.S_addr =
			*(unsigned int *)host->h_addr_list[0];

	}


	InetPton(server.sin_family,L"127.0.0.1", &server.sin_addr.S_un.S_addr);

	// �T�[�o�ɐڑ�
	connect(sock, (struct sockaddr *)&server, sizeof(server));
	while (1)
	{
		// �T�[�o����f�[�^����M
		memset(buf, 0, sizeof(buf));
		int n = recv(sock, buf, sizeof(buf), 0);
		if (n > 0)
		{
			printf("%s", buf);
		}

		memset(buf, 0, sizeof(buf));
		scanf_s("%s", buf);

		// �������M
		n = send(sock, buf, 256, 0);

		if (n < 1)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				//�܂��ł��Ȃ���
				printf("����\n");
			}
			else
			{
				printf("recevied data\n");
				printf("%s\n", buf);	
			}

			//�Ƃ肠�������b�҂�
			Sleep(1000);
			printf("send : %d\n", WSAGetLastError());
			
		}
		
		if (n > 0)
		{
			printf("%d, %s\n", n, buf);
		}
	}
	// winsock2�̏I������
	WSACleanup();

	return 0;
}