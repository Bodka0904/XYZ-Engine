#include "stdafx.h"
#include "Log.h"

#include <time.h>


namespace XYZ {
	int				Logger::s_LogLevel = TRACE;
	std::string		Logger::s_FileName = "Log.txt";
	std::ofstream	Logger::s_LogFile;
	Logger			Logger::s_Instance;


	void Logger::Init()
	{
		s_LogFile.open(s_FileName.c_str(), std::ios::out | std::ios::app);
	}

	std::string Logger::currentDateTime()
	{
		time_t     now = time(NULL);
		struct tm  tstruct;
		char       buf[80];
		localtime_s(&tstruct, &now);
		strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
		return buf;
	}

}


