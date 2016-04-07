// Patron v0.7

#ifndef PAT_H
#define PAT_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>

#include "Date.h"


class Patron
{
private:
	const std::string DATE_PATTERN = R"(^([1][0-2]|[0]?[1-9])/([1-2][0-9]|3[0-2]|[0]?[1-9])/((?:19|20)[0-9]{2})$)";
	const std::string MONEY_PATTERN = R"(^[0-9]{1,8}(\.[0-9]{1,8})?$)";
	const std::string ADDRESS_PATTERN = R"([a-zA-Z0-9_ ]{3,50}$)";
	const std::string NAME_PATTERN = R"(^[A-Z][a-zA-Z ]{2,20}$)";
	const std::string ID_PATTERN = R"(^[0-9]+$)";
	const size_t NUM_VAR = 7;

	std::string _firstName;
	std::string _LastName;
	std::string _address;
	std::string _birthDate;
	std::string _joinDate;
	double _outstandingFees;
	size_t _ID;
	std::string _currentDate;

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
	void SetCurrentDate( std::string );
	const std::string& GetFirstName();
	const std::string& GetLastName();
	const std::string& GetAddress();
	const std::string& GetBirthDate();
	const std::string& GetJoinDate();
	double GetFees();
	size_t GetID();
	bool IsMinor();
	void Write( std::ostream& );
	bool Read( std::istream& );
	void Display( std::ostream& );
};



#endif PAT_H