#include "Srepl.hpp"

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
    auto line = toolbox::string::line_iterator{contents};
    auto end = toolbox::string::line_iterator{};

    while (line != end) {
        askUserForMatches(file, contents, line);
        ++ line;
    }

    toolbox::text_file::save(file, contents);
}

bool Srepl::askUserForMatches(toolbox::fs::path file, std::string &contents, std::size_t lineNumber, std::string_view line)
{
    bool changed = false;
    auto m = std::cregex_iterator(line.begin(), line.end(), re_);
    auto e = std::cregex_iterator();

    for (; m != e; ++m) {
        auto matchStr = std::string{line.substr(m->position(), m->length())};
        auto result = std::regex_replace(matchStr, re_, with_);

        std::cout << file << ":" << lineNumber << ": "
            << line.substr(0, m->position())
            << TEXT_RED << TEXT_STRIKE << matchStr
            << TEXT_GREEN << result
            << TEXT_RESET << line.substr(m->position() + m->length());

        if (askYesNo() == false)
            continue;

        auto absolutePosition = line.data() - contents.data() + m->position();
        contents.replace(absolutePosition, m->length(), result);
        changed = true;
    }

    return changed;
}

bool Srepl::askYesNo()
{
    while (true) {
        std::cout << "\nReplace? [y/n]: ";
        std::flush(std::cout);
        std::string r;
        std::cin >> r;

        if (r == "y" || r == "Y")
            return true;
        else if (r == "n" || r == "N")
            return false;
    }
}