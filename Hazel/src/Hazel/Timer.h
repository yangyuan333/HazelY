#pragma once

#include "Hazel/Core.h"
#include <chrono>

namespace Hazel {

	class Timer {
	public:
		Timer(){
			Reset();
		}
		
		void Reset() {
			m_Start = GetNow();
		}

		static std::chrono::time_point<std::chrono::high_resolution_clock> GetNow() {
			return std::chrono::high_resolution_clock::now();
		}

		static float GetMilliTime(Timer t0, Timer t1) {
			return (t1 - t0) / 1000.0f;
		}

		static float GetTime(Timer t0, Timer t1) {
			return GetMilliTime(t0, t1) / 1000.0f;
		}

		int64_t operator-(Timer t) {
			return
				std::chrono::duration_cast<std::chrono::microseconds>(m_Start-t.m_Start).count();
		}

		int64_t operator-(std::chrono::time_point<std::chrono::high_resolution_clock> t) {
			return
				std::chrono::duration_cast<std::chrono::microseconds>(m_Start - t).count();
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};

}