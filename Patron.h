// Patron v0.4

#ifndef PAT_H
#define PAT_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
#include <regex>


class Patron
{
private:
	const std::string MONEY_PATTERN = R"(^[0-9]{1,8}(\.[0-9]{1,8})?$)";
	const std::string ADDRESS_PATTERN = R"([a-zA-Z0-9_ ]{3,50}$)";
	const std::string NAME_PATTERN = R"(^[A-Z][a-zA-Z ]{3,20}$)";
	const std::string ID_PATTERN = R"(^[0-9]+$)";
	static const std::string DATE_PATTERN;
	const size_t NUM_VAR = 7;

	std::string _firstName;
	std::string _LastName;
	std::string _address;
	std::string _birthDate;
	std::string _joinDate;
	double _outstandingFees;
	size_t _ID;

	static std::string _currentDate;

	const std::string CurrentDateTime();
	void SetFirstName( std::string );
	void SetLastName( std::string );
	void SetBirthdate( std::string );
	void SetJoinDate( std::string );
	void SetID( std::string );
	std::vector<std::string> Split( char*, char );

public:
	Patron();
	Patron( std::string, std::string, std::string, std::string, std::string, std::string );
	~Patron();
	void SetAddress( std::string );
	void SetFees( std::string );
	static void SetCurrentDate( std::string );
	const std::string& GetFirstName();
	const std::string& GetLastName();
	const std::string& GetAddress();
	const std::string& GetBirthDate();
	const std::string& GetJoinDate();
	double GetFees();
	size_t GetID();
	bool IsMinor();
	void Write( std::ostream& );
	void Read( std::istream& );
};



#endif PAT_H