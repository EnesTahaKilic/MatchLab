
#include "League.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

Team* League::findTeam(const std::string& name) {
    for (Team& team : teams) {
        if (team.getName() == name) {
            return &team;
        }
    }
    return nullptr;
}

void League::addTeam(const std::string& name) {
    if (name.empty() || findTeam(name) != nullptr) {
        throw std::invalid_argument("Invalid or duplicate team");
    }

    teams.emplace_back(name);
}

void League::addMatch(const std::string& home,
                      const std::string& away,
                      int homeGoals,
                      int awayGoals) {
    // Validate the match before changing league data.
    Match match(home, away, homeGoals, awayGoals);

    Team* homeTeam = findTeam(home);
    Team* awayTeam = findTeam(away);

    if (homeTeam == nullptr || awayTeam == nullptr) {
        throw std::invalid_argument("Team not found");
    }

    matches.push_back(match);

    homeTeam->recordMatch(homeGoals, awayGoals);
    awayTeam->recordMatch(awayGoals, homeGoals);
}

void League::displayMatches() const {
    if (matches.empty()) {
        std::cout << "No matches recorded.\n";
        return;
    }

    for (const Match& match : matches) {
        match.display();
    }
}

std::vector<Team> League::getStandings() const {
    std::vector<Team> sortedTeams = teams;

    std::sort(sortedTeams.begin(), sortedTeams.end(),
              [](const Team& a, const Team& b) {
        if (a.getPoints() != b.getPoints()) {
            return a.getPoints() > b.getPoints();
        }

        if (a.getGoalDifference() != b.getGoalDifference()) {
            return a.getGoalDifference() > b.getGoalDifference();
        }

        if (a.getGoalsFor() != b.getGoalsFor()) {
            return a.getGoalsFor() > b.getGoalsFor();
        }

        return a.getName() < b.getName();
    });

    return sortedTeams;
}

void League::displayStandings() const {
    const std::vector<Team> sortedTeams = getStandings();

    std::cout << "\n===== LEAGUE STANDINGS =====\n";

    int position = 1;

    for (const Team& team : sortedTeams) {
        std::cout << position++ << ". ";
        team.display();
    }
}
