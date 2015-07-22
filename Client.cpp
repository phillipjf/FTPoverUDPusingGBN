#include "Socket.h"
#include <string>
#include <fstream>
#include <sstream>

//#define FILE "lorem_20000.txt"

string getFile(string file) {
	ifstream in (file.c_str());

	string content(
		(istreambuf_iterator<char>(in)),
		istreambuf_iterator<char>()
	);

	return content;
}

void printFile(string file) {
	ifstream in (file.c_str());

	string content(
		(istreambuf_iterator<char>(in)),
		istreambuf_iterator<char>()
	);

	cout << content << endl;
}

void getPacket(int packet, char* message, string content) {
	for (int i = 0; i < HEADER; i++) { // fill header
		message[i] = 100;
	}

	int index = packet*CONTENT;

	for (int i = 0; i < CONTENT; i++) {
		if (index+i < content.length()) {
			message[i+HEADER] = content.at(index+i);
		}
		else {
			message[i+HEADER] = '#';
		}
	}
}

char checkSum(char* message) {
	char sum = 0;

	for (int i = HEADER; i < BUFFER; i++) {
		sum += message[i];
	}

	if (sum == 0) { // a zero would signify end of array
		sum++;
	}

	return sum;
}

void setSequence(char* message, char sequence) {
	message[0] = sequence;
}

void setCheckSum(char* message) {
	message[1] = checkSum(message);
}

void setACK(char* message, bool ack) {
	message[2] = ack ? 1 : -1;
}

void gremlin(char* message, int packet, Socket sock, const string SERVER, int corrupt, int drop, char sequence) {
	bool send = true;
	cout << "--------------------" << endl;
	cout << "Packet: " << packet << endl;
	cout << "Seq: " << sequence << endl;
	//cout << packet << ": Seq: " <<sequence << " - Res: ACK" << endl;

	if (rand() % 100 < corrupt) {
		int random = rand() % 100;

		if (random < 10) { // 3 bits damaged
			message[6] = rand() % 93 + 33;
		}
		if (random < 30) { // 2 bits damaged
			message[5] = rand() % 93 + 33;
		}

		message[4] = rand() % 93 + 33; // 1 bit damaged

		cout << "Gremlin: Checksum" << endl;
	} else if (rand() % 100 < drop) {
		cout << "Gremlin: Drop Packet" << endl;
		send = false;
	}

	cout << "Sending ... " <<  message << endl;

	if (send) {
		sock.send(SERVER, message);
	}
}

int main(int argc, char * argv[]) {
	if (argc < 4 || argc > 6) {
		cout << "Usage: ./client ip corrupt drop [delay delay_time]" << endl;
		exit(1);
	}

	const string SERVER = argv[1];
	int corrupt = 0;
	int drop = 0;
	int delay = 0;
	int delay_time = 0;
	stringstream(argv[2]) >> corrupt;
	stringstream(argv[3]) >> drop;
	stringstream(argv[4]) >> delay_time;
	stringstream(argv[5]) >> delay_time;

	char sequence = 'A';
	char message[BUFFER];
	Socket sock;
	int packet = 0;
	string file;

	cout << "GET: ";
	cin >> file;
	strcpy(message, file.c_str());
	sock.send(SERVER, message);
	//string content = getFile(file);

	/*




	for (;;) {
		getPacket(packet, message, content); // fills content buffer with file
		setSequence(message, sequence); // add sequence byte
		setCheckSum(message); // calculate and add checkSum
		setACK(message, true); // set ACK

		gremlin(message, packet, sock, SERVER, corrupt, drop, sequence);

		if (sock.receive(true) == -1) { // timeout
			cout << "Res: No response, resending" << endl;
		} else if (sock.buf[2] == -1) {
			if (sequence != sock.buf[0]) { // sequence mismatch
				packet++;
				sequence = sequence == 'A' ? 'B' : 'A';
				cout << "Res: NAK, sequence error, next packet" << endl;

				if (message[12] == '#') {
					break;
				}	
			} else { // checksum
				cout << "Res: NAK, checksum error, resending" << endl; 
			}	
		} else {
			cout << "Res: ACK" << endl;

			sequence = sequence == 'A' ? 'B' : 'A'; // flip sequence
			packet++;

			if (message[4] == '#') {
				break;
			}	
		}
	}

	printFile(file);
	*/

	return 0;
}
