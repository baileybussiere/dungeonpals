#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <chrono>
#include <thread>
#include <fstream>
#include <algorithm>
#include "facilities.h"

using namespace std;

uint8_t p_dir = 2; //0 +x, 1 -y, 2 -x, 3 +y
pair <int, int> p_pos (15, 11);

vector<status> p_stats;
vector<status> p_lost_stats;
vector<map> maps;
vector<map>::iterator curr_map;
bool debug{false};

void init()
{
	load_maps(debug);
	curr_map = maps.begin();
	p_stats.push_back(status("hurt_head", "Your head aches.", "Your head feels better."));
	p_lost_stats.push_back(status("intro", "", "You awaken in a dungeon. You remember nothing."));
}

int main()
{
	/*if(*argv=="1")*/ debug = false;
	init();
	cout << "You awaken in a dungeon." << endl;
	bool exit = false;
	while (!exit)
	{
		if(!debug) clear_screen();
		draw_dungeon(debug);
		print_status_mesgs();
		handle_input(get_input());
	}
	return 0;
}

bool can_walk()
{
	return (*curr_map).paths[p_pos.first][p_pos.second] & 1 << (p_dir + 1) % 4;
}

bool can_descend()
{
	return (*curr_map).paths[p_pos.first][p_pos.second] & 16;
}

bool can_ascend()
{
	return (*curr_map).paths[p_pos.first][p_pos.second] & 32;
}

vector<tuple<item, int, int>>::iterator is_item()
{
	return find_if((*curr_map).items.begin(), (*curr_map).items.end(), [&](const tuple<item, int, int> placed_item){return get<1>(placed_item) == p_pos.first && get<2>(placed_item) == p_pos.second;});
}

void pick_up(vector<tuple<item, int, int>>::const_iterator i)
{

}

void handle_input(string input)
{
	if (input == "h" || input == "help")
	{
		clear_screen();
		cout << "Turn left by typing 'l', 'left', or 'turn left'." << endl;
		sleep(200);
		cout << "Turn right by typing 'r', 'right', or 'turn right'." << endl;
		sleep(200);
		cout << "If there is a path available forward, you can walk forward by typing 'f', 'forward', 'w', or 'walk'." << endl;
		sleep(200);
		cout << "If there is a hole in the ground, you can descend by typing 'd', 'descend', 'down' or 'climb down'." << endl;
		sleep(200);
		cout << "If there is a ladder, you can ascend by typing 'a', 'ascend', 'u', 'up, or 'climb up'." << endl;
		sleep(200);
		cout << "If there is an item (only visible if you are in the same tile), you can pick it up by typing 'p', 'pick up', 't', or 'take'." << endl;
		sleep(200);
		cout << "Press any key to return to the game.";
		cin.get();
		cout << "Good luck.";
		sleep(500);
	}
	else if (input == "l" || input == "left" || input == "turn left")
		p_dir = (p_dir + 3) % 4;
	else if (input == "r" || input == "right" || input == "turn right")
		p_dir = (p_dir + 1) % 4;
	else if (input == "f" || input == "forward" || input == "w" || input == "w")
	{
		if (can_walk())
			p_pos = pair<int, int>(p_pos.first + ((p_dir & 1)? 0: (p_dir & 2)? -1: 1), p_pos.second + ((p_dir & 1)? ((p_dir & 2)? -1: 1): 0));
		else
			cout << "You cannot walk forward." << endl;
	}
	else if (input == "d" || input == "down" || input == "descend", input == "climb down")
	{
		if (can_descend())
			curr_map++;
		else
			cout << "There is no ladder to descend." << endl;
	}
	else if (input == "a" || input == "ascend" || input == "a" || input == "ascend" || input == "climb")
	{
		if (can_ascend())
			curr_map--;
		else
			cout << "There is no ladder to ascend." << endl;
	}
	else if ((input == "t" || input == "take" || input == "p" || input == "pick up"))
	{
		vector<tuple<item, int, int>>::const_iterator iter = is_item();
		if (iter != (*curr_map).items.end())
			pick_up(iter);
	}
	else
		cout << "You fumble confusedly, trying to do something unknown." << endl;
}

uint8_t get_view()
{
	uint8_t x = (*curr_map).paths[p_pos.first][p_pos.second];
	return rc4_shift(x, p_dir); //rotates the view, returning a number: 1's -> path to player's left, 2's -> path ahead of player, 4's -> path to player's right
}

string get_view_ahead()
{
	uint8_t x = (*curr_map).paths[p_pos.first + ((p_dir & 1)? 0: (p_dir & 2)? -1: 1)][p_pos.second + ((p_dir & 1)? ((p_dir & 2)? -1 : 1): 0)];
	x = rc4_shift(x, p_dir);
	string s = "bin/s_";
	if (x & 1)
		s += "l";
	if (x & 2)
		s += "f";
	if (x & 4)
		s += "r";
	return s;
}

void draw_dungeon(bool debug)
{
	if(debug) cout << "Drawing..." << endl;
	uint8_t view = get_view();
	bool l = view & 1; //is path left
	bool f = view & 2; //is path forward
	bool r = view & 4; //is path right
	ifstream d_left(l? "bin/pl": "bin/nl");
	ifstream d_right(r? "bin/pl": "bin/nr");
	ifstream d_lsep((l == f)? "bin/sep": (l? "bin/sepl": "bin/sepb"));
	ifstream d_rsep((f == r)? "bin/sep": (r? "bin/sepl": "bin/sepb"));
	ifstream d_mid(f? get_view_ahead(): "bin/s_none");
	ifstream d_item("bin/items/_none");
	ifstream d_top(can_ascend()? "bin/t_ladder": "bin/t_");
	vector<tuple<item, int, int>>::iterator item_ptr = is_item();
	bool item_flag = item_ptr != (*curr_map).items.end();
	if (item_flag)
		d_item = get<0>((*item_ptr)).tex_file();
	for (int i = 0; i < 24; ++i)
	{
		string line = "";
		string s;
		getline(d_left, s);
		line += s;
		getline(d_lsep, s);
		line += s;
		if (i >= 3 && (i < 20 || !item_flag))
		{
			getline(d_mid, s);
			line += s;
		}
		else if (i < 3)
		{
			getline(d_top, s);
			line += s;
		}
		else
		{
			getline(d_item, s);
			line += "          " + s + "          ";
		}
		getline(d_rsep, s);
		line += s;
		getline(d_right, s);
		line += s;
		cout << line << endl;
	}
}
