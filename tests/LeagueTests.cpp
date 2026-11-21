#include "League.h"
#include "Match.h"
#include "CsvMatchReader.h"

#include <sstream>
#include <stdexcept>
#include <string>

namespace {

void require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

void standingsFollowTieBreakRules() {
    League league;
    league.addTeam("Zulu");
    league.addTeam("Bravo");
    league.addTeam("Alpha");
    league.addTeam("Charlie");

    league.addMatch("Zulu", "Alpha", 2, 0);
    league.addMatch("Bravo", "Charlie", 1, 0);
    league.addMatch("Zulu", "Bravo", 1, 0);
    league.addMatch("Alpha", "Charlie", 2, 1);
    league.addMatch("Zulu", "Charlie", 1, 0);
    league.addMatch("Bravo", "Alpha", 2, 1);

    const auto standings = league.getStandings();
    require(standings.size() == 4, "Standings should include every team");
    require(standings[0].getName() == "Zulu", "Points should rank first");
    require(standings[1].getName() == "Bravo", "Goal difference should break a points tie");
    require(standings[2].getName() == "Alpha", "Goals scored should break a goal difference tie");
    require(standings[3].getName() == "Charlie", "Remaining tie should use team name");
}

void rejectedMatchDoesNotChangeStandings() {
    League league;
    league.addTeam("Home");
    league.addTeam("Away");
    league.addMatch("Home", "Away", 1, 0);

    const auto before = league.getStandings();
    try {
        league.addMatch("Home", "Unknown", 2, 0);
        throw std::runtime_error("Unknown team should be rejected");
    } catch (const std::invalid_argument&) {
    }

    const auto after = league.getStandings();
    require(after.size() == before.size(), "Rejected match must preserve teams");
    for (std::size_t i = 0; i < before.size(); ++i) {
        require(after[i].getName() == before[i].getName(), "Rejected match changed standings order");
        require(after[i].getPlayed() == before[i].getPlayed(), "Rejected match changed played count");
        require(after[i].getPoints() == before[i].getPoints(), "Rejected match changed points");
    }
}

void matchStoresKickoffTime() {
    Match legacyMatch("Home", "Away", 1, 0);
    Match datedMatch("Home", "Away", 1, 0, 1790000000);
    Match observedMatch("Home", "Away", 1, 0, 1790000000, 1790007200);

    require(legacyMatch.getKickoffTime() == 0, "Existing constructor calls should retain the default kickoff");
    require(datedMatch.getKickoffTime() == 1790000000, "Match should retain its kickoff timestamp");
    require(observedMatch.getResultAvailableTime() == 1790007200,
            "Match should retain when its result became available");
}

void teamStatisticsCoverWinsDrawsAndLosses() {
    Team team("Rovers");
    team.recordMatch(3, 1);
    team.recordMatch(2, 2);
    team.recordMatch(0, 1);

    require(team.getPlayed() == 3, "Team should count all recorded matches");
    require(team.getWins() == 1, "Team should count its win");
    require(team.getDraws() == 1, "Team should count its draw");
    require(team.getLosses() == 1, "Team should count its loss");
    require(team.getGoalsFor() == 5, "Team should total goals scored");
    require(team.getGoalsAgainst() == 4, "Team should total goals conceded");
    require(team.getGoalDifference() == 1, "Team should calculate goal difference");
    require(team.getPoints() == 4, "Team should award three points for a win and one for a draw");
}

void invalidTeamAndResultInputsAreRejected() {
    bool emptyNameRejected = false;
    try {
        Team team("");
    } catch (const std::invalid_argument&) {
        emptyNameRejected = true;
    }
    require(emptyNameRejected, "Empty team names should be rejected");

    Team team("Rovers");
    bool negativeScoreRejected = false;
    try {
        team.recordMatch(-1, 0);
    } catch (const std::invalid_argument&) {
        negativeScoreRejected = true;
    }
    require(negativeScoreRejected, "Negative goals should be rejected");
    require(team.getPlayed() == 0, "Rejected score should not change team statistics");
}

void leagueRejectsInvalidTeamRegistration() {
    League league;
    league.addTeam("Rovers");

    bool duplicateRejected = false;
    try {
        league.addTeam("Rovers");
    } catch (const std::invalid_argument&) {
        duplicateRejected = true;
    }
    require(duplicateRejected, "Duplicate team names should be rejected");

    bool emptyNameRejected = false;
    try {
        league.addTeam("");
    } catch (const std::invalid_argument&) {
        emptyNameRejected = true;
    }
    require(emptyNameRejected, "Empty team names should be rejected by the league");
}

void csvReaderLoadsMatchAndTimestamps() {
    std::istringstream csv(
        "home_team,away_team,home_goals,away_goals,kickoff_time,result_available_time\n"
        "Rovers,City,2,1,1790000000,1790007200\n");

    const auto matches = CsvMatchReader::read(csv);
    require(matches.size() == 1, "CSV reader should load one match");
    require(matches[0].getHomeTeam() == "Rovers", "CSV reader should preserve the home team");
    require(matches[0].getAwayTeam() == "City", "CSV reader should preserve the away team");
    require(matches[0].getHomeGoals() == 2 && matches[0].getAwayGoals() == 1,
            "CSV reader should parse the score");
    require(matches[0].getKickoffTime() == 1790000000, "CSV reader should parse kickoff time");
    require(matches[0].getResultAvailableTime() == 1790007200,
            "CSV reader should parse result availability time");
}

void csvReaderHandlesWindowsLineEndingsAndWhitespace() {
    std::istringstream csv(
        "home_team,away_team,home_goals,away_goals,kickoff_time,result_available_time\r\n"
        " Rovers , City , 2 , 1 , 1790000000 , 1790007200 \r\n");

    const auto matches = CsvMatchReader::read(csv);
    require(matches.size() == 1, "CSV reader should handle Windows line endings");
    require(matches[0].getHomeTeam() == "Rovers" && matches[0].getAwayTeam() == "City",
            "CSV reader should trim surrounding whitespace");
}

void csvReaderRejectsMalformedInput() {
    std::istringstream badHeader("home,away,score\n");
    bool headerRejected = false;
    try {
        CsvMatchReader::read(badHeader);
    } catch (const std::invalid_argument&) {
        headerRejected = true;
    }
    require(headerRejected, "CSV reader should reject an unexpected header");

    std::istringstream badRow(
        "home_team,away_team,home_goals,away_goals,kickoff_time,result_available_time\n"
        "Rovers,City,two,1,1790000000,1790007200\n");
    bool rowRejected = false;
    try {
        CsvMatchReader::read(badRow);
    } catch (const std::invalid_argument&) {
        rowRejected = true;
    }
    require(rowRejected, "CSV reader should reject nonnumeric goal values");
}

} // namespace

int main() {
    standingsFollowTieBreakRules();
    rejectedMatchDoesNotChangeStandings();
    matchStoresKickoffTime();
    teamStatisticsCoverWinsDrawsAndLosses();
    invalidTeamAndResultInputsAreRejected();
    leagueRejectsInvalidTeamRegistration();
    csvReaderLoadsMatchAndTimestamps();
    csvReaderHandlesWindowsLineEndingsAndWhitespace();
    csvReaderRejectsMalformedInput();
}
