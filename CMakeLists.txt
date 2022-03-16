cmake_minimum_required(VERSION 3.21)
project(cpp_InternetRelayChat)

set(CMAKE_CXX_STANDARD 11)

add_executable(cpp_InternetRelayChat

        includes/Chat.hpp
        includes/Client.hpp
        includes/Hist.hpp
        includes/Message.hpp
        includes/Server.hpp
        includes/Information.hpp

        srcs/Chat.cpp
        srcs/Client.cpp
        srcs/Hist.cpp
        srcs/Message.cpp
        srcs/Server.cpp
        srcs/Information.cpp


        main.cpp

        bot/Bot.cpp
        bot/Bot.hpp

        bot/Request.cpp
        bot/Request.hpp

        bot/main.cpp)


