#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <Winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define printf

int main()
{
	WSADATA wsaData;
	SOCKET sock0;
	struct sockaddr_in addr;
	struct sockaddr_in client;
	int len;
	SOCKET sock;
	int n;
	BOOL yes = 1;

	// winsock2の初期化
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
	{
		return 1;
	}

	// ソケットの作成
	sock0 = socket(AF_INET, SOCK_STREAM, 0);
	if (sock0 == INVALID_SOCKET) 
	{
		printf("socket : %d\n", WSAGetLastError());
		return 1;
	}

	// ソケットの設定
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	
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

	// TCPクライアントからの接続要求を待てる状態にする
	//listen(sock0, 5);

	while (1) 
	{
		// TCPクライアントからの接続要求を受け付ける
		len = sizeof(client);
		sock = accept(sock0, (struct sockaddr *)&client, &len);

		if (sock == INVALID_SOCKET) 
		{
			printf("accept : %d\n", WSAGetLastError());
			break;
		}

		// 5文字送信
		n = send(sock, "HELLO", 5, 0);

		if (n < 1) 
		{
			printf("send : %d\n", WSAGetLastError());
			break;
		}

		// TCPセッションの終了
		closesocket(sock);
	}
	// winsock2の終了処理
	WSACleanup();

	return 0;
}