cmake_minimum_required(VERSION 3.21)
project(cpp_InternetRelayChat)

set(CMAKE_CXX_STANDARD 98)

add_executable(ft_irca

        src/Socket.cpp
        src/Socket.hpp

        src/Server.cpp
        src/Server.hpp

        src/Challenge.cpp
        src/Challenge.hpp

        src/main.cpp

        )
