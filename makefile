dungeonpals: src/main.cpp src/utils.cpp src/load.cpp include/facilities.h
	g++ -I include -o build/dungeonpals src/main.cpp src/load.cpp src/utils.cpp

test: build/dungeonpals
	build/dungeonpals
