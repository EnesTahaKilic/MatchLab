#include "League.h"
#include "Match.h"
#include "CsvMatchReader.h"

#include <sstream>
#include <stdexcept>
#include <string>
#include <fstream>

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

void csvReaderParsesQuotedFields() {
    std::istringstream csv(
        "home_team,away_team,home_goals,away_goals,kickoff_time,result_available_time\n"
        "\"River Plate, Reserves\",\"The \"\"Reds\"\"\",1,0,1790000000,1790007200\n");

    const auto matches = CsvMatchReader::read(csv);
    require(matches.size() == 1, "CSV reader should parse a quoted row");
    require(matches[0].getHomeTeam() == "River Plate, Reserves",
            "CSV reader should keep commas inside quoted team names");
    require(matches[0].getAwayTeam() == "The \"Reds\"",
            "CSV reader should decode escaped quotes");
}

void csvReaderRejectsMalformedQuotes() {
    const std::string header =
        "home_team,away_team,home_goals,away_goals,kickoff_time,result_available_time\n";
    const std::string badRows[] = {
        "\"Unclosed,City,1,0,1790000000,1790007200\n",
        "Rovers\" FC,City,1,0,1790000000,1790007200\n",
        "\"Rovers\" FC,City,1,0,1790000000,1790007200\n"
    };

    for (const std::string& row : badRows) {
        std::istringstream csv(header + row);
        bool rejected = false;
        try {
            CsvMatchReader::read(csv);
        } catch (const std::invalid_argument&) {
            rejected = true;
        }
        require(rejected, "CSV reader should reject malformed quoted fields");
    }
}

void csvReaderLoadsRepositorySampleFile() {
    const std::string path = std::string(MATCHLAB_SOURCE_DIR) + "/data/matches.csv";
    std::ifstream csv(path);
    require(static_cast<bool>(csv), "Sample CSV file should be available");

    const auto matches = CsvMatchReader::read(csv);
    require(matches.size() == 2, "Sample CSV should contain two matches");
    require(matches[0].getHomeTeam() == "River Plate", "Sample CSV should retain team names");
    require(matches[1].getHomeGoals() == 0 && matches[1].getAwayGoals() == 0,
            "Sample CSV should retain a drawn result");
}

void leagueImportsMatchesAndPreservesTimestamps() {
    League league;
    league.addTeam("River Plate");
    league.addTeam("San Lorenzo");

    std::istringstream csv(
        "home_team,away_team,home_goals,away_goals,kickoff_time,result_available_time\n"
        "River Plate,San Lorenzo,2,1,1790000000,1790007200\n");
    const auto matches = CsvMatchReader::read(csv);
    league.addMatches(matches);

    const auto storedMatches = league.getMatches();
    require(storedMatches.size() == 1, "League should store the imported match");
    require(storedMatches[0].getKickoffTime() == 1790000000,
            "League should preserve the imported kickoff timestamp");
    require(storedMatches[0].getResultAvailableTime() == 1790007200,
            "League should preserve result availability");

    const auto standings = league.getStandings();
    require(standings[0].getPoints() == 3, "League import should update standings");
}

void leagueMatchImportIsAtomicOnUnknownTeam() {
    League league;
    league.addTeam("Rovers");
    league.addTeam("City");
    league.addMatch("Rovers", "City", 1, 0);
    const auto beforeStandings = league.getStandings();
    const auto beforeMatches = league.getMatches();

    const std::vector<Match> imported = {
        Match("Rovers", "City", 0, 1, 1790000000, 1790003600),
        Match("Rovers", "Unknown", 2, 0, 1790007200, 1790010800)
    };

    bool rejected = false;
    try {
        league.addMatches(imported);
    } catch (const std::invalid_argument&) {
        rejected = true;
    }
    require(rejected, "Import should reject matches with an unknown team");

    const auto afterStandings = league.getStandings();
    const auto afterMatches = league.getMatches();
    require(afterMatches.size() == beforeMatches.size(), "Failed import should add no match rows");
    require(afterStandings[0].getPoints() == beforeStandings[0].getPoints(),
            "Failed import should leave standings unchanged");
    require(afterStandings[1].getPoints() == beforeStandings[1].getPoints(),
            "Failed import should leave every team's points unchanged");
}

