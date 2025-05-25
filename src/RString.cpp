#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#define _CRT_SECURE_NO_WARNINGS

#include "RString.h"
#include <string>

namespace RG
{
	RString::RString() : m_data(new char[1]), m_length(1), b_ref(false)
	{
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="string">new value</param>
	/// <param name="ref">should this object be a refrence to another</param>
	RString::RString(RString& string, bool ref) : b_ref(ref)
	{
		if (!b_ref)
		{
			m_length = string.m_length;
			m_data = new char[string.m_length];

			if(string.b_ref)
				std::strcpy(m_data, *(char**)(string.m_data));
			else
				std::strcpy(m_data, string.m_data);
		}
		else
		{
			m_length = string.m_length;
			m_data = (string.b_ref) ? *(char**)(string.m_data) : (char*)(&string.m_data);
		}
	}

	RString::RString(const char* data)
	{
		b_ref = false;
		m_length = std::strlen(data) + 1;
		m_data = new char[m_length];
		std::strcpy(m_data, data);
	}

	void RString::operator=(RString& string)
	{
		m_length = std::strlen(string.m_data) + 1;

		if (!b_ref)
		{
			delete[] m_data;
			m_data = new char[m_length];

			if (string.b_ref)
				std::strcpy(m_data, *(char**)(string.m_data));
			else
				std::strcpy(m_data, string.m_data);
		}
		else
		{
			delete[] *(char**)m_data;
			*(char**)m_data = new char[m_length];

			if (string.b_ref)
				std::strcpy(*(char**)m_data, *(char**)(string.m_data));
			else
				std::strcpy(*(char**)m_data, string.m_data);
		}
	}

	void RString::Resize(int newSize)
	{
	}

	RString::operator const char* () const
	{
		return (b_ref) ? (char*)*m_data : m_data;
	}

	RString::~RString()
	{
		if (!b_ref)
			delete[] m_data;
	}
}