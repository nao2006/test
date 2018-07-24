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
	int n;
	char *deststr = nullptr;
	unsigned int **addrptr;
	u_long val = 1;
	ioctlsocket(sock, FIONBIO, &val);

	// winsock2の初期化
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	// ソケットの作成
	sock = socket(AF_INET, SOCK_STREAM, 0);

	// 接続先指定用構造体の準備
	server.sin_family = AF_INET;
	server.sin_port = htons(12345);
	server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	// サーバに接続
	connect(sock, (struct sockaddr *)&server, sizeof(server));

	// サーバからデータを受信
	memset(buf, 0, sizeof(buf));
	int n = recv(sock, buf, sizeof(buf), 0);

	printf("%d, %s\n", n, buf);

	// 文字送信
	//n = send(sock, buf, 256, 0);


	// winsock2の終了処理
	WSACleanup();

	return 0;
}