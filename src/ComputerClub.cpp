#include "ComputerClub.h"
#include <algorithm>
#include <iostream>

ComputerClub::ComputerClub(int tablesNum, const std::string& open, const std::string& close, int rate)
    : tableCount_(tablesNum), openTime_(open), closeTime_(close), hourlyRate_(rate) {
    for (int i = 1; i <= tablesNum; ++i)
        tables_.emplace_back(i, rate);
}

void ComputerClub::logEvent(const Event& event) {
    eventLog_.push_back(event);
}

void ComputerClub::logEventWithError(const Event& event, ErrorType error) {
    logEvent(event);
    logEvent(Event(event.getTime(), error));
}

bool ComputerClub::isClientInClub(const std::string& clientName) const {
    auto it = clients_.find(clientName);
    return it != clients_.end() && it->second.isInClub();
}

int ComputerClub::countFreeTables() const {
    return std::count_if(tables_.begin(), tables_.end(), [](const Table& t) { return !t.isOccupied(); });
}

void ComputerClub::processEvents(const std::vector<Event>& events) {
    for (const auto& event : events) {
        switch (event.getType()) {
            case EventType::CLIENT_ARRIVED: processClientArrived(event); break;
            case EventType::CLIENT_SEATED: processClientSeated(event); break;
            case EventType::CLIENT_WAITING: processClientWaiting(event); break;
            case EventType::CLIENT_LEFT: processClientLeft(event); break;
            default: break;
        }
    }
}

void ComputerClub::processClientArrived(const Event& event) {
    const std::string& client = event.getClient();

    if (!TimeUtils::isTimeInRange(event.getTime(), openTime_, closeTime_)) {
        logEventWithError(event, ErrorType::NOT_OPEN_YET);
        return;
    }

    if (isClientInClub(client)) {
        logEventWithError(event, ErrorType::YOU_SHALL_NOT_PASS);
        return;
    }

    clients_.emplace(client, Client(client));
    logEvent(event);
}

void ComputerClub::processClientSeated(const Event& event) {
    const std::string& client = event.getClient();
    int tableNum = event.getTable();

    if (!isClientInClub(client)) {
        logEventWithError(event, ErrorType::CLIENT_UNKNOWN);
        return;
    }

    Table& table = tables_[tableNum - 1];
    if (table.isOccupied()) {
        logEventWithError(event, ErrorType::PLACE_IS_BUSY);
        return;
    }

    Client& clientObj = clients_.at(client);
    if (clientObj.isSeated()) {
        Table& prevTable = tables_[clientObj.getTable() - 1];
        prevTable.release(event.getTime());
        processWaitingQueue(event.getTime(), prevTable.getNumber());
    }

    clientObj.seat(tableNum);
    table.occupy(client, event.getTime());
    logEvent(event);
}

void ComputerClub::processClientWaiting(const Event& event) {
    const std::string& client = event.getClient();

    if (!isClientInClub(client)) {
        logEventWithError(event, ErrorType::CLIENT_UNKNOWN);
        return;
    }

    if (countFreeTables() > 0) {
        logEventWithError(event, ErrorType::I_CAN_WAIT_NO_LONGER);
        return;
    }

    if (waitingQueue_.size() >= tableCount_) {
        logEvent(event);
        clients_.at(client).leave();
        logEvent(Event(event.getTime(), EventType::OUT_CLIENT_LEFT, client));
        return;
    }

    clients_.at(client).moveToQueue();
    waitingQueue_.push(client);
    logEvent(event);
}

void ComputerClub::processClientLeft(const Event& event) {
    const std::string& client = event.getClient();

    if (!isClientInClub(client)) {
        logEventWithError(event, ErrorType::CLIENT_UNKNOWN);
        return;
    }

    Client& clientObj = clients_.at(client);
    int freedTable = -1;
    if (clientObj.isSeated()) {
        freedTable = clientObj.getTable();
        tables_[freedTable - 1].release(event.getTime());
    }

    clientObj.leave();
    logEvent(event);

    if (freedTable != -1)
        processWaitingQueue(event.getTime(), freedTable);
}

void ComputerClub::processWaitingQueue(const std::string& time, int tableNum) {
    if (waitingQueue_.empty()) return;

    const std::string nextClient = waitingQueue_.front();
    waitingQueue_.pop();

    clients_.at(nextClient).seat(tableNum);
    tables_[tableNum - 1].occupy(nextClient, time);
    logEvent(Event(time, EventType::OUT_CLIENT_SEATED, nextClient, tableNum));
}

void ComputerClub::close() {
    std::vector<std::string> remaining;
    for (const auto& [name, client] : clients_) {
        if (client.isInClub()) remaining.push_back(name);
    }
    std::ranges::sort(remaining);

    for (const auto& name : remaining) {
        if (Client& client = clients_.at(name); client.isSeated())
            tables_[client.getTable() - 1].release(closeTime_);
        logEvent(Event(closeTime_, EventType::OUT_CLIENT_LEFT, name));
    }
}

void ComputerClub::printResults() const {
    std::cout << openTime_ << "\n";
    for (const auto& event : eventLog_)
        std::cout << event.toString() << "\n";
    std::cout << closeTime_ << "\n";

    for (const Table& table : tables_)
        std::cout << table.getNumber() << " " << table.getRevenue() << " " << table.getUsageTime() << "\n";
}