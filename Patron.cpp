#ifndef PAT_CPP
#define PAT_CPP

#include "Patron.h"


Patron::Patron()
{
	_firstName = "John";
	_LastName = "Doe";
	_address = "";
	_birthDate = Date::GetCurrentDate();
	_joinDate = Date::GetCurrentDate();
	_outstandingFees = 0;
	_ID = 0;
	_currentDate = "";
}


Patron::Patron( std::string firstName, std::string lastName, std::string address, std::string birthDate, std::string joinDate, std::string ID )
{
	SetFirstName( firstName );

	SetLastName( lastName );

	SetAddress( address );

	SetBirthdate( birthDate );

	SetJoinDate( joinDate );

	SetID( ID );

	_outstandingFees = 0;

	_currentDate = "";
}


Patron::~Patron()
{
}


void Patron::SetFirstName( std::string firstName )
{
	if ( std::regex_match( firstName, std::regex( NAME_PATTERN ) ) )
	{
		_firstName = firstName;
	}
	else
	{
		throw std::logic_error( "Bad argument for first name" );
	}
}


void Patron::SetLastName( std::string lastName )
{
	if ( std::regex_match( lastName, std::regex( NAME_PATTERN ) ) )
	{
		_LastName = lastName;
	}
	else
	{
		throw std::logic_error( "Bad argument for last name" );
	}
}


void Patron::SetAddress( std::string address )
{
	if ( std::regex_match( address, std::regex( ADDRESS_PATTERN ) ) )
	{
		_address = address;
	}
	else
	{
		throw std::logic_error( "Bad argument for address" );
	}
}


void Patron::SetBirthdate( std::string birthDate )
{
	if ( std::regex_match( birthDate, std::regex( DATE_PATTERN ) ) )
	{
		_birthDate = birthDate;
	}
	else
	{
		throw std::logic_error( "Bad argument for birth date" );
	}
}


void Patron::SetJoinDate( std::string joinDate )
{
	if ( std::regex_match( joinDate, std::regex( DATE_PATTERN ) ) )
	{
		_joinDate = joinDate;
	}
	else
	{
		throw std::logic_error( "Bad argument for join date" );
	}
}


void Patron::SetFees( std::string fees )
{
	if ( std::regex_match( fees, std::regex( MONEY_PATTERN ) ) )
	{
		_outstandingFees = stod( fees );
	}
	else
	{
		throw std::logic_error( "Bad argument for outstanding fees" );
	}
}


void Patron::SetID( std::string ID )
{
	if ( std::regex_match( ID, std::regex( ID_PATTERN ) ) )
	{
		_ID = stoi( ID );
	}
	else
	{
		throw std::logic_error( "Bad argument for ID" );
	}
}


void Patron::SetCurrentDate( std::string currentDate )
{
	if ( currentDate == "" ) { _currentDate = Date::GetCurrentDate();  return; }

	if ( std::regex_match( currentDate, std::regex( DATE_PATTERN ) ) )
	{
		_currentDate = currentDate;
	}
	else
	{
		throw std::logic_error( "Bad argument for current date" );
	}
}


const std::string& Patron::GetFirstName()
{
	return _firstName;
}


const std::string& Patron::GetLastName()
{
	return _LastName;
}


const std::string& Patron::GetAddress()
{
	return _address;
}


const std::string& Patron::GetBirthDate()
{
	return _birthDate;
}


const std::string& Patron::GetJoinDate()
{
	return _joinDate;
}


double Patron::GetFees()
{
	return _outstandingFees;
}


size_t Patron::GetID()
{
	return _ID;
}


bool Patron::IsMinor()
{
	std::vector<std::string> splitBirth;
	std::vector<std::string> splitNow;
	std::string now = ( _currentDate == "" ) ? Date::GetCurrentDate() : _currentDate;

	splitBirth = Split( &_birthDate[0], '/' );
	splitNow = Split( &now[0], '/' );

	try
	{
		if ( stoi( splitBirth[2] ) > stoi( splitNow[2] ) - 18 )
		{
			return true;
		}
		else if ( stoi( splitBirth[2] ) == stoi( splitNow[2] ) - 18 )
		{
			if ( stoi( splitBirth[0] ) < stoi( splitNow[0] ) )
			{
				return true;
			}
			else if ( stoi( splitBirth[0] ) == stoi( splitNow[0] ) )
			{
				if ( stoi( splitBirth[1] ) < stoi( splitNow[1] ) ) { return true; }
			}
		}
	}
	catch ( const std::exception& )
	{
		throw std::logic_error( "Could not check if the patron was a minor" );
	}

	return false;
}


void Patron::Write( std::ostream& out )
{
	out <<
		_firstName << "," <<
		_LastName << "," <<
		_address << "," <<
		_birthDate << "," <<
		_joinDate << "," <<
		_outstandingFees << "," <<
		_ID << std::endl;
}


bool Patron::Read( std::istream& in )
{
	std::vector<std::string> splitVector;
	std::string lineInput;

	getline( in, lineInput );

	if ( lineInput != "" )
	{
		splitVector = Split( &lineInput[0], ',' );
	}
	else
	{
		return false;
	}

	try
	{
		if ( splitVector.size() < NUM_VAR ) { throw std::logic_error( "Error loading from patron file" ); }

		SetFirstName( splitVector[0] );
		SetLastName( splitVector[1] );
		SetAddress( splitVector[2] );
		SetBirthdate( splitVector[3] );
		SetJoinDate( splitVector[4] );
		SetFees( splitVector[5] );
		SetID( splitVector[6] );
	}
	catch ( const std::exception& error )
	{
		//std::string errorMessage = "Could not read patron from the file: " + error.what();
		throw std::logic_error( error.what() );
	}

	return true;
}


std::vector<std::string> Patron::Split( char* input, char delim = ' ' )
{
	std::vector<std::string> splitResult;

	do {
		char *begin = input;

		while ( *input != delim && *input )
		{
			input++;
		}

		splitResult.push_back( std::string( begin, input ) );

	} while ( *input++ != 0 );

	return splitResult;
}



#endif PAT_CPP