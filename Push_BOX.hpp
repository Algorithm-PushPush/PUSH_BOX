/*
 * Push_BOX.h
 *
 *  Created on: 2019. 6. 15.
 *      Author: juwan
 */

#ifndef PUSH_BOX_H_
#define PUSH_BOX_H_

#define N 10
#include <stdlib.h>
#include <vector>
#include <string>
using namespace std;

struct Object {
	int xPosition;
	int yPosition;
	unsigned char zn;
	unsigned long ozn; //unsigned long이다.
};

struct MapInfo {
	int h, w, index;
	vector<vector<int>> map;
	string solve;
	string level_r;

	MapInfo(int _h, int _w)
	: h(_h), w(_w), map(h, vector<int>(w)){}
	void load_solve();
};

struct PushPush {
	MapInfo * mapInfo;
	struct Object obj[N] = { };

	int wbox = 0;
	int clearStage = 0;
	int glocation = 10;
	int step = 0;
	int countPush = 0;

	~PushPush();
	void loadMap(char * inputDir);
	void Level();
	void Play();
};
#endif /* PUSH_BOX_H_ */