void standingsUseOnlyResultsAvailableByTimestamp() {
    League league;
    league.addTeam("Home");
    league.addTeam("Away");
    league.addTeam("Third");

    const std::vector<Match> matches = {
        Match("Home", "Away", 2, 0, 10, 100),
        Match("Away", "Third", 1, 1, 110, 200),
        Match("Third", "Home", 3, 0, 210, 300),
        Match("Home", "Third", 5, 0)
    };
    league.addMatches(matches);

    const auto beforeFirstResult = league.getStandings(99);
    require(beforeFirstResult[0].getPlayed() == 0,
            "No result should be counted before the first availability time");

    const auto atFirstResult = league.getStandings(100);
    require(atFirstResult[0].getName() == "Home" && atFirstResult[0].getPoints() == 3,
            "A result should be counted at its exact availability time");
    require(atFirstResult[1].getPlayed() == 0,
            "Later results and results without an availability timestamp should be excluded");

    const auto beforeSecondResult = league.getStandings(199);
    require(beforeSecondResult[1].getPlayed() == 0,
            "A result should not be counted before it becomes available");

    const auto atSecondResult = league.getStandings(200);
    require(atSecondResult[1].getPlayed() == 1,
            "The result should be counted at the inclusive cutoff");

    const auto currentStandings = league.getStandings();
    require(currentStandings[0].getPlayed() == 3,
            "The existing standings query should continue to include all recorded matches");
}

void standingsRejectNegativeTimestamp() {
    League league;
    bool rejected = false;
    try {
        league.getStandings(-1);
    } catch (const std::invalid_argument&) {
        rejected = true;
    }
    require(rejected, "Negative as-of timestamps should be rejected");
}

void leagueTracksHomeAndAwayPerformanceSeparately() {
    League league;
    league.addTeam("Home");
    league.addTeam("Away");
    league.addMatch("Home", "Away", 2, 1, 10, 20);
    league.addMatch("Away", "Home", 0, 0, 30, 40);

    const auto standings = league.getStandings();
    const Team& home = standings[0].getName() == "Home" ? standings[0] : standings[1];
    const Team& away = standings[0].getName() == "Away" ? standings[0] : standings[1];

    require(home.getHomePlayed() == 1 && home.getAwayPlayed() == 1,
            "League should separate a team's home and away match counts");
    require(home.getHomeGoalsFor() == 2 && home.getHomeGoalsAgainst() == 1,
            "League should track goals in the home role");
    require(home.getAwayGoalsFor() == 0 && home.getAwayGoalsAgainst() == 0,
            "League should track goals in the away role");
    require(away.getHomeGoalsFor() == 0 && away.getAwayGoalsFor() == 1,
            "League should assign each score to the correct venue role");

    const auto historical = league.getStandings(40);
    const Team& historicalHome = historical[0].getName() == "Home" ? historical[0] : historical[1];
    require(historicalHome.getHomePlayed() == 1 && historicalHome.getAwayPlayed() == 1,
            "Historical standings should also preserve venue-specific counts");
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
    csvReaderParsesQuotedFields();
    csvReaderRejectsMalformedQuotes();
    csvReaderRejectsMalformedInput();
    csvReaderLoadsRepositorySampleFile();
    leagueImportsMatchesAndPreservesTimestamps();
    leagueMatchImportIsAtomicOnUnknownTeam();
    standingsUseOnlyResultsAvailableByTimestamp();
    standingsRejectNegativeTimestamp();
    leagueTracksHomeAndAwayPerformanceSeparately();
}
