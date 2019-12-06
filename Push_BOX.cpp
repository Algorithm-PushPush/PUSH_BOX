//============================================================================
// Name        : Push_BOX.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <ncurses.h>
#include <stdlib.h>
#include"Push_BOX.hpp"
#include <string>
#include <unistd.h>
#include "sokoban.hpp"
#include <fstream>

using namespace std;

void MapInfo::load_solve(){
	string & ref = level_r = "";
	for(int i = 0; i < h; i++){
		for(int j = 0; j < w; j++){
			switch(map[i][j]){
				case 0:
					ref += ' ';
					break;
				case 1:	case 6:
					ref += '#';
					break;
				case 2:
					ref += '.';
					break;
				case 4:
					ref += '$';
						break;
				case 5:
					ref += '@';
					break;
			}
		}
		ref += '\n';
	}
    Board board(ref);
	solve = board.solve();

	index = 0;
}

PushPush::~PushPush(){
	delete mapInfo;
}

void PushPush::loadMap(char * inputDir){
	ifstream f(inputDir);
	int h, w;
	f >> h >> w;
	mapInfo = new MapInfo(h, w);

	for(int i = 0; i < h; i++){
		for(int j = 0; j < w; j++){
			char c;
			f >> c;
			mapInfo->map[i][j] = c - '0';
		}
	}
}

void PushPush::Level() {
	clear();
	clearStage = 0;
	glocation = 10;

	mvprintw(2, 1, "MOVE = KEY_PAD. RESTART = R. NEXT MAP = M, EXIT-Q");
	
	int x = 0, y = 0;;
	wbox = 0;
	
	for (y = 0; y < mapInfo->h; y++) {
		for (x = 0; x < mapInfo->w; x++) {//맵이라는 배열 전체를 쭉 훑는 반복문
			switch (mapInfo->map[y][x]) {//맵의 원소에 따라서 화면에 출력하는 것이 달라짐.
			case 0://맵 배열의 원소가 0이면 빈공간
				mvaddch(y + 4, x + 10, '-'|COLOR_PAIR(4));
				break;
			case 1://맵 배열의 원소가 1이면 벽생성.
				mvaddch(y + 4, x + 10, '#'|COLOR_PAIR(1));
				break;
			case 2://맵 배열의 원소가 2이면 목표지점.
				mvaddch(y + 4, x + 10, 'x'|COLOR_PAIR(2));
				clearStage += 1;
				glocation += 1;
				obj[glocation].yPosition = y + 4;
				obj[glocation].xPosition = x + 10;
				break;
			case 4://맵 배열의 원소가 4이면, 우선 빈공간을 할당하고.
				mvaddch(y + 4, x + 10, '-'|COLOR_PAIR(4));
				wbox += 1;
				obj[wbox].ozn = mvinch(y + 4, x + 10);//그 위에 상자라는 객체를 덮는다. 따라서 상자객체가 이동하게되면 자동적으로 빈공간이 다시 할당된다.
				obj[wbox].yPosition = y + 4;
				obj[wbox].xPosition = x + 10;
				obj[wbox].zn = '@';
				mvaddch(obj[wbox].yPosition, obj[wbox].xPosition,obj[wbox].zn|COLOR_PAIR(5));
				break;
			case 5://이 것 또한 마찬가지로, 캐릭터 객체를 빈공간 위에 덮는 방식이다.
				mvaddch(y + 4, x + 10, '-'|COLOR_PAIR(4));
				obj[0].ozn = mvinch(y + 4, x + 10);
				obj[0].yPosition = y + 4;
				obj[0].xPosition = x + 10;
				obj[0].zn = '8';
				mvaddch(obj[0].yPosition, obj[0].xPosition,
						obj[0].zn|COLOR_PAIR(3));
				break;//obj[0] 는 캐릭터다.
			}
		}
	}
	move(obj[0].yPosition, obj[0].xPosition);	
}


