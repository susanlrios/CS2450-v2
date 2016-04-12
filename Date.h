// Date v1.1

#ifndef DATE_H
#define DATE_H

#include <vector>
#include <string>
#include <time.h>
#include <regex>


class Date
{
private:
	static const std::string DATE_PATTERN;
	static std::string _currentDate;
	static const std::string CurrentDateTime();
	static std::vector<std::string> Split( char*, char );

public:
	static void SetCurrentDate( std::string );
	static const std::string& GetCurrentDate();
	static void AddDayToCurrent();
};



#endif DATE_H