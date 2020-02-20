add_executable(
    srepl
    main.cxx
)

target_link_libraries(
    srepl
    PRIVATE
    cpptoolbox
    stdc++fs
)

target_compile_features(
    srepl PRIVATE cxx_std_17
)