void PushPush::Play(){
	bool restart = false;
	chtype up, lf, dw, rg, oup, olf, odw, org;
	up = (mvinch(obj[0].yPosition-1,obj[0].xPosition) & A_CHARTEXT);//캐릭터의 한 칸위 좌표에 존재하는 문자를 숫자형태로 반환하여 가져옴.
	lf = (mvinch(obj[0].yPosition,obj[0].xPosition-1) & A_CHARTEXT);
	dw = (mvinch(obj[0].yPosition+1,obj[0].xPosition) & A_CHARTEXT);
	rg = (mvinch(obj[0].yPosition,obj[0].xPosition+1) & A_CHARTEXT);
	oup = (mvinch(obj[0].yPosition-2,obj[0].xPosition) & A_CHARTEXT);//캐릭터의  칸위 좌표에 존재하는 문자를 숫자형태로 반환하여 가져옴.
	olf = (mvinch(obj[0].yPosition,obj[0].xPosition-2) & A_CHARTEXT);
	odw = (mvinch(obj[0].yPosition+2,obj[0].xPosition) & A_CHARTEXT);
	org = (mvinch(obj[0].yPosition,obj[0].xPosition+2) & A_CHARTEXT);

	for (int o = 0; o <= wbox; o++) {
		mvaddch(obj[o].yPosition, obj[o].xPosition, obj[o].ozn);
	}//화면 상에 상자를 표시하는 for문.
	switch (mapInfo->solve[mapInfo->index++]) {
	// case KEY_UP:
	case 'u':
	case  'U':
		step++;
		mvprintw(5, 30, "Number of steps for now -> %d", step); // 화면상에 이동한 횟수를 표시

		if (up != '#') {//아스키 코드로 35 = '#', 64 = '@', 45 = '-', 120 = 'x' 위에 막힌 공간이 없을 시.
			if (up == '@' && (oup == '-' || oup == 'x')) {// 캐릭터 위에 상자가 존재하고, 그 상자 위에 빈 공간이나 목표지점있을 시.
				bool decide = false; // 목표지점과 일치하는 지 확인해주는 변수
				for (int i = 11; i <= glocation; i++){
					if(obj[0].yPosition-1 == obj[i].yPosition && obj[0].xPosition == obj[i].xPosition){
						decide = true; // 캐릭터 위에 목표지점이 있을 시 bool값을 true로 변경.
					}
				}
				if(decide == false){ // 상자를 목표지점에 넣었을 때, 넣어야 하는 상자의 개수를 빼준다.
					if(oup == 'x'){
						clearStage--;
					}
				}
				else{ //
					if(oup == '-'){ // 상자를 목표지점에서 다시 위로 꺼냈을 때, 넣어야하는 상자 개수를 다시 늘려준다.
						clearStage++;
					}
				}
				obj[0].yPosition -= 1; //캐릭터 위치를 위로 변경.

				for (int o = 1; o <= wbox; o++) {
					if ((obj[0].yPosition == obj[o].yPosition)
							&& (obj[0].xPosition == obj[o].xPosition)) {//캐릭터와 상자가 겹치게된다면, 상자를 위로 올리는 조건식.
						++countPush;// 상자를 밀었을 때 민 횟수를 증가시킨다.
						mvprintw(15, 30, "Number of Push for now -> %d", countPush); //화면 상에 상자를 민 횟수를 표시.
						obj[o].yPosition -= 1;//상자의 위치변경.
					}
				}
			} else if (up != '@')//위에 벽은 아니고, 캐릭터위에 상자가없다면, 단순하게 캐릭터의 위치만 이동.
				obj[0].yPosition -= 1;
		}//맨 처음 if문에 걸리지않는 경우. 즉, 위에 벽이 있는 경우 아무런 동작을 하지않는다.
		break;
	// case KEY_DOWN:
	case 'd':
	case 'D':
		step++;
		mvprintw(5, 30, "Number of steps for now -> %d", step);
		if (dw != '#') {
			if (dw == '@' && (odw == '-' || odw == 'x')) {
				bool decide = false;
				for (int i = 11; i <= glocation; i++){
					if(obj[0].yPosition+1 == obj[i].yPosition && obj[0].xPosition == obj[i].xPosition){
						decide = true;
					}
				}
				if(decide == false){
					if(odw == 'x'){
						clearStage--;
					}
				}
				else{
					if(odw == '-'){
						clearStage++;
					}
				}
				obj[0].yPosition += 1;
				for (int o = 1; o <= wbox; o++) {
					if ((obj[0].yPosition == obj[o].yPosition)
							&& (obj[0].xPosition == obj[o].xPosition)) {
						++countPush;
						mvprintw(15, 30, "Number of Push for now -> %d", countPush);
						obj[o].yPosition += 1;
					}
				}
			}
			else if (dw != '@')
				obj[0].yPosition += 1;
		}
		break;
	// case KEY_LEFT:
	case 'l':
	case 'L':
		step++;
		mvprintw(5, 30, "Number of steps for now -> %d", step);
		if (lf != '#') {
			if (lf == '@' && (olf == 45 || olf == 120)) {
				bool decide = false;
				for (int i = 11; i <= glocation; i++){
					if(obj[0].yPosition == obj[i].yPosition && obj[0].xPosition-1 == obj[i].xPosition){
						decide = true;
					}
				}
				if(decide == false){
					if(olf == 120){
						clearStage--;
					}
				}
				else{
					if(olf == 45){
						clearStage++;
					}
				}
				obj[0].xPosition -= 1;
				for (int o = 1; o <= wbox; o++) {
					if ((obj[0].yPosition == obj[o].yPosition)
							&& (obj[0].xPosition == obj[o].xPosition)) {
						++countPush;
						mvprintw(15, 30, "Number of Push for now -> %d", countPush);

						obj[o].xPosition -= 1;
					}
				}
			} else if (lf != 64)
				obj[0].xPosition -= 1;
		}
		break;
	// case KEY_RIGHT:
	case 'r':
	case 'R':
		step++;
		mvprintw(5, 30, "Number of steps for now -> %d", step);
		if (rg != '#') {
			if (rg == '@' && (org == 45 || org == 120)) {
				bool decide = false;
				for (int i = 11; i <= glocation; i++){
					if(obj[0].yPosition == obj[i].yPosition && obj[0].xPosition+1 == obj[i].xPosition){
						decide = true;
					}
				}
				if(decide == false){
					if(org == 120){
						clearStage--;
					}
				}
				else{
					if(org == 45){
						clearStage++;
					}
				}
				obj[0].xPosition += 1;
				for (int o = 1; o <= wbox; o++) {
					if ((obj[0].yPosition == obj[o].yPosition)
							&& (obj[0].xPosition == obj[o].xPosition)) {

						++countPush;
						mvprintw(15, 30, "Number of Push for now -> %d", countPush);
						obj[o].xPosition += 1;
					}
				}
			} else if (rg != '@')
				obj[0].xPosition += 1;
		}
		break;
	}

	if (!restart) {
		for (int o = 0; o <= wbox; o++) {
			obj[o].ozn = mvinch(obj[o].yPosition, obj[o].xPosition);//객체의 ozn에, 객체가 이동하기 전, 터미널 화면상에 있는 문자를 임시로 저장. 이후 객체가 다른 곳으로 움직였을 때, ozn을 통헤 빈공간이나 목표지점을 원상복구하는 역할이다.
			mvaddch(obj[o].yPosition, obj[o].xPosition,
					obj[o].zn | ((o==0)?COLOR_PAIR(3):COLOR_PAIR(5)));//터미널 상에, 좌표의 객체를 표현. 객체가 상자이면, COLOR_PAIR를 통해 '@'를 나타내고, 캐릭터일시 '8'을 나타낸다.
		}
		move(obj[0].yPosition, obj[0].xPosition);
		mvprintw(20,20, "Number of boxes left to complete -> %d", clearStage); // 화면상에 클리어 목표까지 남은 상자 개수 표시
	} else
		restart = false;
}