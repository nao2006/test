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
	int n = 0;
	char *deststr = nullptr;
	unsigned int **addrptr = nullptr;
	u_long val = 1;

	ioctlsocket(sock, FIONBIO, &val);
	
	// winsock2�̏�����
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
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

	if (server.sin_addr.S_un.S_addr != 0xffffffff)
	{
		struct hostent *host;

		host = gethostbyname(deststr);

		if (host == NULL)
		{
			if(WSAGetLastError() == WSAHOST_NOT_FOUND)
			{
				printf("host not found : %s\n", deststr);
			}
			return 1;
		}

		server.sin_addr.S_un.S_addr = *(unsigned int *)host->h_addr_list[0];

		//ad0drptr = (unsigned int **)host->h_addr_list;

		while (addrptr != nullptr)
		{
			server.sin_addr.S_un.S_addr = *(*addrptr);

			//connect()�����������烋�[�v�𔲂�
			if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == 0);
			{
				//break;
			}

			addrptr++;
			//connect�����s��������̃A�h���X�ł�����
		}

		//connect�����ׂĎ��s�����ꍇ����if���ɓ���
		if (addrptr == nullptr)
		{
			printf("connect	: %d\n", WSAGetLastError());
			return 1;
		}
	}
	else
	{
		//inet_addr()�����������Ƃ�
		// �T�[�o�ɐڑ�
		if (connect(sock, (struct sockaddr *)&server, sizeof(server)) != 0)
		{
			printf("connect : %d\n", WSAGetLastError());
			return 1;
		}
	}
	
	//�{����connect()�������������ǂ����𔻒f���Ă��玟�ɐi��

	// �T�[�o����f�[�^����M
	memset(buf, 0, sizeof(buf));
	n = recv(sock, buf, sizeof(buf), 0);

	if (n < 0)
	{
		printf("recv : %d\n", WSAGetLastError());
		return 1;

		// �������M
		n = send(sock, buf, 256, 0);
	}

	printf("%d, %s\n", n, buf);

	
	closesocket(sock);

	// winsock2�̏I������
	WSACleanup();

	return 0;
}
