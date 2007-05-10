#include "lffe.h"

int LFFE::get_ant1(int n){
	return row[n].ant1;
}
int LFFE::get_ant2(int n) {
	return row[n].ant2;
}
dalcomplex LFFE::get_data(int n, int polarization, int channel){
	return row[n].data[polarization][channel];
}
double LFFE::get_time(int n){
	return row[n].time;
}
double LFFE::get_u(int n) {
	return row[n].u;
}
double LFFE::get_v(int n) {
	return row[n].v;
}
double LFFE::get_w(int n) {
	return row[n].w;
}
