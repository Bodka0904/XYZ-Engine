#pragma once
#include "ColorCodes.h"
#include "Core.h"

#include <fstream>
#include <iostream>

namespace XYZ {

	enum LogLevel
	{
		NOLOG =		1 << 0,
		INFO =		1 << 1,
		WARNING =	1 << 2,
		ERR =		1 << 3,
		API =		1 << 4,
		TRACE =		API | ERR | WARNING | INFO
	};

	class Logger
	{
	private:
		struct Colorize
		{
			Colorize(AnsiCode code)
			{
				printf("\033[1;%dm", code);
			}
			~Colorize()
			{
				printf("\033[1;%dm", AnsiCode::COLOR_WHITE_F);
			}
		};
	public:
		Logger(const Logger&) = delete;
		static void Init();

		template <typename... Args>
		void Info(Args... args)
		{
			if ((s_LogLevel & LogLevel::INFO) == LogLevel::INFO)
			{
				Colorize color(AnsiCode::COLOR_GREEN_F);
				std::cout << currentDateTime() << " ";
				(std::cout << ... << args) << std::endl;
				(s_LogFile << ... << args) << std::endl;
			}
		}

		template <typename... Args>
		void Warn(Args... args)
		{
			if ((s_LogLevel & LogLevel::WARNING) == LogLevel::WARNING)
			{
				Colorize color(AnsiCode::COLOR_YELLOW_F);
				std::cout << currentDateTime() << " ";
				(std::cout << ... << args) << std::endl;
				(s_LogFile << ... << args) << std::endl;
			}
		}

		template <typename... Args>
		void Error(Args... args)
		{
			if ((s_LogLevel & LogLevel::ERR) == LogLevel::ERR)
			{
				Colorize color(AnsiCode::COLOR_RED_F);
				std::cout << currentDateTime() << " ";
				(std::cout << ... << args) << std::endl;
				(s_LogFile << ... << args) << std::endl;
			}
		}

		template <typename... Args>
		void API(Args... args)
		{
			if ((s_LogLevel & LogLevel::API) == LogLevel::API)
			{
				Colorize color(AnsiCode::COLOR_MAGENTA_F);
				std::cout << currentDateTime() << " ";
				(std::cout << ... << args) << std::endl;
				(s_LogFile << ... << args) << std::endl;
			}
		}

		inline void SetLogLevel(int level) { s_LogLevel = level; };

		static void SetLogFile(const std::string& logfile) { s_FileName = logfile; };
		static Logger& Get() { return s_Instance; };
	private:
		Logger() {};

		std::string currentDateTime();
	private:

		static int s_LogLevel;
		static std::ofstream s_LogFile;
		static std::string s_FileName;
		static Logger s_Instance;
	};

#define XYZ_LOG_INFO(...)  Logger::Get().Info(__FUNCTION__,": ", __VA_ARGS__)
#define XYZ_LOG_WARN(...) Logger::Get().Warn(__FUNCTION__,": ",__VA_ARGS__)
#define XYZ_LOG_ERR(...)  Logger::Get().Error(__FUNCTION__,": ",__VA_ARGS__)
#define XYZ_LOG_API(...)  Logger::Get().API(__FUNCTION__,": ",__VA_ARGS__)
}