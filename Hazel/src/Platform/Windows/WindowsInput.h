#pragma once

#include "Hazel/Core.h"

#include "Hazel/Input.h"

namespace Hazel {

	class HAZEL_API WindowsInput :public Input 
	{	
	protected:
		bool IsKeyPressedImpl(KeyCode keycode) override;
		bool IsMouseButtonPressedImpl(int button) override;
		std::pair<float, float> GetMousePostionImpl() override;
		float GetMouseXImpl() override;
		float GetMouseYImpl() override;
	};


}