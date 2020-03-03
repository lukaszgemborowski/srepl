#include "IterateFilesystem.hpp"
#include "Srepl.hpp"
#include <args/args.hpp>
#include <iostream>

auto decideSreplMode(bool yesForAll, bool alwaysAsk)
{
    auto mode = SreplMode::Show;

    if (yesForAll)
        mode = SreplMode::Replace;
    if (alwaysAsk)
        mode = SreplMode::Interactive;

    return mode;
}

int main(int argc, char **argv)
{
    auto opt_yesForAll = args::opt<bool>{'y', false};
    auto opt_alwaysAsk = args::opt<bool>{'i', false};
    auto parser = args::parser{opt_yesForAll, opt_alwaysAsk};
    parser.parse(argc, argv);

    if (*opt_yesForAll && *opt_alwaysAsk) {
        std::cerr << argv[0] << " can't use -y together with -i\n";
        return 0;
    }

    auto args = parser.posargs();

    if (args.size() != 3) {
        std::cerr << argv[0] << " [path] [regex] [replace_with]\n";
        return 0;
    }

    Srepl srepl {
        decideSreplMode(opt_yesForAll, opt_alwaysAsk),
        args[0],
        std::regex{args[1]},
        args[2]
    };

    IterateFilesystem(args[0], srepl);
}
