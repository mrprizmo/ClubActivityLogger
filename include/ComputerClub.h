#pragma once
#include <vector>
#include <queue>
#include <map>
#include "Event.h"
#include "Table.h"
#include "Client.h"
#include "TimeUtils.h"

class ComputerClub {
public:
	ComputerClub(int tablesNum, const std::string& open, const std::string& close, int rate);
	void processEvents(const std::vector<Event>& events);
	void close();
	void printResults() const;

	[[nodiscard]] const std::vector<Event>& getEventLog() const {
		return eventLog_;
	}

	[[nodiscard]] const std::vector<Table>& getTables() const {
		return tables_;
	}

private:
	int tableCount_;
	std::string openTime_;
	std::string closeTime_;
	int hourlyRate_;
	std::vector<Table> tables_;
	std::map<std::string, Client> clients_;
	std::queue<std::string> waitingQueue_;
	std::vector<Event> eventLog_;

	void processClientArrived(const Event& event);
	void processClientSeated(const Event& event);
	void processClientWaiting(const Event& event);
	void processClientLeft(const Event& event);
	void processWaitingQueue(const std::string& time, int tableNum);
	void logEvent(const Event& event);
	void logEventWithError(const Event& event, ErrorType error);
	bool isClientInClub(const std::string& clientName) const;
	int countFreeTables() const;
};