#pragma once
#include <iostream>
#include <sstream>
#include "Event.h"
#include <vector>
#include <algorithm>
#include "TimeUtils.h"

class InputParser {
public:
    static bool parseInput(std::istream& input,
                          int& tableCount,
                          std::string& openTime,
                          std::string& closeTime,
                          int& hourlyRate,
                          std::vector<Event>& events) {
        std::string line;
        if (!std::getline(input, line) || !parsePositiveInt(line, tableCount)) {
            std::cout << line << std::endl;
            return false;
        }

        if (!std::getline(input, line) || !parseTimes(line, openTime, closeTime)) {
            std::cout << line << std::endl;
            return false;
        }

        if (!std::getline(input, line) || !parsePositiveInt(line, hourlyRate)) {
            std::cout << line << std::endl;
            return false;
        }

        std::string prevTime;
        while (std::getline(input, line)) {
            if (line.empty()) continue;

            std::istringstream iss(line);
            std::string time;
            int eventType;

            if (!(iss >> time >> eventType)) {
                std::cout << line << std::endl;
                return false;
            }

            if (!prevTime.empty() && !TimeUtils::isTimeOrdered(prevTime, time)) {
                std::cout << line << std::endl;
                return false;
            }
            prevTime = time;

            try {
                switch (eventType) {
                    case 1: {
                        std::string client;
                        if (!(iss >> client) || !isValidClientName(client))
                            throw std::invalid_argument("Invalid client name");
                        events.emplace_back(time, EventType::CLIENT_ARRIVED, client);
                        break;
                    }
                    case 2: {
                        std::string client;
                        int table;
                        if (!(iss >> client >> table) || !isValidClientName(client) || table < 1 || table > tableCount)
                            throw std::invalid_argument("Invalid seating");
                        events.emplace_back(time, EventType::CLIENT_SEATED, client, table);
                        break;
                    }
                    case 3: {
                        std::string client;
                        if (!(iss >> client) || !isValidClientName(client))
                            throw std::invalid_argument("Invalid client");
                        events.emplace_back(time, EventType::CLIENT_WAITING, client);
                        break;
                    }
                    case 4: {
                        std::string client;
                        if (!(iss >> client) || !isValidClientName(client))
                            throw std::invalid_argument("Invalid client");
                        events.emplace_back(time, EventType::CLIENT_LEFT, client);
                        break;
                    }
                    default:
                        throw std::invalid_argument("Unknown event type");
                }
            } catch (...) {
                std::cout << line << std::endl;
                return false;
            }
        }
        return true;
    }

private:
    static bool parsePositiveInt(const std::string& s, int& value) {
        try {
            size_t pos;
            value = std::stoi(s, &pos);
            return pos == s.size() && value > 0;
        } catch (...) {
            return false;
        }
    }

    static bool parseTimes(const std::string& line,
                          std::string& open,
                          std::string& close) {
        std::istringstream iss(line);
        return iss >> open >> close &&
               TimeUtils::isValidTime(open) &&
               TimeUtils::isValidTime(close);
    }

    static bool isValidClientName(const std::string& name) {
        return !name.empty() &&
               std::ranges::all_of(name, [](char c) {
                   return std::isalnum(c) || c == '_' || c == '-';
               });
    }
};
