#include <iostream>
#include <vector>
#include "Event.h"
#include "ComputerClub.h"
#include "InputParser.h"

int main() {
	int tableCount;
	std::string openTime, closeTime;
	int hourlyRate;
	std::vector<Event> events;

	if (!InputParser::parseInput(std::cin, tableCount, openTime, closeTime, hourlyRate, events)) {
		return 1;
	}

	ComputerClub club(tableCount, openTime, closeTime, hourlyRate);

	club.processEvents(events);

	club.close();

	club.printResults();

	return 0;
}