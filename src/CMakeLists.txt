add_executable(Forest WIN32 MACOSX_BUNDLE
    main.cpp
    objects.cpp
    objloader.cpp
    shader.cpp
    texture.cpp
    Camera.cpp
    Object.cpp
    PerlinNoise.cpp
)

target_link_libraries(Forest ${OPENGLEXAMPLES_REQUIRED_LIBS} png)
