#include "dal.h"

/*
class CS1 {
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
*/

typedef struct CS1_ROW {
	int ant1;
	int ant2;
	dalcomplex data[4][256];
	double time;
	double u;
	double v;
	double w;
} CS1_ROW;

class CS1 {
private:
	CS1_ROW row[50];
public:
	int get_ant1(int n);
	int get_ant2(int n);
	dalcomplex get_data(int n, int polarization, int channel);
	double get_time(int n);
	double get_u(int n);
	double get_v(int n);
	double get_w(int n);
};
