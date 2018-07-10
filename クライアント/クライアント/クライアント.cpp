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

	// winsock2の初期化
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	// ソケットの作成
	sock = socket(AF_INET, SOCK_STREAM, 0);
	
	// 接続先指定用構造体の準備
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

			// connect()が成功したらloopを抜けます
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
		// inet_addr()が成功したとき
		if (connect(sock,
			(struct sockaddr *)&server,
			sizeof(server)) != 0) 
			{
				printf("connect : %d\n", WSAGetLastError());
				return 1;
			}
	}

	// サーバに接続
	connect(sock, (struct sockaddr *)&server, sizeof(server));
	while (1)
	{
		// サーバからデータを受信
		memset(buf, 0, sizeof(buf));
		int n = recv(sock, buf, sizeof(buf), 0);

		printf("%d, %s\n", n, buf);
	}
	// 本当はconnect()が成功したかどうかを判断してから次に進むべきです

	memset(buf, 0, sizeof(buf));
	int n = recv(sock, buf, sizeof(buf), 0);

	printf("%d, %s\n", n, buf);

	closesocket(sock);

	// winsock2の終了処理
	WSACleanup();

	return 0;
}