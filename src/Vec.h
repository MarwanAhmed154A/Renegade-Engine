#pragma once

#include <vector>

namespace RG
{
	template<typename T>
	class Vec
	{
	public:
		Vec()
		{

		}

		T& Push(T element)
		{
			vec.push_back(element);
			return vec[vec.size() - 1];
		}

		void Delete(int index)
		{
			vec.erase(vec.begin() + index);
		}

		T& operator[](int index)
		{
			return vec[index];
		}

		int GetLength()
		{
			return vec.size();
		}
	private:
		std::vector<T> vec;
	};
}
