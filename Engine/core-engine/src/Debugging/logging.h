/*!***************************************************************************************
\file			logging.h
\project
\author			Shawn Tanary

\par			Course: GAM200
\par			Section:
\date			16/09/2022

\brief
	This file contins functions that logger messages to either console or to a file.

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#pragma once
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "pch.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/sinks/rotating_file_sink.h> // support for rotating file logging

#include <memory>
#include "Editor/ConsoleLog.h"

namespace Copium 
{
	class Log
	{
	public:
		/***************************************************************************/
		/*!
		\brief
			Initialises all the loggers with their desired levels and formats
		*/
		/**************************************************************************/
		static void init();

		/***************************************************************************/
		/*!
		\brief
			Used to log asserts to both file and to the console
		*/
		/**************************************************************************/
		static void error_log();

		/***************************************************************************/
		/*!
		\brief
			Used to stop the engine if the condition is true
		\param expr_str
			The error condition that was true which will be shown
		\param expr
			the assert condition
		\param file
			the file that caused an error
		\param line
			the line that caused an error
		\param msg
			the message to display along with the error
		*/
		/**************************************************************************/
		static void assert_to_file(std::string expr_str, bool expr, std::string file, int line, std::string msg);

		/***************************************************************************/
		/*!
		\brief
			Returns a string sent to the console
		\return
			the message as a string as well
		*/
		/**************************************************************************/
		static std::string to_string(std::string msg);

		/***************************************************************************/
		/*!
		\brief
			Creates multiple rotating log files
		\param fileSize
			the maximum file size
		\param fileAmount
			the maximum amount of files
		*/
		/**************************************************************************/
		static void create_multiplefile(int fileSize, int fileAmount);

		/***************************************************************************/
		/*!
		\brief
			Returns the consoleLogger
		\return
			the consoleLogger
		*/
		/**************************************************************************/
		inline static std::shared_ptr<spdlog::logger>& getConsoleLogger() { return consoleLogger; }

	private:
		static std::shared_ptr<spdlog::logger> consoleLogger;


	};
}
//User Macros
#define CONSOLE_CRITICAL(...)															\
							{															\
								Copium::Log::getConsoleLogger()->critical(__VA_ARGS__); \
								Window::EditorConsole::add_logEntry(__VA_ARGS__);		\
							}
#define CONSOLE_ERROR(...)																\
							{															\
									Copium::Log::getConsoleLogger()->error(__VA_ARGS__);\
									Window::EditorConsole::add_logEntry(__VA_ARGS__);	\
							}
#define CONSOLE_WARN(...)																\
							{															\
									Copium::Log::getConsoleLogger()->warn(__VA_ARGS__); \
									Window::EditorConsole::add_logEntry(__VA_ARGS__);	\
							}
#define CONSOLE_INFO(...)																\
							{															\
									Copium::Log::getConsoleLogger()->info(__VA_ARGS__); \
									Window::EditorConsole::add_logEntry(__VA_ARGS__);	\
							}
#define CONSOLE_TRACE(...)																\
							{															\
									Copium::Log::getConsoleLogger()->trace(__VA_ARGS__);\
									Window::EditorConsole::add_logEntry(__VA_ARGS__);	\
							}

#define FILE_CRITICAL(...)		::spdlog::critical(__VA_ARGS__);
#define FILE_ERROR(...)			::spdlog::error(__VA_ARGS__)
#define FILE_WARN(...)			::spdlog::warn(__VA_ARGS__)
#define FILE_INFO(...)			::spdlog::info(__VA_ARGS__)

//enter the condition to trigger the assert, followed by the message you want.
#define COPIUM_ASSERT(Expr, Msg) Copium::Log::assert_to_file(#Expr, Expr, __FILE__, __LINE__, Msg);

#define Console_ToString(...)	::Log::to_string(__VA_ARGS__)