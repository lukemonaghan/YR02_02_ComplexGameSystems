#include "Time.hpp"
#include "App.hpp"

#include <limits>

namespace Osiris {

	//!---CONSTRUCTORS---!
	Timer::Timer() {
		dTime		= 0.0f;
		dStartTime	= 0.0f;
		dEndTime	= 0.0f;
		dTimeScale	= 1.0f;
		bRunning	= true;
	}

	//!---METHODS---!
	void Timer::Update() {
		if (bRunning)
			dTime += App::GetDeltaTime() * dTimeScale;
	}

	void Timer::Start(double d_Length, double d_TimeScale) {
		if (d_Length <= 0)
			d_Length = std::numeric_limits<float>::infinity();
		dStartTime = dTime;
		dEndTime = dStartTime + d_Length;
		dTimeScale = d_TimeScale;
	}
	void Timer::Start(double d_length) {
		Start(d_length, 1);
	}
	void Timer::Pause() {
		bRunning = false;
	}
	void Timer::Resume() {
		bRunning = true;
	}

	bool Timer::Finished() {
		if (dTime - dStartTime >= dEndTime)
			return true;
		return false;
	}

	//!---GETTERS---!
	double Timer::GetTime() {
		return dTime - dStartTime;
	}
	double Timer::GetTimeLeft() {
		return dEndTime - dTime;
	}
	double Timer::GetTimeScale() {
		return dTimeScale;
	}

	//!---SETTERS---!
	void Timer::SetTimeScale(double f_TimeScale) {
		dTimeScale = f_TimeScale;
	}
}

