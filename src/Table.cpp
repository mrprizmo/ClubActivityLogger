#include "Table.h"

int Table::timeToMinutes(const std::string& time) {
	const int hours = std::stoi(time.substr(0, 2));
	const int minutes = std::stoi(time.substr(3, 2));
	return hours * 60 + minutes;
}

std::string Table::minutesToTime(const int mins) {
	const int hours = mins / 60;
	const int minutes = mins % 60;
	std::ostringstream oss;
	oss << std::setw(2) << std::setfill('0') << hours << ":"
		<< std::setw(2) << std::setfill('0') << minutes;
	return oss.str();
}

Table::Table(const int num, const int cost)
	: number(num), occupied(false), totalMinutes(0),
	  revenue(0), hourCost(cost) {}

void Table::occupy(const std::string& client, const std::string& start) {
	occupied = true;
	currentClient = client;
	busySince = start;
}

void Table::release(const std::string& end) {
	if (!occupied) return;

	const int start = timeToMinutes(busySince);
	const int endTime = timeToMinutes(end);
	const int duration = endTime - start;

	totalMinutes += duration;
	const int hours = (duration + 59) / 60;
	revenue += hours * hourCost;

	occupied = false;
	currentClient.clear();
}
