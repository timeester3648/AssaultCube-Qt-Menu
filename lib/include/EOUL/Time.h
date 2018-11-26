#pragma once

#include <time.h>
#include <string>
#include <chrono>

namespace EOUL {

	namespace Util {

		class Timer {

			private:

				std::chrono::time_point<std::chrono::steady_clock> begin, end;
				bool started = false;

			public:

				/* starts timer */
				void start();
				/* stops timer */
				void stop();

				/* returns the time past since between the start() and stop() calls (in seconds) */
				double getDuration();

		};

		/* returns the current date */
		std::string date();

		/* returns the current second the clock is at */
		unsigned short second();
		/* returns the current minute the clock is at */
		unsigned short minute();
		/* returns the current hour the clock is at */
		unsigned short hour();
		/* returns the current day, if year based then returns the day from January 1st (0 - 365 else), otherwise it is from 0 - 6 (Sunday = 0) */
		unsigned short day(bool yearBased = false);
		/* returns the current month since January from 0 - 11 */
		unsigned short month();
		/* returns the current year */
		unsigned short year();

	}

}