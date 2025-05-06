#pragma once
#include <string>
#include <stdexcept>
#include <iomanip>
#include <sstream>

enum class EventType {
    CLIENT_ARRIVED = 1,
    CLIENT_SEATED = 2,
    CLIENT_WAITING = 3,
    CLIENT_LEFT = 4,
    OUT_CLIENT_LEFT = 11,
    OUT_CLIENT_SEATED = 12,
    ERROR = 13
};

enum class ErrorType {
    NOT_OPEN_YET,
    YOU_SHALL_NOT_PASS,
    PLACE_IS_BUSY,
    CLIENT_UNKNOWN,
    I_CAN_WAIT_NO_LONGER
};

inline std::string errorTypeToString(ErrorType error) {
    switch (error) {
        case ErrorType::NOT_OPEN_YET: return "NotOpenYet";
        case ErrorType::YOU_SHALL_NOT_PASS: return "YouShallNotPass";
        case ErrorType::PLACE_IS_BUSY: return "PlaceIsBusy";
        case ErrorType::CLIENT_UNKNOWN: return "ClientUnknown";
        case ErrorType::I_CAN_WAIT_NO_LONGER: return "ICanWaitNoLonger!";
        default: throw std::invalid_argument("Unknown error type");
    }
}

class Event {
    std::string time;
    EventType type;
    std::string clientName;
    int tableNumber{};
    ErrorType error;
    bool isError;

public:
    Event(std::string t, EventType tp, std::string client, int table = 0);

    Event(std::string t, ErrorType err);

    [[nodiscard]] std::string toString() const;

    [[nodiscard]] std::string getTime() const { return time; }
    [[nodiscard]] EventType getType() const { return type; }
    [[nodiscard]] std::string getClient() const { return clientName; }
    [[nodiscard]] int getTable() const { return tableNumber; }
    [[nodiscard]] bool isErrorEvent() const { return isError; }
    [[nodiscard]] ErrorType getError() const { return error; }
};
