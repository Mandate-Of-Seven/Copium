/*!***************************************************************************************
\file			logging.cpp
\project
\author			Shawn Tanary

\par			Course: GAM200
\par			Section:
\date			16/09/2022

\brief
    This file contins functions that logger messages to either console or to a file.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "pch.h"
#include "logging.h"
namespace Copium 
{
    std::shared_ptr<spdlog::logger> Log::consoleLogger;


    void Log::init()
    {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::trace);
        console_sink->set_pattern("[%T] [%^%l%$] %v");

        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/Engine.txt", true);
        file_sink->set_level(spdlog::level::warn);
        file_sink->set_pattern("[source %s] [function %!] [line %#] [%H:%M:%S] %n:  %v");

        spdlog::sinks_init_list sink_list = { file_sink, console_sink };


        //spdlog::set_pattern("%^[%T] %n: %v%$");
        spdlog::set_pattern("[source %s] [function %!] [line %#] %v");
        consoleLogger = std::make_shared<spdlog::logger>("Copium Engine", sink_list.begin(), sink_list.end());

    }

    std::string Log::to_string(std::string msg)
    {
        std::ostringstream _oss;
        auto ostream_logger = spdlog::get("gtest_logger");
        if (!ostream_logger)
        {
            auto ostream_sink = std::make_shared<spdlog::sinks::ostream_sink_st>(_oss);
            ostream_logger = std::make_shared<spdlog::logger>("gtest_logger", ostream_sink);
            ostream_logger->set_pattern(">%v<");
            ostream_logger->set_level(spdlog::level::trace);
        }
        spdlog::set_default_logger(ostream_logger);
        spdlog::trace(msg);
        std::string text = _oss.str();
        std::cout << text << std::endl;

        return text;
    }

    void Log::multi_sink()
    {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::warn);
        console_sink->set_pattern("[multi_sink_example] [%^%l%$] %v");

        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/multisink.txt", true);
        file_sink->set_level(spdlog::level::trace);

        spdlog::sinks_init_list sink_list = { file_sink, console_sink };

        spdlog::logger logger("multi_sink", sink_list.begin(), sink_list.end());
        logger.set_level(spdlog::level::trace);
        logger.warn("this should appear in both console and file");
        logger.info("this message should not appear in the console, only in the file");
        spdlog::set_default_logger(std::make_shared<spdlog::logger>("multi_sink", spdlog::sinks_init_list({ console_sink, file_sink })));
    }
}