#include "cs1.h"

int CS1::get_ant1(int n){
	return row[n].ant1;
}
int CS1::get_ant2(int n) {
	return row[n].ant2;
}
dalcomplex CS1::get_data(int n, int polarization, int channel){
	return row[n].data[polarization][channel];
}
double CS1::get_time(int n){
	return row[n].time;
}

double CS1::get_u(int n) {
	return row[n].u;
}
double CS1::get_v(int n) {
	return row[n].v;
}
double CS1::get_w(int n) {
	return row[n].w;
}
