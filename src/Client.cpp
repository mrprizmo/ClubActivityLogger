#include "Client.h"

Client::Client(std::string name)
	: name_(std::move(name)), state_(State::Present), table_(0) {}

void Client::seat(int tableNum) {
	state_ = State::Seated;
	table_ = tableNum;
}

void Client::leave() {
	state_ = State::NotPresent;
	table_ = 0;
}

void Client::moveToQueue() {
	state_ = State::InQueue;
	table_ = 0;
}