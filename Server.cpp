#include "Socket.h"
#include <fstream>

#define FILE "out.txt"

void printFile(string file) {
	ifstream in(file.c_str());

	string content(
		(istreambuf_iterator<char>(in)),
		istreambuf_iterator<char>()
	);

	cout << content << endl;
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

int main() {
	Socket sock;
	ofstream out(FILE);
	char sequence = 'A';
	int packet = 0;
	sock.receive(false);
	cout << sock.buf << endl;

	/*



	for (;;) {
		sock.receive(false);

		cout << "--------------------" << endl;
		cout << "Packet: " << packet << endl;
		cout << "Seq: " << sequence << endl;
		cout << sock.buf << endl;

		if (sock.buf[0] != sequence || sock.buf[1] != (int)checkSum(sock.buf)) { // bad packet
			if (sock.buf[0] != sequence) {
				cout << "NAK - Sequence Error" << endl;
			} else {
				cout << "NAK - Checksum Error" << endl;
			}

			char nak[BUFFER];
			nak[0] = sequence; // sequence
			nak[1] = checkSum(sock.buf); // received checksum
			nak[2] = -1; // NAK signal
			sock.respond(nak);
		}
		else { // good packet
			cout << packet << ": ACK" << endl;
			packet++;

			char ack[BUFFER];
			ack[0] = sequence; // sequence
			ack[1] = checkSum(sock.buf); // received checksum
			ack[2] = 1; // ACK signal
			sock.respond(ack);

			sequence = sequence == 'A' ? 'B' : 'A';

			// Copy content buffer to file
			for (int i = HEADER; i < BUFFER; i++) {
				if (sock.buf[i] == '#') {
					break;
				}
				out << sock.buf[i];
			}

			if (sock.buf[4] == '#') {
				break;
			}
		}
	}

	out.close();
	printFile("out.txt");
	*/
	return 0;
}
