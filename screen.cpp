#include "screen.h"
#include<fstream>
#include<string>
#include<algorithm>
#include<execution>
#include<unordered_map>


const std::unordered_map<char, screen::flag> CHAR_TO_FLAG{
	{' ', screen::flag::EMPTY},
	{'.', screen::flag::GOAL},
	{'@', screen::flag::PLAYER},
	{'+', screen::flag::GOAL | screen::flag::PLAYER},
	{'$', screen::flag::BOX},
	{'*', screen::flag::GOAL | screen::flag::BOX},
	{'#', screen::flag::WALL},
	//handle unexpected newline char
	{'\n', screen::flag::WALL},
	{'\r', screen::flag::WALL},
};

screen::table::table(const table& other)
	:table()
{
	std::copy(std::execution::par_unseq, other.data[0], other.data[index_max + 1], data[0]);
}

void screen::table::fill(const flag val)
{
	std::fill(std::execution::par_unseq, data[0], data[index_max + 1], val);
}

void screen::readFromStream(std::istream& in)
{
	_cell.fill(flag::WALL);
	std::string buffer;
	index_type row = 2;
	while (std::getline(in, buffer)) {
		if (row > index_max - 2) [[unlikely]] throw buffer;
		if (buffer.size() > index_max - 3) [[unlikely]] throw buffer;
		index_type col = 2;
		for (auto x : buffer) {
			flag f = CHAR_TO_FLAG.at(x);
			_cell[row][col] = f;
			if (!!(f & flag::BOX)) _box.push_back(location(row, col));
			if (!!(f & flag::GOAL)) _goal.push_back(location(row, col));
			++col;
		}
		++row;
	}
}

screen::screen(std::istream& in) 
{
	readFromStream(in);
}

screen::screen(const std::string& filename)
{
	std::ifstream ifs(filename, std::ios::in);
	readFromStream(dynamic_cast<std::istream&>(ifs));
}