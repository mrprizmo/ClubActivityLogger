#include "Event.h"

Event::Event(std::string t, const EventType tp, std::string client, int table)
		: time(std::move(t)), type(tp), clientName(std::move(client)),
		  tableNumber(table), isError(false), error() {}

Event::Event(std::string t, const ErrorType err)
	: time(std::move(t)), type(EventType::ERROR), error(err), isError(true) {}

std::string Event::toString() const {
	std::ostringstream oss;
	oss << time << " " << static_cast<int>(type);

	if (isError) {
		oss << " " << errorTypeToString(error);
	} else {
		if (type == EventType::CLIENT_SEATED || type == EventType::OUT_CLIENT_SEATED) {
			oss << " " << clientName << " " << tableNumber;
		} else if (type != EventType::ERROR) {
			oss << " " << clientName;
		}
	}
	return oss.str();
}