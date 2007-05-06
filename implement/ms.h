#include "dal.h"

class MS {
private:
	int ant1;
	int ant2;
	dalcomplex data[4][256];
	double time;
	double u;
	double v;
	double w;
public:
	int get_ant1();
	int get_ant2();
	dalcomplex get_data(int polarization, int channel);
	double get_time();
	double get_u();
	double get_v();
	double get_w();
};
