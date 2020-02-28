#ifndef SREPL_SREPL_HPP
#define SREPL_SREPL_HPP

#include <regex>
#include <string>
#include <toolbox/cpp/filesystem.hpp>
#include <toolbox/string/line_iterator.hpp>

class Srepl
{
public:
    Srepl(toolbox::fs::path path, std::regex re, std::string with);
    void operator()(toolbox::fs::path file);

private:
    bool askUserForMatches(const toolbox::fs::path& file, toolbox::string::line_reference line);

private:
    toolbox::fs::path   path_;
    std::regex          re_;
    std::string         with_;
};

#endif // SREPL_SREPL_HPP
