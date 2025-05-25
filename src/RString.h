#pragma once

namespace RG
{
	class __declspec(deprecated("Unafe! Do not use!")) RString
	{
	public:
		RString();
		RString(RString& string, bool ref = false);
		RString(const char* data);

		void Resize(int newSize);

		 operator const char* () const;
		 void operator=(RString& str);

		~RString();
	//private:
		char* m_data;
		int m_length;
		bool b_ref;
	};
}