#include "PolyphaseSort.h"

template PolyphaseSort<Record>::PolyphaseSort(long long unsigned int, bool);
template PolyphaseSort<Record>::PolyphaseSort(std::string, bool);
template PolyphaseSort<Record>::~PolyphaseSort();
template void PolyphaseSort<Record>::initial_distribution(void);
template void PolyphaseSort<Record>::merge(void);
template void PolyphaseSort<Record>::sort();

template<typename T>
PolyphaseSort<T>::PolyphaseSort(long long unsigned int rec, bool p) : file{ rec }, print{ p }
{
	init_tapes();
	std::cout << file.file_name << ":\n";
	if(print)
		file.print_file();
}

template<typename T>
PolyphaseSort<T>::PolyphaseSort(std::string fname, bool p) : file{ fname }, print{ p }
{
	init_tapes();
	std::cout << file.file_name << ":\n";
	if (print)
		file.print_file();
}

template<typename T>
PolyphaseSort<T>::~PolyphaseSort()
{
	for(int i = 0; i < t; ++i)
		if (tapes[i]->series == 1)
		{
			tapes[i]->file.close();
			std::rename(tapes[i]->file_name.c_str(), ("s_" + file.file_name).c_str());
		}
}

template<typename T>
void PolyphaseSort<T>::sort()
{
	int phase = 1;
	std::cout << "\n\nInitial distribution\n\n";
	initial_distribution();
	long long unsigned int series = get_number_of_series();
	if (print)
		print_tapes();
	while (get_number_of_series() != 1)
	{
		std::cout << "Phase " << phase++ << "\n";
		std::cout << "------------------------------------------------------------------------\n";

		merge();

		if (print)
			print_tapes();
	}
	std::cout << "\nSeries: " << series << "\n";
	std::cout << "Phases: " << phase - 1 << "\n";
	print_disk_op();
}

template<typename T>
void PolyphaseSort<T>::init_tapes()
{
	for (int i = 0; i < t; ++i)
		tapes[i] = std::make_unique<Tape<T>>("Tape " + std::to_string(i));
}

template<typename T>
void PolyphaseSort<T>::initial_distribution()
{
	unsigned int c = 1; // current tape
	unsigned int prev_c = c;
	int series_saved = 0;
	while (!file.end_of_file())
	{
		T r = file.get_next_record();
		if (tapes[c]->prev.compare(r))
		{
			tapes[c]->series++;
			series_saved++;
			if (series_saved >= tapes[prev_c]->series) // change tape
			{
				series_saved = 0;
				prev_c = c;
				c++;
				if (c >= t)
					c = 1;
				if (!tapes[c]->prev.compare(r) && tapes[c]->series != 0) // series joining support
				{
					tapes[c]->series--;
					series_saved--;
				}
			}
		}
		tapes[c]->save_record(r);
		tapes[c]->prev = r;
	}
	series_saved++;
	tapes[c]->series++;
	tapes[c]->dummies = tapes[prev_c]->series - series_saved;
	for (int i = 1; i < t; ++i)
	{
		tapes[i]->save_buffer();
		tapes[i]->return_to_beg();
	}
	set_previous_to_min();
}

