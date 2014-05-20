#pragma once

namespace Osiris {
	class Timer {
	protected:
		//	!----VARIABLES----!
		double			dTime;
		double			dStartTime;
		double			dEndTime;
		double			dTimeScale;
		bool			bRunning;

	public:
						Timer();

		void			Update();

		void			Start(double d_Length, double d_TimeScale);
		void			Start(double d_Length);
		void			Start();
		void			Pause();
		void			Resume();

		bool			Finished();

		double			GetTime();
		double			GetTimeLeft();
		double			GetTimeScale();

		void			SetTimeScale(double d_TimeScale);
	};
};

