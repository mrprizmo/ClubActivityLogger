#pragma once
#include<string>
#include<cmath>

namespace TimeUtils {
	inline bool isValidTime(const std::string& time) {
		if (time.size() != 5 || time[2] != ':') return false;
		const int hours = std::stoi(time.substr(0, 2));
		const int minutes = std::stoi(time.substr(3, 2));
		return hours >= 0 && hours < 24 && minutes >= 0 && minutes < 60;
	}

	inline int timeToMinutes(const std::string& time) {
		return std::stoi(time.substr(0, 2)) * 60 + std::stoi(time.substr(3, 2));
	}

	inline bool isTimeInRange(const std::string& time,
							 const std::string& start,
							 const std::string& end) {
		return timeToMinutes(time) >= timeToMinutes(start) &&
			   timeToMinutes(time) <= timeToMinutes(end);
	}

	inline bool isTimeOrdered(const std::string& t1, const std::string& t2) {
		return timeToMinutes(t1) <= timeToMinutes(t2);
	}
};