#pragma once
#include<ios>
#include<string>
#include<vector>
#include<utility>
#include<limits>
#include "bitwise_flag_operator.h"
class screen
{
public:
	using FLAG_UNDERLYING_TYPE = uint8_t;
	enum struct flag :FLAG_UNDERLYING_TYPE
	{
		EMPTY = 0,
		WALL = 1,
		BOX = 2,
		GOAL = 4,
		PLAYER = 8,
		BLOCK_MASK = WALL | BOX
	};
	using index_type = uint8_t;
	//assert min() <= 0
	constexpr static size_t index_max = std::min<uintmax_t>(std::numeric_limits<index_type>::max(), std::numeric_limits<size_t>::max() - 1);
	struct table {
		flag (*data)[index_max + 1];
		table() :data{new flag[index_max + 1][index_max + 1] } {}
		~table() { if(data) delete[] data; }
		inline flag* operator[](size_t i) const {
			return data[i];
		}
		table(table&& other) noexcept{
			data = other.data;
			other.data = nullptr;
		}
		table(const table& other);
		void fill(const flag val);

	};
	using location = std::pair<index_type, index_type>;
protected:
	table _cell;
	std::vector<location> _box;
	std::vector<location> _goal;
public:
	inline const table& cell() const noexcept { return _cell; }
	inline const std::vector<location>& box() const noexcept { return _box; }
	inline const std::vector<location>& goal() const noexcept { return _goal; }
	void readFromStream(std::istream& in);
	screen(std::istream& in);
	screen(const std::string& filename);
};
