#include "lffe.h"

int LFFE::get_ant1(){
	return ant1;
}
int LFFE::get_ant2() {
	return ant2;
}
dalcomplex LFFE::get_data(int polarization, int channel){
	return data[polarization][channel];
}
double LFFE::get_time(){
	return time;
}

double LFFE::get_u() {
	return u;
}
double LFFE::get_v() {
	return v;
}
double LFFE::get_w() {
	return w;
}
