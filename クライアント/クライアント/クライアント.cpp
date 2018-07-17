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

	// winsock2の初期化
	if (WSAStartup(MAKEWORD(2, 0), &wsaData)!=0)
	{
		printf("ソケット初期化：失敗\n");
		return 1;
	}

	// ソケットの作成
	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock == INVALID_SOCKET)
	{
		printf("sock : %d\n", WSAGetLastError());
		return 1;
	}

	// 接続先指定用構造体の準備
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

	// サーバに接続
	connect(sock, (struct sockaddr *)&server, sizeof(server));
	while (1)
	{
		// サーバからデータを受信
		memset(buf, 0, sizeof(buf));
		int n = recv(sock, buf, sizeof(buf), 0);
		if (n > 0)
		{
			printf("%s", buf);
		}

		memset(buf, 0, sizeof(buf));
		scanf_s("%s", buf);

		// 文字送信
		n = send(sock, buf, 256, 0);

		if (n < 1)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				//まだできないよ
				printf("だめ\n");
			}
			else
			{
				printf("recevied data\n");
				printf("%s\n", buf);	
			}

			//とりあえず一一秒待ち
			Sleep(1000);
			printf("send : %d\n", WSAGetLastError());
			
		}
		
		if (n > 0)
		{
			printf("%d, %s\n", n, buf);
		}
	}
	// winsock2の終了処理
	WSACleanup();

	return 0;
}