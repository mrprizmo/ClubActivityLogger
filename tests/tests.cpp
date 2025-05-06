#include <gtest/gtest.h>
#include "Event.h"
#include "InputParser.h"
#include "ComputerClub.h"
#include <iostream>

TEST(TimeUtilsTest, TimeToMinutes) {
    EXPECT_EQ(TimeUtils::timeToMinutes("00:00"), 0);
    EXPECT_EQ(TimeUtils::timeToMinutes("01:30"), 90);
    EXPECT_EQ(TimeUtils::timeToMinutes("23:59"), 1439);
}

TEST(TimeUtilsTest, IsValidTime) {
    EXPECT_TRUE(TimeUtils::isValidTime("09:00"));
    EXPECT_TRUE(TimeUtils::isValidTime("23:59"));
    EXPECT_FALSE(TimeUtils::isValidTime("24:00"));
    EXPECT_FALSE(TimeUtils::isValidTime("12:60"));
    EXPECT_FALSE(TimeUtils::isValidTime("9:00"));
}

TEST(TimeUtilsTest, IsTimeOrdered) {
    EXPECT_TRUE(TimeUtils::isTimeOrdered("09:00", "09:00"));
    EXPECT_TRUE(TimeUtils::isTimeOrdered("09:00", "10:00"));
    EXPECT_FALSE(TimeUtils::isTimeOrdered("10:00", "09:00"));
}

TEST(TableTest, OccupyAndRelease) {
    Table table(1, 10);
    EXPECT_FALSE(table.isOccupied());
    table.occupy("client1", "10:00");
    EXPECT_TRUE(table.isOccupied());
    table.release("11:15");
    EXPECT_FALSE(table.isOccupied());
    EXPECT_EQ(table.getRevenue(), 20);
    EXPECT_EQ(table.getUsageTime(), "01:15");
}

TEST(TableTest, MultipleSessions) {
    Table table(2, 15);
    table.occupy("client2", "09:00");
    table.release("09:30");
    table.occupy("client3", "10:00");
    table.release("11:01");
    EXPECT_EQ(table.getRevenue(), 15 + 30);
    EXPECT_EQ(table.getUsageTime(), "01:31");
}

TEST(ComputerClubTest, SimpleScenario) {
    ComputerClub club(2, "09:00", "19:00", 10);
    std::vector<Event> events = {
        Event("09:01", EventType::CLIENT_ARRIVED, "alice"),
        Event("09:02", EventType::CLIENT_SEATED, "alice", 1),
        Event("09:10", EventType::CLIENT_ARRIVED, "bob"),
        Event("09:11", EventType::CLIENT_SEATED, "bob", 2),
        Event("10:00", EventType::CLIENT_LEFT, "alice"),
        Event("10:30", EventType::CLIENT_LEFT, "bob")
    };
    club.processEvents(events);
    club.close();
}

TEST(InputParserTest, ParseValidInput) {
    std::istringstream input(
        "2\n"
        "09:00 19:00\n"
        "10\n"
        "09:01 1 alice\n"
        "09:02 2 alice 1\n"
        "09:10 1 bob\n"
        "09:11 2 bob 2\n"
        "10:00 4 alice\n"
        "10:30 4 bob\n"
    );
    int tableCount, rate;
    std::string open, close;
    std::vector<Event> events;
    EXPECT_TRUE(InputParser::parseInput(input, tableCount, open, close, rate, events));
    EXPECT_EQ(tableCount, 2);
    EXPECT_EQ(open, "09:00");
    EXPECT_EQ(close, "19:00");
    EXPECT_EQ(rate, 10);
    EXPECT_EQ(events.size(), 6);
}

TEST(InputParserTest, ParseInvalidInput) {
    std::istringstream input(
        "0\n"
        "09:00 19:00\n"
        "10\n"
    );
    int tableCount, rate;
    std::string open, close;
    std::vector<Event> events;
    EXPECT_FALSE(InputParser::parseInput(input, tableCount, open, close, rate, events));
}

TEST(IntegrationTest, ExampleFromTask) {
    std::stringstream input;
    input << "3\n"
          << "09:00 19:00\n"
          << "10\n"
          << "08:48 1 client1\n"
          << "09:41 1 client1\n"
          << "09:48 1 client2\n"
          << "09:52 3 client1\n"
          << "09:54 2 client1 1\n"
          << "10:25 2 client2 2\n"
          << "10:58 1 client3\n"
          << "10:59 2 client3 3\n"
          << "11:30 1 client4\n"
          << "11:35 2 client4 2\n"
          << "11:45 3 client4\n"
          << "12:33 4 client1\n"
          << "12:43 4 client2\n"
          << "15:52 4 client4\n";

    int tableCount;
    std::string openTime, closeTime;
    int hourlyRate;
    std::vector<Event> events;

    ASSERT_TRUE(InputParser::parseInput(input, tableCount, openTime, closeTime,
                                        hourlyRate, events));

    ComputerClub club(tableCount, openTime, closeTime, hourlyRate);
    club.processEvents(events);
    club.close();

    std::vector<Event> eventLog = club.getEventLog();

    std::vector<std::string> expectedEvents = {
        "08:48 1 client1",
        "08:48 13 NotOpenYet",
        "09:41 1 client1",
        "09:48 1 client2",
        "09:52 3 client1",
        "09:52 13 ICanWaitNoLonger!",
        "09:54 2 client1 1",
        "10:25 2 client2 2",
        "10:58 1 client3",
        "10:59 2 client3 3",
        "11:30 1 client4",
        "11:35 2 client4 2",
        "11:35 13 PlaceIsBusy",
        "11:45 3 client4",
        "12:33 4 client1",
        "12:33 12 client4 1",
        "12:43 4 client2",
        "15:52 4 client4",
        "19:00 11 client3"
    };

    ASSERT_EQ(eventLog.size(), expectedEvents.size());

    for (size_t i = 0; i < expectedEvents.size(); ++i) {
        EXPECT_EQ(eventLog[i].toString(), expectedEvents[i]) << "Mismatch at event #" << i;
    }

    const auto& tables = club.getTables();

    ASSERT_EQ(tables.size(), 3);
    EXPECT_EQ(tables[0].getRevenue(), 70);
    EXPECT_EQ(tables[0].getUsageTime(), "05:58");
    EXPECT_EQ(tables[1].getRevenue(), 30);
    EXPECT_EQ(tables[1].getUsageTime(), "02:18");
    EXPECT_EQ(tables[2].getRevenue(), 90);
    EXPECT_EQ(tables[2].getUsageTime(), "08:01");
}