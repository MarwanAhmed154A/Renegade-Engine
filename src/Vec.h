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
			return vec.size();
			
		}

		operator std::vector<T>() { return vec; }

	private:
		std::vector<T> vec;
		friend class SerializationManager;
	};
}