add_executable(
    srepl
    main.cxx
    Srepl.cxx
)

target_link_libraries(
    srepl
    PRIVATE
    cpptoolbox
    args
    stdc++fs
)

target_compile_features(
    srepl PRIVATE cxx_std_17
)
