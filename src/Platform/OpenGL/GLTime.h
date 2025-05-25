#pragma once

#include "Time/Time.h"

namespace RG
{
	class GLTime : public Time
	{
	public:
		virtual void OnUpdateBeginImpl() override;
		virtual void OnUpdateEndImpl() override;
		virtual double GetTimeImpl()override;
		virtual double GetDeltaTimeImpl() override;
	};
}