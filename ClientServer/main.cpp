
#define WIN32_LEAN_AND_MEAN

#include"../../std_lib_facilities.h"

#include<Windows.h>
#include<WS2tcpip.h>


//link with ws2_32. lib
#pragma comment (lib,"ws2_32.lib")



//default størrelsen  på bafferen 
//og default port 
#define default_buff  5024


const int port = 8080;

int main() {

	string ipAdress = "127.0.0.1"; //default: ip adress of the server


	//create the windSock
	WSADATA data;
	int wsOk = WSAStartup(MAKEWORD(2, 2), &data);

	//if the windsock can't start
	if (wsOk != 0) {
		cerr << "can't start windsock, error!" << wsOk << endl;
		return 1;
	}

	//creating the socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	//if the socket has error
	if (sock == INVALID_SOCKET) {
		cerr << "Can't  create the socket! " << WSAGetLastError() << endl;
		return 1;

	}

	//fill in the structre for ip adress
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAdress.c_str(), &hint.sin_addr);



	//connecting to the server
	int connServer = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connServer == SOCKET_ERROR) {
		cerr << "can't connect to the server! " << WSAGetLastError() << endl;
		//closing the sock
		closesocket(sock);
		WSACleanup();
		return 1;
	}

	//send and reiceive data 

	char sendBuff[default_buff];
	string ClientInput;

	do {
		cout << ">> ";
		getline(cin, ClientInput);

		//sending the text 
		if (ClientInput.size() > 0) {
			int sendresult = send(sock, ClientInput.c_str(), ClientInput.size() + 1, 0);
			if (sendresult != SOCKET_ERROR) {
				ZeroMemory(sendBuff, default_buff);
				int byteReceived = recv(sock, sendBuff, default_buff, 0);
				if (byteReceived > 0) {
					cout << "#SERVER# " << string(sendBuff, 0, byteReceived) << endl;

				}
			}

		}
	} while (ClientInput.size() > 0);

	//closing everything
	closesocket(sock);
	WSACleanup();


}