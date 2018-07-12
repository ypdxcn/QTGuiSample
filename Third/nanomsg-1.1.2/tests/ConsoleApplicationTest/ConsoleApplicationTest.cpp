// ConsoleApplicationTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>

#include <cassert>
#include <cstdio>
#include <ctime>
#include "../../src/nn.h"
#include "../../src/pubsub.h" 
#include <iostream>

#pragma comment(lib, "nanomsg.lib")

#define SERVER "server"
#define CLIENT "client"


char *date()
{
	time_t raw = time(&raw);
	struct tm info;
	localtime_s(&info,&raw);
	char *text = asctime(&info);
	text[strlen(text) - 1] = '\0'; // remove '\n'
	return text;
}

int server(const char *url)
{
	int sock = nn_socket(AF_SP, NN_PUB);
	assert(sock >= 0);
	assert(nn_bind(sock, url) >= 0);
	while (1)
	{
		char *d = date();
		int sz_d = strlen(d) + 1; // '\0' too
		printf_s("SERVER: PUBLISHING DATE %s\n", d);
		int bytes = nn_send(sock, d, sz_d, 0);
		assert(bytes == sz_d);
		Sleep(20); // 1 s
	}
	return nn_shutdown(sock, 0);
}

int client(const char *url, const char *name)
{
	int sock = nn_socket(AF_SP, NN_SUB);
	assert(sock >= 0);
	// TODO learn more about publishing/subscribe keys
	assert(nn_setsockopt(sock, NN_SUB, NN_SUB_SUBSCRIBE, "", 0) >= 0);
	assert(nn_connect(sock, url) >= 0);
	while (1)
	{
		char *buf = NULL;
		int bytes = nn_recv(sock, &buf, NN_MSG, 0);
		assert(bytes >= 0);
		printf_s("CLIENT (%s): RECEIVED %s\n", name, buf);
		nn_freemsg(buf);
	}
	return nn_shutdown(sock, 0);
}

int main(int argc, char** argv)
{
	printf_s("Hello, insigmaudpserver...\n");

	printf_s("argc: %d\n", argc);
	const char *tmp = CLIENT;
	if (strncmp(tmp, "server", strlen(SERVER)) == 0 )
		return server("tcp://127.0.0.1:5560" );
	else if (strncmp(tmp,"client", strlen(CLIENT)) == 0 )
		return client("tcp://127.0.0.1:5560", "baby");
	else
	{
		fprintf(stderr, "Usage: pubsub %s|%s <URL> <ARG> ...\n",
			SERVER, CLIENT);
		return 1;
	}
	//return 0;
}