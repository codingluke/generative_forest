add_executable(Forest WIN32 MACOSX_BUNDLE
    mesh.cpp
    main.cpp
)

target_link_libraries(Forest ${OPENGLEXAMPLES_REQUIRED_LIBS} png)
