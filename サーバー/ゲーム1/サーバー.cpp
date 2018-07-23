#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Winsock2.h>
#include <WS2tcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")


int main()
{
	WSADATA wsaData;
	SOCKET sock0;
	struct sockaddr_in addr;
	struct sockaddr_in client;
	int len;
	SOCKET sock = INVALID_SOCKET;
	int n;
	BOOL yes = 1;

	// winsock2�̏�����
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
	{
		printf("�\�P�b�g�������F���s\n");
		return 1;
	}

	// �\�P�b�g�̍쐬
	sock0 = socket(AF_INET, SOCK_STREAM, 0);
	if (sock0 == INVALID_SOCKET) 
	{
		printf("socket : %d\n", WSAGetLastError());
		return 1;
	}

	// �\�P�b�g�̐ݒ�
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(sock, (struct sockaddr *)&addr, sizeof(addr));
	
	//�����Ńm���u���b�L���O�ɐݒ�
	//val = 0�Ńu���b�L���O���[�h�ɐݒ�
	//�\�P�b�g�̏����ݒ�̓u���b�L���O���[�h�ł�
	u_long val = 1;

	ioctlsocket(sock, FIONBIO, &val);

	setsockopt(sock0, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(yes));

	if (bind(sock0, (struct sockaddr *)&addr, sizeof(addr)) != 0) 
	{
		printf("bind : %d\n", WSAGetLastError());
		return 1;
	}

	if (listen(sock0, 5) != 0) 
	{
		printf("listen : %d\n", WSAGetLastError());
		return 1;
	}

	// TCP�N���C�A���g����̐ڑ��v����҂Ă��Ԃɂ���
	//listen(sock0, 5);

	while (1) 
	{
		// TCP�N���C�A���g����̐ڑ��v�����󂯕t����
		len = sizeof(client);
		sock = accept(sock0, (struct sockaddr *)&client, &len);


		printf("accepted connection from %s, port=%d\n",
			inet_ntoa(client.sin_addr), ntohs(client.sin_port));


		if (sock == INVALID_SOCKET) 
		{
			printf("accept : %d\n", WSAGetLastError());
			break;
		}

		char buf[256];

		while (true)
		{
			memset(buf, 0, sizeof(buf));

			scanf_s("%s", buf);

			// �������M
			n = send(sock, buf, 256, 0);
			
		}
		
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
				break;
			}

			Sleep(1000);
			printf("send : %d\n", WSAGetLastError());
			break;
		}
		
		// TCP�Z�b�V�����̏I��
		closesocket(sock);
	}
	// winsock2�̏I������
	WSACleanup();

	return 0;
}