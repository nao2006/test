#include <stdio.h>
#include <winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

int main()
{
	WSADATA wsaData;
	struct sockaddr_in server;
	SOCKET sock;
	char buf[32];


	// winsock2�̏�����
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	// �\�P�b�g�̍쐬
	sock = socket(AF_INET, SOCK_STREAM, 0);

	// �ڑ���w��p�\���̂̏���
	server.sin_family = AF_INET;
	server.sin_port = htons(12345);
	 InetPton(server.sin_family,L"127.0.0.1", &server.sin_addr.S_un.S_addr);

	// �T�[�o�ɐڑ�
	connect(sock, (struct sockaddr *)&server, sizeof(server));
	while (1)
	{
		// �T�[�o����f�[�^����M
		memset(buf, 0, sizeof(buf));
		int n = recv(sock, buf, sizeof(buf), 0);

		printf("%d, %s\n", n, buf);
	}
	// winsock2�̏I������
	WSACleanup();

	return 0;
}