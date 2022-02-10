#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "facilities.h"

using namespace std;

void clear_screen()
{
	cout << string(100, '\n');
}

void sleep(int millis)
{
	this_thread::sleep_for(chrono::milliseconds(millis));
}

string get_input()
{
	cout << ">> ";
	string s;
	cin >> s;
	return s;
}

uint8_t rc4_shift(uint8_t x, uint8_t shift)
{
	return (x >> shift | x << 4 - shift) % 8;
}

void print_status_mesgs()
{
	for (vector<status>::iterator i = p_lost_stats.begin(); !p_lost_stats.empty() && i != p_lost_stats.end(); ++i)
	{
		cout << (*i).lose() << endl;
		p_lost_stats.erase(i);
	}
	for (vector<status>::iterator i = p_stats.begin(); !p_stats.empty() && i != p_stats.end(); ++i)
		cout << (*i).print() << endl;
}