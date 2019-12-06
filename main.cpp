#include "Push_BOX.hpp"
#include <ncurses.h>
#include <iostream>
#include <unistd.h>

using namespace std;

void palette()
{
	init_color(COLOR_BLACK, 0, 0, 0);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_BLUE, COLOR_BLACK);
	init_pair(5, COLOR_CYAN, COLOR_BLACK);
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		cout << "Usage : PUSH input_dir" << endl;
		exit(1);
	}
	int testCases = 1;

	while (testCases <= argc)
	{

		PushPush *doublePush = new PushPush();

		doublePush->loadMap(argv[testCases]);

		int ch;
		initscr();
		keypad(stdscr, true);
		if (!has_colors())
		{
			endwin();
			cout << "ERROR initialising colors." << endl;
			exit(1);
		}

		start_color();
		palette();

		doublePush->mapInfo->load_solve();
		doublePush->Level();

		while ((ch = getch()) != 'q')
		{ // q키가 눌릴 때까지 실행을 반복한다.
			if (ch == 'r' || ch == 'R')
			{
				doublePush->mapInfo->load_solve();
				doublePush->Level();
			}

			doublePush->Play(); //, ch);
			if (doublePush->clearStage == 0)
			{ // 만약 클리어 목표 상자가 0이 되면 다음 맵으로 넘어간다.
				break;
			}
		}

		mvprintw(20, 20, "Game Finished\n");
		refresh();
		sleep(1);
		
		delete doublePush;
		testCases++;
	}
	endwin();

	return 0;
}