#include <systemc.h>

using namespace std;

SC_MODULE(cpu2)
{
	sc_fifo_in<int> input;
	sc_inout<sc_uint<8>> hexes[6];
	
	void task1() {

		while (1) {
			sc_uint<8> progNo = input.read();
			if (progNo == 4) {
				cout << "Pomaranczowe" << endl;	
				hexes[5].write('-');
				hexes[4].write('-');
				hexes[3].write('-');
				hexes[2].write('2');
				hexes[1].write('-');
				hexes[0].write('-');
			}
			if (progNo == 17) {
				cout << "Pomaranczowe i czerwone" << endl;
				hexes[5].write('-');
				hexes[4].write('-');
				hexes[3].write('-');
				hexes[2].write('4');
				hexes[1].write('-');
				hexes[0].write('-');
			}
		}
		wait(10, SC_MS);
	}

	SC_CTOR(cpu2) {
		SC_THREAD(task1);
	}
};