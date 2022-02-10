#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "facilities.h"

string m_dir = "bin/maps/";

void load_maps(bool debug)
{
	if(debug) cout << "Loading... " << endl;
	for (int i = 0; i < map_height; ++i)
	{
		if(debug) cout << "    Layer " << i << ": " << flush;
		map load_map;
		ifstream map_file = ifstream(m_dir + to_string(i));
		string curr_line;
		int line_counter = 0;
		while (getline(map_file, curr_line))
		{
			if(debug) cout << line_counter << " " << flush;
			if(line_counter < map_dim)
			{
				for (int j = 0; j < map_dim; ++j)
					load_map.paths[j][line_counter] = stoi(curr_line.substr(j * 2, 2), 0, 16);
				line_counter++;
			}
			else if(curr_line.size() == 4)
			{
				if(debug) cout << "item." << flush;
				int item_index = stoi(curr_line.substr(2, 2), 0, 16);
				if(debug) cout << item_index << " at (" << flush;
				int item_x = curr_line.at(0) - '0';
				int item_y = curr_line.at(1) - '0';
				if(debug) cout << item_x << ", " << item_y << ") " << flush;
				load_map.items.push_back(tuple<item, int, int>(items_lib[item_index].instantiate(), item_x, item_y));
				if(debug) cout << "added. " << flush;
			}
		}
		if(debug) cout << "done." << flush;
		maps.push_back(load_map);
	}
}
