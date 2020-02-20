#ifndef SREPL_ITERATEFILESYSTEM_HPP
#define SREPL_ITERATEFILESYSTEM_HPP

#include <toolbox/cpp/filesystem.hpp>

template<class FsIterator, class FileMatcher, class Callback>
void IterateFilesystem(FsIterator fsi, FileMatcher matcher, Callback callback)
{
    for (auto &it : fsi) {
        if (!matcher(it)) {
            continue;
        }

        callback(it);
    }
}

template<class Callback>
void IterateFilesystem(toolbox::fs::path root, Callback callback)
{
    IterateFilesystem(
        toolbox::fs::recursive_directory_iterator{root},
        [](auto &f) { return toolbox::fs::is_regular_file(f.status()); },
        callback);
}

#endif // LIBREPLACER_ITERATEFILESYSTEM_HPP
