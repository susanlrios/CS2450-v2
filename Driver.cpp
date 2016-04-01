

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "Patron.h"
#include "Book.h"


int main( int argc, char* argv[] )
{
	std::unordered_map<Patron*, std::vector<Book*>> testHashMap;
	std::vector<Patron*> patrons;
	std::vector<Book*> books;
	std::ostringstream convert;
	std::string input;


	try
	{
		for ( size_t i = 0; i < 10; i++ )
		{
			convert.str( "" );
			convert.clear();
			convert << i;
			patrons.push_back( new Patron( "Chris", "Mansfield", "1234 Street Name_Orem_UT", "4/20/1993", "3/26/2016", convert.str() ) );
		}

		for ( size_t i = 0; i < 10; i++ )
		{
			convert.str( "" );
			convert.clear();
			convert << i;
			books.push_back( new Book( (std::string("Book" + convert.str() ) ), "Cam", "adult", i ) );
		}
	}
	catch ( const std::exception& err )
	{
		std::cout << err.what() << std::endl;

		std::cin.clear();
		do {
			getline( std::cin, input );
		} while ( input != "" );

		return 0;
	}




	// Testing unordered maps

	testHashMap[patrons[0]] = std::vector<Book*>();

	std::cout << "Testing an unordered hash map" << std::endl << std::endl;

	for ( size_t i = 0; i < 4; i++ )
	{
		std::cout << "Checking out " << books[i]->GetTitle() << " To " << patrons[0]->GetFirstName() << " " << patrons[0]->GetLastName() << std::endl;

		books[i]->checkOut( patrons[0] );

		testHashMap[patrons[0]].push_back( books[i] );			// Add the book to the patron's vector of checked out books
	}

	std::cout << std::endl << "Number of books checked out to " << patrons[0]->GetFirstName() << " " << patrons[0]->GetLastName() << ": " << testHashMap[patrons[0]].size() << std::endl;

	// End map test





	std::cout << "\nPlease enter the current date or press enter for today's date" << std::endl;

	do
	{
		try
		{
			getline( std::cin, input );

			Patron::SetCurrentDate( input );
		}
		catch ( const std::exception& error )
		{
			std::cout << error.what() << std::endl;

			input = "-1";
		}
	} while ( input == "-1" );


	// Based on the current date entered, check to see if the patron is a minor
	if ( patrons[0]->IsMinor() )								
	{
		std::cout << "They are a minor" << std::endl;
	}
	else
	{
		std::cout << "They are not minor" << std::endl;
	}





	for each ( Patron* patron in patrons )			// Delete all of the patrons
	{
		delete( patron );
	}

	for each ( Book* book in books )				// Delete all of the books
	{
		delete( book );
	}

	std::cin.clear();
	do {
		getline( std::cin, input );
	} while ( input != "" );

	return 0;
}