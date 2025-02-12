cmake_minimum_required(VERSION 3.30)
project(TestSFMLGame)

set(CMAKE_CXX_STANDARD 17)

add_executable(TestSFMLGame src/main.cpp src/Components/AnimationComponent.cpp src/GUI/Button.cpp src/States/EditorState.cpp src/Entities/Entity.cpp src/Game.cpp
        src/States/GameState.cpp src/Components/HitboxComponent.cpp src/States/MainMenuState.cpp src/Components/MovementComponent.cpp src/GUI/PauseMenu.cpp src/Entities/Player.cpp src/States/State.cpp src/Tile/Tile.cpp
        headers/Components/AnimationComponent.h headers/GUI/Button.h headers/States/EditorState.h headers/Entities/Entity.h headers/Game.h
        headers/States/GameState.h headers/Components/HitboxComponent.h headers/States/MainMenuState.h headers/Components/MovementComponent.h headers/GUI/PauseMenu.h headers/Entities/Player.h headers/States/State.h headers/Tile/Tile.h
)

include_directories(headers /usr/include c:/SFML/include)

set(SFML_ROOT c:/SFML)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(TestSFMLGame ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
file(COPY assets/fonts DESTINATION ${CMAKE_BINARY_DIR})
file(COPY assets/images/backgrounds DESTINATION ${CMAKE_BINARY_DIR})


# Copy dlls to build
if(WIN32)
    file(GLOB BINARY_DEP_DLLS "${SFML_INCLUDE_DIR}/../bin/*.dll")
    file(COPY ${BINARY_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
    file(GLOB MINGW_DEP_DLLS "C:/mingw64/bin/*.dll")
    file(COPY ${MINGW_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
endif()