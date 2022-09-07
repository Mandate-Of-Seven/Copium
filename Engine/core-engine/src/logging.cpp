#include "logging.h"

std::shared_ptr<spdlog::logger> Log::consoleLogger;
//std::shared_ptr<spdlog::logger> ostream_logger;

void Log::init()
{
	spdlog::set_pattern("%^[%T] %n: %v%$");
	consoleLogger = spdlog::stdout_color_mt("GameEngine");
	consoleLogger->set_level(spdlog::level::trace);
}

std::string Log::toString(std::string msg)
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
    std::string test = _oss.str();
    std::cout << test << std::endl;

    return test;
}

