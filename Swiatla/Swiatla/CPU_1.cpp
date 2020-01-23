#include <systemc.h>
#include <conio.h>

using namespace std;

SC_MODULE(cpu1) {
	sc_inout<sc_uint<8>> switches;
	sc_inout<sc_uint<8>> hexes[6];
	sc_inout<sc_uint<8>> diodes;
	sc_fifo_out<int> output;


	

	bool er = false;
	bool er_read = false;
	int last = 0;
	int count;
	bool constructed = false;

	bool sw_states[6] = { false, false, false, false, false, false };


	void task1() {

		int in = 0;
		while (1) {
			wait(500, SC_NS);

			if (_kbhit()) {

				displayHEX();
				displayLED();
				displaySW();
				wait(25, SC_NS);

				cout << endl << endl<< "Wybierz opcje: ";
				cin >> in;

				if (in >= 0 && in <= 6)
				{
					wait(500, SC_NS);
					sc_uint<8> sw_state = switches.read() ^ toBitFlag(in);
					switches.write(sw_state);
					wait(500, SC_NS);
				}
				else
				{
					cout << "Blad! Zakres [0-6]" << endl;
				}
			}

			wait(500, SC_NS);
			sc_uint<8> sw_state = switches.read();

			if (in == 3 && diodes.read() != 4 && diodes.read() != 17&& diodes.read() != 0x80 ) {
				if (diodes.read() == 0)
				{
					diodes.write(4);
					output.write(4);
				}
				else if (diodes.read() == 1 )
				{
					diodes.write(17);
					output.write(17);
				}
				in = 0;
				
				
			}
			
			else if (in == 3 && diodes.read() == 17)
			{


				diodes.write(1);
				hexes[5].write('-');
				hexes[4].write('E');
				hexes[3].write('R');
				hexes[2].write('R');
				hexes[1].write('O');
				hexes[0].write('R');
				in = 0;

			}
			else if (in == 3 && diodes.read() == 4)
			{
				in = 0;
				diodes.write(sw_state);
				for (int i = 0; i < 6; i++)
					hexes[i].write('-');
			}

			else if ( diodes.read() == 4 && in == 1) {
				
				
					diodes.write(17);
					output.write(17);
					in = 0;



			}

			else if (diodes.read() == 17 && in == 1) {


				diodes.write(4);
				output.write(4);
				in = 0;



			}
			else if(in !=3 && in != 0 && (diodes.read() == 17 || diodes.read() == 4))
			{
				diodes.write(0x80);
				cout << "Error" << endl;
				hexes[5].write('-');
				hexes[4].write('E');
				hexes[3].write('R');
				hexes[2].write('R');
				hexes[1].write('O');
				hexes[0].write('R');
				in = 0;

			}
			else if (in != 3 && in != 0 && diodes.read() == 0x80 && sw_state[2] )
			{
				if (sw_state[0])
				{
					diodes.write(17);
					output.write(17);
					in = 0;
					
				}
				else 
				{
					diodes.write(4);
					output.write(4);
					in = 0;
				}


			}
		
			wait(500, SC_NS);
			count = 0;
			wait(500, SC_NS);
			

			

			sc_uint<8> oldest_bit = 0x80;
			wait(500, SC_NS);

			for (int i = 7; i >= 0; i--) {
				if (oldest_bit & sw_state) 
				count++;
				oldest_bit = oldest_bit >> 1;
			}

			wait(500, SC_NS);

			/* Error */
			if (count > 2 && sw_state[0]|| count > 1 && !sw_state[0]) {
				diodes.write(0x80);
				cout << "Error" << endl;
				hexes[5].write('-');
				hexes[4].write('E');
				hexes[3].write('R');
				hexes[2].write('R');
				hexes[1].write('O');
				hexes[0].write('R');

			
			}
			else if (count == 0 && !sw_state[0]) {
				diodes.write(sw_state);
				for (int i = 0; i < 6; i++)
					hexes[i].write('-');
			}
		
			wait(500, SC_NS);
			switches.write(sw_state);
			wait(100, SC_MS);
		}
	}

	void task2() {
		while (1) {
			sc_uint<8> progNo = switches.read();

		
			if (progNo == 1 && diodes.read() != 1) {

			
				diodes.write(1);
				hexes[5].write('-');
				hexes[4].write('E');
				hexes[3].write('R');
				hexes[2].write('R');
				hexes[1].write('O');
				hexes[0].write('R');
				
			}
			
			wait(10, SC_MS);
		}
	}

	void task3() {
		while (1) {
			sc_uint<8> progNo = switches.read();

		
			if (progNo == 2 && diodes.read() != 2 ) {

				cout << "Zielone" << endl;
				diodes.write(2);
				hexes[5].write('-');
				hexes[4].write('-');
				hexes[3].write('-');
				hexes[2].write('-');
				hexes[1].write('-');
				hexes[0].write('1');
			}

			if (progNo == 3 && diodes.read() !=9) {

				cout << "Czerwone" << endl;
				diodes.write(9);
			    
				hexes[5].write('-');
				hexes[4].write('-');
				hexes[3].write('-');
				hexes[2].write('-');
				hexes[1].write('-');
				hexes[0].write('3');
			}

			/////////////////////////////////////////////
			if (progNo == 9 && diodes.read() != 3) {

				cout << "Zielone" << endl;
				diodes.write(3);
				hexes[5].write('-');
				hexes[4].write('-');
				hexes[3].write('-');
				hexes[2].write('-');
				hexes[1].write('-');
				hexes[0].write('1');
			}

			if (progNo == 8 && diodes.read() != 8) {

				cout << "Czerwone" << endl;
				diodes.write(8);
				hexes[5].write('-');
				hexes[4].write('-');
				hexes[3].write('-');
				hexes[2].write('-');
				hexes[1].write('-');
				hexes[0].write('3');
			}


			////////////////////////////////////
			if (progNo == 32 && diodes.read() != 32) {

				cout << "Awaryjne" << endl;
				diodes.write(32);
				hexes[5].write('-');
				hexes[4].write('-');
				hexes[3].write('-');
				hexes[2].write('-');
				hexes[1].write('-');
				hexes[0].write('5');
			}

			if ( progNo == 33 && diodes.read() != 33) {

				cout << "Awaryjne" << endl;
				diodes.write(33);
				hexes[5].write('-');
				hexes[4].write('-');
				hexes[3].write('-');
				hexes[2].write('-');
				hexes[1].write('-');
				hexes[0].write('5');
			}
			/////////////////////////////////////////////
					////////////////////////////////////
			if (progNo == 16 && diodes.read() != 16) {

				cout << "Pomaranczowe i czerwone" << endl;
				diodes.write(16);
				hexes[5].write('-');
				hexes[4].write('-');
				hexes[3].write('-');
				hexes[2].write('-');
				hexes[1].write('-');
				hexes[0].write('4');
			}

			if (progNo == 17 && diodes.read() != 17) {

				cout << "Pomaranczowe" << endl;
				diodes.write(17);
				hexes[5].write('-');
				hexes[4].write('-');
				hexes[3].write('-');
				hexes[2].write('-');
				hexes[1].write('-');
				hexes[0].write('2');
			}
		
			wait(10, SC_MS);
		}
	}



	// Funkcja wyswietlania diod i ich statusow
	void displayLED() {
		sc_uint<8> diodes_state = diodes.read(); //odczyt diody
		sc_uint<8> old_bit = 0x80;
		cout << "################################################" << endl;
		cout << "# LED:| ";

		for (int i = 7; i >= 0; i--) {
			if (diodes_state & old_bit) {
				cout << "1";
				if (i == 0) cout << " | "; else cout << " || ";
			}
			else {
				cout << "0";
				if (i == 0) cout << " | "; else cout << " || ";
			}
			old_bit = old_bit >> 1;
		}

		cout << "#" << endl;
		cout << "# Nr.:| 8 || 7 || 6 || 5 || 4 || 3 || 2 || 1 | #" << endl;
		cout << "################################################" << endl;

	}

	// Funkcja wyswietlania switchy i ich statusow

	void displaySW() {
		sc_uint<8> switches_state = switches.read();
		sc_uint<8> old_bit = 0x80;
		cout << "# SWI:| ";

		for (int i = 7; i >= 0; i--) {
			if (switches_state & old_bit) {
				cout << "1";
				if (i == 0) cout << " | "; else cout << " || ";
			}
			else {
				cout << "0";
				if (i == 0) cout << " | "; else cout << " || ";
			}
			old_bit = old_bit >> 1;
		}
		cout << "#" << endl;
		cout << "# Nr.:| 8 || 7 || 6 || 5 || 4 || 3 || 2 || 1 | #" << endl;
		cout << "################################################" << endl; 
	}

	// Funkcja wyswietlania hexow
	void displayHEX() {
		if (!constructed) postConstruct();

		/* Odczytywanie hexow */
		sc_uint<8> hex[6];
		for (int i = 0; i < 6; i++)
			hex[i] = hexes[i].read();

		cout << "################################################" << endl;
		cout << "# HEX:          | ";
		for (int i = 5; i >= 0; i--) {
			if (hex[i] >= 0 && hex[i] <= 6) {
				cout << hex[i];
				if (i == 0) cout << " | "; else cout << " || ";
			}
			else if (hex[i] == (sc_uint<8>) 'E') {
				cout << "E";
				if (i == 0) cout << " | "; else cout << " || ";
			}
			else if (hex[i] == (sc_uint<8>) 'R') {
				cout << "r";
				if (i == 0) cout << " | "; else cout << " || ";
			}
			else if (hex[i] == (sc_uint<8>) 'O') {
				cout << "o";
				if (i == 0) cout << " | "; else cout << " || ";
			}
			else if (hex[i] == (sc_uint<8>) '1') {
				cout << "1";
				if (i == 0) cout << " | "; else cout << " || ";
			}
			else if (hex[i] == (sc_uint<8>) '2') {
				cout << "2";
				if (i == 0) cout << " | "; else cout << " || ";
			}
			else if (hex[i] == (sc_uint<8>) '3') {
				cout << "3";
				if (i == 0) cout << " | "; else cout << " || ";
			}
			else if (hex[i] == (sc_uint<8>) '4') {
				cout << "4";
				if (i == 0) cout << " | "; else cout << " || ";
			}
			else if (hex[i] == (sc_uint<8>) '5') {
				cout << "5";
				if (i == 0) cout << " | "; else cout << " || ";
			}
			else if (hex[i] == (sc_uint<8>) '6') {
				cout << "6";
				if (i == 0) cout << " | "; else cout << " || ";
			}
			else if (hex[i] == (sc_uint<8>) 'Z') {
				cout << "Z";
				if (i == 0) cout << " | "; else cout << " || ";
			}
			else if (hex[i] == (sc_uint<8>) '-') {
				cout << "-";
				if (i == 0) cout << " | "; else cout << " || ";
			}
		}
		cout << "#" << endl;
		cout << "# Nr.:          | 6 || 5 || 4 || 3 || 2 || 1 | #" << endl;
	}

	sc_uint<8> toBitFlag(int value) {

		switch (value) {
		case 0:
		case 1:
		case 2:
			return value;
		case 3:
			return 0x4;
		case 4:
			return 0x8;
		case 5:
			return 0x10;
		case 6:
			return 0x20;
		default:
			return 0;
		}

	}

	//czyszczenie hexa
	void postConstruct() {
		for (int i = 0; i < 6; i++)
			hexes[i].write((sc_uint<8>) ' ');
	}

	SC_CTOR(cpu1) {
		SC_THREAD(task1); // MENU + ERROR
		SC_THREAD(task2); //  osb³uga  strony
		SC_THREAD(task3); //  obs³uga œwiate³
		//SC_THREAD(task4); //  obs³uga œwiate³ pomaranczowe/czerone i pomaranczowe
	}
};