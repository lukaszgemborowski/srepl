#include "IterateFilesystem.hpp"
#include "Srepl.hpp"
#include <args/args.hpp>
#include <iostream>

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

int main(int argc, char **argv)
{
    auto mode = SreplMode::Show;

    auto parser = args::parser{
        opt_flag('i', mode, SreplMode::Interactive),
        opt_flag('y', mode, SreplMode::Replace)
    };

    parser.parse(argc, argv);
    auto args = parser.posargs();

    if (args.size() != 3) {
        std::cerr << argv[0] << " [path] [regex] [replace_with]\n";
        return 0;
    }

    Srepl srepl {
        mode,
        args[0],
        std::regex{args[1]},
        args[2]
    };

    IterateFilesystem(args[0], srepl);
}
