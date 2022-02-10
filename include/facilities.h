#ifndef FACILITIES_H
#define FACILITIES_H

#include <string>
#include <vector>
#include <fstream>
#include <tuple>

using namespace std;

const int map_dim = 16;
const int map_height = 1;

class item
{
	string name;
	string desc;
	string tex_path = "";

protected:
	int metadata1;
	int metadata2;
	bool new_item;

public:
	item(string name, string desc, string tex_path)
	{
		this->name = name;
		this->desc = desc;
		this->tex_path = tex_path;
	}

	item instantiate() const
	{
		item return_item(name, desc, tex_path);
		cout << "instantiating... " << flush;
		return_item.metadata1 = 0;
		return_item.metadata2 = 0;
		return_item.new_item = true;
		return return_item;
	}

	ifstream tex_file()
	{
		return ifstream("bin/items/" + tex_path);
	}

	string get_name()
	{
		return name;
	}

	string get_desc()
	{
		return desc;
	}

	string get_tex_path()
	{
		return tex_path;
	}
};

class map
{
public:
	uint8_t paths[map_dim][map_dim]; //1's +y, 2's +x, 4's -y, 8's -x
	vector<tuple<item, int, int>> items;
};

class status
{
	string identifier;
	string mesg;
	string loss_mesg;

public:
	status(string identifier, string mesg, string loss_mesg)
	{
		this->identifier = identifier;
		this->mesg = mesg;
		this->loss_mesg = loss_mesg;
	}

	string print()
	{
		return mesg;
	}

	string lose()
	{
		return loss_mesg;
	}
};

extern vector<status> p_stats;
extern vector<status> p_lost_stats;
extern vector<map> maps;
extern vector<map>::iterator curr_map;
extern vector<item> p_items;
const vector<item> items_lib =
{
	item("Scrap of Paper", "A scrap of paper, with what appear to be specks of oil on it. Useless without a writing implement. In the corner a faintly pencilled 'H' is visible.", "paper"),
	item("Lump of Charcoal", "An unremarkable, soft lump of charcoal.", "charcoal"),
	item("Canteen", "An empty, lightly-rusted canteen, with a screw-off top that losened up after some struggling. It appears clean enough, but it might be a good idea to thoroughly wash it out before putting anything in it.", "canteen"),
	item("Sunglasses", "It's dark enough already...", "sunglasses"),
	item("Brass Key", "A small, clean - maybe polished - brass key. There don't appear to be many locks around. Or any doors, for that matter.", "key"),
	item("Pillbox", "A floral-decorated pillbox. When shook, something rattles inside.", "pillbox"),
	item("Pill", "An unmarked pill - or caplet, to be specific. Consuming it would be unwise.", "pill"),
	item("Empty Pillbox", "A floral-decorated pillbox. Empty now.", "pillbox")
};

//utils.cpp
void clear_screen();
void sleep(int millis);
void print_status_mesgs();
string get_input();
uint8_t rc4_shift(uint8_t x, uint8_t shift);

//main.cpp
void handle_input(string input);
void draw_dungeon(bool debug);

//load.cpp
void load_maps(bool debug);

#endif
