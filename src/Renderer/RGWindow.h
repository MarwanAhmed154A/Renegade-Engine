#pragma once

#include "Maths/Vec2.h"

namespace RG
{
	class RGWindow
	{
	public:
		static RGWindow* Create(int width, int height, const char* name);

		virtual void Init() = 0;
		virtual void HandleEvents() = 0;
		virtual void Update() = 0;
		virtual void Clear(float R, float G, float B) = 0;
		virtual bool ShouldClose() = 0;
		virtual void* GetNativeWindow() = 0;

		Vec2 GetSize();
	protected:
		Vec2 size;
	};
}