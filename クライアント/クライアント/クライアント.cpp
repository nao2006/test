#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")


int main(int argc, char *argv[])
{
	WSADATA wsaData;
	struct sockaddr_in server;
	SOCKET sock;
	char buf[32];
	char *deststr;
	unsigned int **addrptr;

	if (argc != 2)
	{
		printf("Usage : %s dest\n", argv[0]);
		return 1;
	}

	deststr = argv[1];

	// winsock2�̏�����
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	// �\�P�b�g�̍쐬
	sock = socket(AF_INET, SOCK_STREAM, 0);
	
	// �ڑ���w��p�\���̂̏���
	server.sin_family = AF_INET;
	server.sin_port = htons(12345);
	InetPton(server.sin_family,L"127.0.0.1", &server.sin_addr.S_un.S_addr);
	
	server.sin_addr.S_un.S_addr = inet_addr(deststr);

	if (server.sin_addr.S_un.S_addr == 0xffffffff)
	{
		struct hostent *host;

		host = gethostbyname(deststr);

		if (host == NULL)
		{
			return 1;
		}

		addrptr = (unsigned int **)host->h_addr_list;

		while (*addrptr != NULL)
		{
			server.sin_addr.S_un.S_addr = *(*addrptr);

			// connect()������������loop�𔲂��܂�
			if (connect(sock,
				(struct sockaddr *)&server,
				sizeof(server)) == 0)
			{
				break;
			}

			addrptr;

		}
	}
	else
	{
		// inet_addr()�����������Ƃ�
		if (connect(sock,
			(struct sockaddr *)&server,
			sizeof(server)) != 0) 
			{
				printf("connect : %d\n", WSAGetLastError());
				return 1;
			}
	}

	// �T�[�o�ɐڑ�
	connect(sock, (struct sockaddr *)&server, sizeof(server));
	while (1)
	{
		// �T�[�o����f�[�^����M
		memset(buf, 0, sizeof(buf));
		int n = recv(sock, buf, sizeof(buf), 0);

		printf("%d, %s\n", n, buf);
	}
	// �{����connect()�������������ǂ����𔻒f���Ă��玟�ɐi�ނׂ��ł�

	memset(buf, 0, sizeof(buf));
	int n = recv(sock, buf, sizeof(buf), 0);

	printf("%d, %s\n", n, buf);

	closesocket(sock);

	// winsock2�̏I������
	WSACleanup();

	return 0;
}