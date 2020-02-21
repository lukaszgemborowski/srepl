#include "Srepl.hpp"
#include "ChangeOnce.hpp"
#include <toolbox/text_file.hpp>
#include <toolbox/string/line_iterator.hpp>
#include <iostream>
#include <algorithm>

namespace
{
const auto TEXT_GREEN = "\x1b[32m";
const auto TEXT_RED = "\x1b[31m";
const auto TEXT_RESET = "\x1b[0m";
const auto TEXT_STRIKE = "\x1b[9m";
}

Srepl::Srepl(toolbox::fs::path path, std::regex re, std::string with)
    : path_ {path}
    , re_ {re}
    , with_ {with}
{}

void Srepl::operator()(toolbox::fs::path file)
{
    auto contents = toolbox::text_file::load(file);
    auto changed = ChangeOnce{false};

    for (auto line = toolbox::string::line_iterator{contents}; line != toolbox::string::line_iterator{}; ++ line) {
        changed = askUserForMatches(file, contents, line);
    }

    if (changed)
        toolbox::text_file::save(file, contents);
}

namespace // IO functions, could be extracted and may be a parameter for Srepl class
{

void printMatch(toolbox::fs::path file, std::size_t lineNumber, std::string_view line, std::string_view replacement, std::cmatch match)
{
    std::cout << file << ":" << lineNumber << ": "
                << line.substr(0, match.position())
                << TEXT_RED << TEXT_STRIKE << line.substr(match.position(), match.length())
                << TEXT_RESET << TEXT_GREEN << replacement
                << TEXT_RESET << line.substr(match.position() + match.length())
                << "\n";
}

bool askYesNo(std::string_view question, std::string_view yes, std::string_view no)
{
    while (true) {
        std::cout << question << "[" << yes[0] << "/" << no[0] << "]: ";
        std::string r;
        std::cin >> r;

        if (yes.find(r[0]) != std::string_view::npos)
            return true;
        else if (no.find(r[0]) != std::string_view::npos)
            return false;
    }
}

}

bool Srepl::askUserForMatches(toolbox::fs::path file, std::string &contents, std::size_t lineNumber, std::string_view line)
{
    bool changed = false;

    for (auto m = std::cregex_iterator(line.begin(), line.end(), re_); m != std::cregex_iterator(); ++m) {
        auto matchStr = std::string{line.substr(m->position(), m->length())};
        auto result = std::regex_replace(matchStr, re_, with_);

        printMatch(file, lineNumber, line, result, *m);
        if (askYesNo("Replace?", "yY","nN") == false)
            continue;

        auto absolutePosition = line.data() - contents.data() + m->position();
        contents.replace(absolutePosition, m->length(), result);
        changed = true;
    }

    return changed;
}
