#pragma once
#include "ColorCodes.h"
#include "Core.h"

#include <fstream>
#include <iostream>

namespace XYZ {
	enum LogLevel
	{
		INFO,
		WARNING,
		ERR,
		TRACE
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
			if (s_LogLevel == LogLevel::INFO || LogLevel::TRACE)
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
			if (s_LogLevel == LogLevel::WARNING || LogLevel::TRACE)
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
			if (s_LogLevel == LogLevel::ERR || LogLevel::TRACE)
			{
				Colorize color(AnsiCode::COLOR_RED_F);
				std::cout << currentDateTime() << " ";
				(std::cout << ... << args) << std::endl;
				(s_LogFile << ... << args) << std::endl;
			}
		}

		inline void SetLogLevel(LogLevel level) { s_LogLevel = level; };

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

}