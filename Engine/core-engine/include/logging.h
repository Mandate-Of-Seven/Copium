#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/ostream_sink.h"
#include <memory>
#include <string>
#include <iostream>

class Log
{
	public:
		static void init();
		static std::string toString(std::string msg);
		inline static std::shared_ptr<spdlog::logger>& getConsoleLogger() {return consoleLogger; }
	private:
		static std::shared_ptr<spdlog::logger> consoleLogger;
		
};

//User Macros
#define Console_Critical(...)		::Log::getConsoleLogger()->critical(__VA_ARGS__)
#define Console_Error(...)		::Log::getConsoleLogger()->error(__VA_ARGS__)
#define Console_Warn(...)		::Log::getConsoleLogger()->warn(__VA_ARGS__)
#define Console_Info(...)		::Log::getConsoleLogger()->info(__VA_ARGS__)
#define Console_Trace(...)		::Log::getConsoleLogger()->trace(__VA_ARGS__)

#define Console_ToString(...)	::Log::toString(__VA_ARGS__)