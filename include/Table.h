#pragma once
#include <string>
#include <stdexcept>
#include <iomanip>
#include <sstream>

class Table {
	int number;
	bool occupied;
	std::string currentClient;
	std::string busySince;
	int totalMinutes;
	int revenue;
	int hourCost;

	static int timeToMinutes(const std::string& time);

	static std::string minutesToTime(int mins);

public:
	Table(int num, int cost);

	void occupy(const std::string& client, const std::string& start);

	void release(const std::string& end);

	[[nodiscard]] bool isOccupied() const { return occupied; }
	[[nodiscard]] int getNumber() const { return number; }
	[[nodiscard]] int getRevenue() const { return revenue; }
	[[nodiscard]] std::string getUsageTime() const {
		return minutesToTime(totalMinutes);
	}
};