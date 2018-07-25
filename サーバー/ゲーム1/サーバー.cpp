#define  _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Winsock2.h>
#include <WS2tcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

#define PORT 50001


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
	char buf[256];

	// winsock2の初期化
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
	{
		return 1;
	}

	// ソケット作成
	sock0 = socket(AF_INET, SOCK_STREAM, 0);

	if (sock0 == INVALID_SOCKET)
	{
		printf("socket : %d\n", WSAGetLastError());
		return 1;
	}

	// ソケット設定
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	//関連付け
	if (bind(sock0, (struct sockaddr *)&addr, sizeof(addr))!= 0)
	{
		printf("bind : %d\n", WSAGetLastError());
		return 1;
	}
	
	//TCPクライアントからの接続要求を持てる状態にします
	if (listen(sock0, 5) != 0)
	{
		printf("listen : %d\n", WSAGetLastError());
		return 1;
	}

	// TCPクライアントからの接続要求を受け付ける
	len = sizeof(client);
	while (sock == INVALID_SOCKET)
	{
		sock = accept(sock0, (struct sockaddr *)&client, &len);
		if (sock != INVALID_SOCKET)
		{
			break;
		}
	}

	//TCPクライアントのセッション事の処理
	while (true)
	{
		// 文字送信
		n = send(sock, buf, 256, 0);

		if (n < 1)
		{
			printf(" send : %d\n", WSAGetLastError());
		}

		printf("accepted connection from %s, port=%d\n",
			inet_ntoa(client.sin_addr), ntohs(client.sin_port));

		
		//TCPセッションの終了
		if (sock == INVALID_SOCKET)
		{
			closesocket(sock);
			break;
		}
	}

	//winsock2の終了処理
	WSACleanup();

	return 0l;
}