add_executable(Forest WIN32 MACOSX_BUNDLE
    main.cpp
)

target_link_libraries(Forest ${OPENGLEXAMPLES_REQUIRED_LIBS} png)
