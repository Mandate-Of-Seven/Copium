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
namespace Copium 
{
    void LoggingSystem::init()
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

        COPIUM_ASSERT(!consoleLogger,"Console Logger was not created properly");
        std::cout << "Logging init was called" << std::endl;
    }

    void LoggingSystem::update()
    {

    }

    void LoggingSystem::exit()
    {

    }

    std::string LoggingSystem::to_string(std::string msg)
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

        COPIUM_ASSERT(!ostream_logger, "Output stream Logger was not created properly");
        spdlog::trace(msg);
        std::string text = _oss.str();
        std::cout << text << std::endl;

        return text;
    }

    void LoggingSystem::create_multiplefile(int fileSize,int fileAmount)
    {
        int max_size = fileSize;
        int max_files = fileAmount;
        auto logger = spdlog::rotating_logger_mt("rotating_logger", "logs/rotating.txt",
                                 max_size, max_files);

        COPIUM_ASSERT(!logger, "Multiple file Logger was not created properly");
        logger->info("The purpose of this is to test the rotating file sink");
    }

    void LoggingSystem::error_log()
    {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::warn);
        console_sink->set_pattern("[Error] [%^%l%$] %v");

        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/Crashes.txt", true);
        file_sink->set_level(spdlog::level::trace);

        spdlog::sinks_init_list sink_list = { file_sink, console_sink };

        spdlog::logger logger("Error", sink_list.begin(), sink_list.end());
        logger.set_level(spdlog::level::trace);
        spdlog::set_default_logger(std::make_shared<spdlog::logger>("Error", spdlog::sinks_init_list({ console_sink, file_sink })));
    }

    void LoggingSystem::assert_to_file(std::string expr_str, bool expr, std::string file, int line, std::string msg)
    {
        error_log();

        if (expr)
        {
            std::string temp =  "\nCaused By:\t" + expr_str + '\n' +
                                "Info:\t\t" + msg + '\n' +
                                "Source:\t\t" + file + " (Line: " + std::to_string(line) + ")\n";
            FILE_CRITICAL(temp);
            spdlog::drop("Error");
            abort();
        }
    }
}