#include "CsvMatchReader.h"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace {

std::string trim(std::string value) {
    const auto first = std::find_if_not(value.begin(), value.end(), [](unsigned char character) {
        return std::isspace(character) != 0;
    });
    const auto last = std::find_if_not(value.rbegin(), value.rend(), [](unsigned char character) {
        return std::isspace(character) != 0;
    }).base();

    if (first >= last) {
        return {};
    }

    return std::string(first, last);
}

std::int64_t parseInteger(const std::string& value, std::size_t lineNumber) {
    std::istringstream stream(trim(value));
    std::int64_t parsed = 0;
    stream >> parsed;

    if (!stream || stream.peek() != std::char_traits<char>::eof()) {
        throw std::invalid_argument("Invalid integer on CSV line " + std::to_string(lineNumber));
    }

    return parsed;
}

std::vector<std::string> splitLine(const std::string& line) {
    std::vector<std::string> fields;
    std::istringstream stream(line);
    std::string field;

    while (std::getline(stream, field, ',')) {
        fields.push_back(field);
    }

    if (!line.empty() && line.back() == ',') {
        fields.emplace_back();
    }

    return fields;
}

} // namespace

std::vector<Match> CsvMatchReader::read(std::istream& input) {
    constexpr const char* expectedHeader =
        "home_team,away_team,home_goals,away_goals,kickoff_time,result_available_time";

    std::string line;
    std::size_t lineNumber = 1;
    if (!std::getline(input, line) || trim(line) != expectedHeader) {
        throw std::invalid_argument("CSV header does not match the expected format");
    }

    std::vector<Match> matches;
    while (std::getline(input, line)) {
        ++lineNumber;
        if (line.empty()) {
            continue;
        }

        std::vector<std::string> fields = splitLine(line);
        if (fields.size() != 6) {
            throw std::invalid_argument("Expected six CSV columns on line " + std::to_string(lineNumber));
        }

        for (std::string& field : fields) {
            field = trim(std::move(field));
        }

        const std::int64_t homeGoals = parseInteger(fields[2], lineNumber);
        const std::int64_t awayGoals = parseInteger(fields[3], lineNumber);
        const std::int64_t kickoffTime = parseInteger(fields[4], lineNumber);
        const std::int64_t resultAvailableTime = parseInteger(fields[5], lineNumber);

        if (homeGoals > std::numeric_limits<int>::max() || awayGoals > std::numeric_limits<int>::max() ||
            homeGoals < std::numeric_limits<int>::min() || awayGoals < std::numeric_limits<int>::min()) {
            throw std::invalid_argument("Goal count is outside the supported range on CSV line " +
                                        std::to_string(lineNumber));
        }

        matches.emplace_back(fields[0], fields[1], static_cast<int>(homeGoals),
                             static_cast<int>(awayGoals), kickoffTime, resultAvailableTime);
    }

    if (input.bad()) {
        throw std::runtime_error("Failed while reading CSV input");
    }

    return matches;
}
