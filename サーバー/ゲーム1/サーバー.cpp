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
	char buf[256];

	// winsock2の初期化
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	// ソケット作成
	sock0 = socket(AF_INET, SOCK_STREAM, 0);

	// ソケット設定
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(sock0, (struct sockaddr *)&addr, sizeof(addr));

	//TCPクライアントからの接続要求を持てる状態にします
	listen(sock0, 5);

	// TCPクライアントからの接続要求を受け付ける
	len = sizeof(client);
	sock = accept(sock0, (struct sockaddr *)&client, &len);

	// 文字送信
	n = send(sock, buf, 256, 0);

	//TCPセッションの終了
	closesocket(sock);

	//winsock2の終了処理
	WSACleanup();

	return 0l;
}