template<typename T>
void PolyphaseSort<T>::merge()
{
	const unsigned int s = get_tape_for_saving(); // s = tape for saving 
	const unsigned int m = get_shortest_tape();  // m = tape with smallest number of series != 0
	const unsigned int sa = tapes[m]->series;
	bool clear_all = true;
	for (int i = 0; i < t; ++i)
		if (tapes[i]->series != 1 && i != s)
			clear_all = false;

	int d = tapes[get_dummies_tape()]->dummies;
	if (d == 0)
		d = -1;

	std::vector<std::pair<T, int>> v2;
	std::vector<std::pair<T, int>> v;
	T min;
	min.min();
	for (int i = 0; i < t; ++i)
		if (i != s)
		{
			if (tapes[i]->prev.compare(min))
			{
				if(!tapes[i]->end_of_file())
					v.push_back(std::make_pair(tapes[i]->prev, i));
				else
					v.push_back(std::make_pair(tapes[i]->prev, -1));
			}
			else if (tapes[i]->dummies == 0)
				v.push_back(std::make_pair(tapes[i]->get_next_record(), i));
		}

	while (!tapes[m]->end_of_file() || tapes[s]->series != sa)
	{
		if (v.size() == 0) // end of series
		{
			tapes[s]->series++;
			if (tapes[s]->series == sa)
				break;
			if (d > 0)
				d--;
			v = v2;
			if (d == 0)
			{
				v.push_back(std::make_pair(tapes[get_dummies_tape()]->get_next_record(), get_dummies_tape()));
				d--;
			}
			v2.clear();
		}
		int to_save = get_min_rec(v);
		tapes[s]->save_record(v[to_save].first);

		int next = v[to_save].second;
		if (next != -1)
			tapes[next]->prev = v[to_save].first;
		v.erase(v.begin() + to_save);
		if (next == -1)
			continue;

		T r = tapes[next]->get_next_record();
		if (!tapes[next]->prev.compare(r))   // min <= r
		{
			if (tapes[next]->end_of_file())
				v.push_back(std::make_pair(r, -1));
			else
				v.push_back(std::make_pair(r, next));
		}
		else
		{
			tapes[next]->series--;
			if (tapes[next]->end_of_file())
			{
				v2.push_back(std::make_pair(r, -1));
				tapes[next]->prev = r;
			}
			else
				v2.push_back(std::make_pair(r, next));
		}
	}
	for (std::pair<T, int> x : v2)
		if (x.second != -1)
			tapes[x.second]->prev = x.first;

	tapes[get_dummies_tape()]->dummies = 0;
	tapes[s]->save_buffer();
	tapes[s]->return_to_beg();

	if (clear_all)
	{
		for (int i = 0; i < t; ++i)
			if (i != s)
				tapes[i]->clear();
	}
	else
		tapes[m]->clear();
}

template<typename T>
void PolyphaseSort<T>::print_disk_op() const
{
	long long unsigned int sum = 0;
	for (int i = 0; i < t; ++i)
		sum += tapes[i]->disk_operations;

	sum += file.disk_operations;
	std::cout << "Disk operations: " << sum << "\n";
}

template<typename T>
void PolyphaseSort<T>::print_tapes()
{
	T r;
	r.min();
	for (int i = 0; i < t; i++)
	{
		std::cout << tapes[i]->file_name << " Series - " << tapes[i]->series << "(" << tapes[i]->dummies << ")" <<" :\n";
		if (tapes[i]->series > 0)
		{
			if (tapes[i]->prev.compare(r))
				std::cout << tapes[i]->prev << " ";
			if (!tapes[i]->end_of_file())
				tapes[i]->print_file();
			else
				std::cout << "\n";
		}
		std::cout << "\n";
	}
	std::cout << "------------------------------------------------------------------------\n";
}

template<typename T>
long long unsigned int PolyphaseSort<T>::get_number_of_series() const
{
	long long unsigned int sum = 0;
	for (int i = 0; i < t; ++i)
		sum += tapes[i]->series;
	return sum;
}

template<typename T>
unsigned int PolyphaseSort<T>::get_dummies_tape() const
{
	for (unsigned int i = 0; i < t; ++i)
		if (tapes[i]->dummies != 0)
			return i;
	return 0;
}

template<typename T>
unsigned int PolyphaseSort<T>::get_tape_for_saving() const
{
	for (int i = 0; i < t; ++i)
		if (tapes[i]->series == 0)
		{
			return i;
		}
	return 0;
}

template<typename T>
unsigned int PolyphaseSort<T>::get_shortest_tape() const
{
	unsigned int m = get_tape_for_saving();
	m = (m + 1) % t;
	for (int i = 0; i < t; ++i)
	{
		if (tapes[i]->series + tapes[i]->dummies < tapes[m]->series + tapes[m]->dummies && i != get_tape_for_saving())
			m = i;
	}
	return m;
}

template<typename T>
unsigned int PolyphaseSort<T>::get_min_rec(const std::vector<std::pair<T, int>>& v) const
{
	T min = v[0].first;
	int to_save = 0;
	for (unsigned int i = 1; i < v.size(); ++i)
	{
		if (min.compare(v[i].first))
		{
			to_save = i;
			min = v[to_save].first;
		}
	}
	return to_save;
}

template<typename T>
void PolyphaseSort<T>::set_previous_to_min()
{
	for (int i = 0; i < t; ++i)
		tapes[i]->prev.min();
}
