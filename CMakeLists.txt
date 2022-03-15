cmake_minimum_required(VERSION 3.21)
project(cpp_InternetRelayChat)

set(CMAKE_CXX_STANDARD 11)

add_executable(cpp_InternetRelayChat

        includes/Chat.hpp
        includes/Client.hpp
        includes/History.hpp
        includes/Message.hpp
        includes/Server.hpp
        includes/UserInfo.hpp
        includes/utils.hpp

        srcs/Chat.cpp
        srcs/Client.cpp
        srcs/History.cpp
        srcs/Message.cpp
        srcs/Server.cpp
        srcs/UserInfo.cpp
        srcs/utils.cpp


        main.cpp



#        ft_chat
        bot/Bot.cpp
        bot/Bot.hpp

        bot/Request.cpp
        bot/Request.hpp

        bot/main.cpp)


