#include "Args.hpp"
#include <args/args.hpp>

namespace
{
auto opt_flag(char c, SreplMode &mode, SreplMode value)
{
    return args::opt {
        c,
        false,
        args::save{mode},
        [&mode, value](auto arg) {
            if (arg)
                return value;
            else
                return mode;
        }
    };
}
} // namespace

Args::Args(int argc, char **argv)
{
    auto parser = args::parser{
        opt_flag('i', mode_, SreplMode::Interactive),
        opt_flag('y', mode_, SreplMode::Replace)
    };

    parser.parse(argc, argv);
    auto posargs = parser.posargs();

    if (posargs.size() != 3) {
        return;
    }

    path_ = posargs[0];
    pattern_ = posargs[1];
    replacement_ = posargs[2];
}

bool Args::validate() const
{
    return path_ && pattern_ && replacement_;
}

SreplMode Args::mode() const
{
    return mode_;
}

std::string Args::path() const
{
    return path_;
}

std::string Args::pattern() const
{
    return pattern_;
}

std::string Args::replacement() const
{
    return replacement_;
}

