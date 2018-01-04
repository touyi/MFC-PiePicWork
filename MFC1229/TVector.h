#pragma once
#include<vector>
#include<functional>
using std::vector;
using std::function;
template<typename T>
class TVector
{
public:
	void push_back(T&it) {
		vec.push_back(it);
	}
	void push_back(T&&it)
	{
		vec.push_back(std::move(it));
	}
	void clear(){
		vec.clear();
	}
	void for_each(function<bool(T&)> func)
	{
		for (auto it = vec.begin(); it != vec.end(); it++)
		{
			if (func(*it))
			{
				break;
			}
		}
	}
	void erase(T& it)
	{
		for (int i = 0; i < vec.size(); i++)
		{
			if (vec[i] == it)
			{
				vec.erase(vec.begin() + i);
				break;
			}
		}
	}
	void erase(int index)
	{
		vec.erase(vec.begin() + index);
	}
	T& operator[](int index) {
		//return arr[index];
		return vec[index];
	}
	int size()
	{
		//return arr;
		return vec.size();
	}
private:
	vector<T> vec;
};

