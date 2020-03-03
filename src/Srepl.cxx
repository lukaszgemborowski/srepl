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

Srepl::Srepl(SreplMode mode, toolbox::fs::path path, std::regex re, std::string with)
    : path_ {path}
    , re_ {re}
    , with_ {with}
    , mode_ {mode}
{}

void Srepl::operator()(toolbox::fs::path file)
{
    // load the whole file
    auto tf = toolbox::text_file{file};

    // if 'changed' will be set to true, lambda will be executed when exiting the scope
    // that means it will be called only if askUserForMatches returns true at least once
    auto changed = ChangeOnce{false}
                   .on(true, [&]() { tf.save(); });

    for (auto line : tf.iterator<toolbox::string::line_iterator>()) {
        changed = askUserForMatches(file, line);
    }
}

namespace // IO functions, could be extracted and may be a parameter for Srepl class
{

void printMatch(const toolbox::fs::path &file, std::size_t lineNumber, std::string_view line, std::string_view replacement, const std::cmatch &match)
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
        std::cout << question << " [" << yes[0] << "/" << no[0] << "]: ";
        std::string r;
        std::cin >> r;

        if (yes.find(r[0]) != std::string_view::npos)
            return true;
        else if (no.find(r[0]) != std::string_view::npos)
            return false;
    }
}

std::string regex_replace(std::regex &re, std::string_view input, const std::string& with)
{
    std::string sub;
    std::regex_replace(std::back_inserter(sub), input.begin(), input.end(), re, with);
    return sub;
}

}

bool Srepl::askUserForMatches(const toolbox::fs::path& file, toolbox::string::line_reference line)
{
    bool changed = false;

    if (line.length() > 1024)
        return false;

    for (auto m = std::cregex_iterator((*line).begin(), (*line).end(), re_); m != std::cregex_iterator{}; ++m) {
        // get substr of matched part of the line
        auto matchStr = line.substr(m->position(), m->length());

        // replace that match with contents of with_
        auto subst = regex_replace(re_, matchStr, with_);

        // print a line and ask user how to proceed
        printMatch(file, line.lineNumber(), *line, subst, *m);

        // show mode, do not ask and replace, just print the match
        if (mode_ == SreplMode::Show) {
            continue;
        }

        if (mode_ == SreplMode::Interactive) {
            if (askYesNo("Replace?", "yY","nN") == false)
                continue;
        }

        // replace the match in-string
        line.replace(m->position(), m->length(), subst);

        // recalculate line iterator, TODO: do this inside replace method
        line.recalculate();

        changed = true;
    }

    return changed;
}
