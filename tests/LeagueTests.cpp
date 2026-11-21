#include "League.h"

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

} // namespace

int main() {
    standingsFollowTieBreakRules();
    rejectedMatchDoesNotChangeStandings();
}
