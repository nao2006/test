#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")


int main()
{
	WSADATA wsaData;
	struct sockaddr_in server;
	SOCKET sock;
	char buf[32];
	char *deststr = nullptr;

	if (__argc != 2) 
	{
		printf("Usage : %s dest\n", __argv[0]);
		return 1;
	}

	// winsock2の初期化
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	// ソケットの作成
	sock = socket(AF_INET, SOCK_STREAM, 0);

	// 接続先指定用構造体の準備
	server.sin_family = AF_INET;
	server.sin_port = htons(12345);
	server.sin_addr.S_un.S_addr = inet_addr(deststr);

	if (server.sin_addr.S_un.S_addr == 0xffffffff) 
	{
		struct hostent *host;

		host = gethostbyname(deststr);
		if (host == NULL) 
		{
			return 1;
		}
		server.sin_addr.S_un.S_addr =
			*(unsigned int *)host->h_addr_list[0];
	}


	InetPton(server.sin_family,L"127.0.0.1", &server.sin_addr.S_un.S_addr);

	// サーバに接続
	connect(sock, (struct sockaddr *)&server, sizeof(server));
	while (1)
	{
		// サーバからデータを受信
		memset(buf, 0, sizeof(buf));
		int n = recv(sock, buf, sizeof(buf), 0);

		printf("%d, %s\n", n, buf);
	}
	// winsock2の終了処理
	WSACleanup();

	return 0;
}