
#ifndef __SOKOBAN_H__
#define __SOKOBAN_H__

// #include <iostream>
// #include <string>
#include <vector>
// #include <queue>
// #include <tuple>
// #include <array>
// #include <map>
// #include <boost/algorithm/string.hpp>
// #include <boost/unordered_set.hpp>

using namespace std;
//typedef vector<char> TableRow;
//typedef vector<TableRow> Table;
using TableRow = vector<char>;
using Table = vector<TableRow>;

struct Board {
    Table sData, dData;
    int px, py;
    Board(string lev);
    bool move(int x, int y, int dx, int dy, Table &data);
    bool push(int x, int y, int dx, int dy, Table &data);
    bool isSolved(const Table &data);
    string solve();
};

#endif