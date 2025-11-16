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

		T& PushMove(T&& element)
		{
			vec.push_back(std::move(element));
			return vec[vec.size() - 1];
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
			return (int)vec.size();
		}

		void Resize(unsigned int size)
		{
			vec.resize(size);
		}

		operator std::vector<T>() { return vec; }

		std::vector<T>::iterator begin() { return vec.begin(); }
		std::vector<T>::iterator end()   { return vec.end();   }
	private:
		std::vector<T> vec;
		friend class SerializationManager;
	};
}