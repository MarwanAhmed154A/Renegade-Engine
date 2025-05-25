#pragma once

namespace RG
{
	class Time
	{
	public:
		static void Init();
		static void OnUpdateBegin();
		static void OnUpdateEnd();
		static double GetTime();
		static double GetDeltaTime();
	protected:
		virtual void OnUpdateBeginImpl() = 0;
		virtual void OnUpdateEndImpl() = 0;
		virtual double GetTimeImpl() = 0;
		virtual double GetDeltaTimeImpl() = 0;

		double beginTime, endTime, deltaTime;
	private:
		static Time* s_handle;
	};
}