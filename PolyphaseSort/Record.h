#pragma once
#pragma pack(push, 1)
#include <cstdlib>
#include <ostream>
#include <istream>
#include <algorithm>

struct Record
{
	float p_a;
	float p_b;
	float p_ab;

	void random() // required
	{
		p_a = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
		p_b = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
		p_ab = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX / std::min({ p_a, p_b }));
	}

	Record min() // required
	{
		p_a = -1.0f;
		p_b = 1.0f;
		p_ab = -1.0f;
		return *this;
	}

	float conditional_probabiliy() const // P (A | B)
	{
		if (p_b == 0)
			return 0;
		return p_ab / p_b;
	}

	bool compare(const Record& r) const // required
	{
		if (conditional_probabiliy() > r.conditional_probabiliy())
			return true;
		else
			return false;
	}

	friend std::ostream& operator<<(std::ostream& os, const Record r)
	{
		os.precision(2);
		os << r.conditional_probabiliy();
		return os;
	}

	friend std::istream& operator>>(std::istream& is, Record& r)
	{
		is >> r.p_a >> r.p_b >> r.p_ab;
		return is;
	}
};

#pragma pack(pop)