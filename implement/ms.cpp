#include "ms.h"

int MS::get_ant1(){
	return ant1;
}
int MS::get_ant2() {
	return ant2;
}
dalcomplex MS::get_data(int polarization, int channel){
	return data[polarization][channel];
}
double MS::get_time(){
	return time;
}

double MS::get_u() {
	return u;
}
double MS::get_v() {
	return v;
}
double MS::get_w() {
	return w;
}
