#pragma once
#include <string>

class Client {
public:
    enum class State {
        NotPresent,
        Present,
        InQueue,
        Seated
    };

    explicit Client(std::string name);
    void seat(int tableNum);
    void leave();
    void moveToQueue();

    bool isInClub() const { return state_ != State::NotPresent; }
    bool isSeated() const { return state_ == State::Seated; }
    bool isInQueue() const { return state_ == State::InQueue; }
    int getTable() const { return table_; }
    const std::string& getName() const { return name_; }

private:
    std::string name_;
    State state_;
    int table_;
};