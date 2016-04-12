/*
	c++ Group 4
		Team Lead: Susan Rios
		QA: Jon Dale
		Developer: Cameron Brown
		Developer: Chris Mansfield

	Library program v1.1
	Meets all requirements and some additional features
*/

#define version 1.1

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <vector>
#include<string>

#include "Library.h"

enum OptionMain {
	checkIN = 1,
	checkOUT = 2,
	listBooks = 3,
	listOverdue = 4,
	listByPatron = 5,
	changeDate = 6,
	addDay = 7,
	exitProgram = 8
};

enum OptionSearch {
	searchByBook = 1,
	searchByPatron = 2,
	exitToMain = 3
};



void ClearScreen()
{
	system( "cls" );
}


const std::string SearchPatron( Library& library )
{
	std::string patronResult = "-1";
	std::string input;

	ClearScreen();

	// Display all patrons
	library.displayPatrons( std::cout );

	do {
		// Get user input for the desired patron
		std::cout << std::endl << "Type \"back\" to return to the previous menu" << std::endl << "Which patron would you like to select? ";
		getline( std::cin, input );

		// Return the Patron's ID
		if ( input != "back" )
		{
			patronResult = library.FindPatron( input, std::cout );
		}

	} while ( patronResult == "-1" && input != "back" );

	return patronResult;
}


const std::string SearchCheckedOutBooks( Library& library, const std::string& patronID = "" )
{
	std::string bookResult = "-1";
	std::string input;

	ClearScreen();
	std::cout << std::endl;

	// Display all books
	library.ListCheckedOut( std::cout );

	// Get user input for the desired book
	do {
		ClearScreen();
		std::cout << std::endl;

		// list the books checked out by patron, and return the Book's ISBN
		if ( patronID == "" )
		{
			library.ListCheckedOut( std::cout );
		}
		else
		{
			library.ListBooksByPatron( patronID, std::cout );
		}

		// Get user input for the desired patron
		std::cout << std::endl << "Type \"back\" to return to the previous menu" << std::endl << "Which book would you like to select? ";
		getline( std::cin, input );

		//try to check in the book entered
		if ( input != "back" )
		{
			std::ofstream temp;
			bookResult = library.FindBook( input, temp );
		}


	} while ( bookResult == "-1" && input != "back" );

	return bookResult;
}


const std::string SearchMenu( Library& library )
{
	std::string patronResult;
	std::string bookResult;
	std::string input = "";

	do
	{
		ClearScreen();

		std::cout << std::endl << " Check-In a Book" << std::endl << "Search by patron or by book?" << std::endl <<
			"1-Search by book" << std::endl <<
			"2-Search by Patron" << std::endl <<
			"3-Return to main menu" << std::endl;

		getline( std::cin, input );

		switch ( stoi( input ) )
		{
		case searchByBook:

			bookResult = SearchCheckedOutBooks( library );

			if ( bookResult == "-1" ) { break; }

			// try to check in book
			library.checkin( "", bookResult );

			std::cout << std::endl << "Book Successfully Checked-In" << std::endl;
			getline( std::cin, input );

			break;
		case searchByPatron:
			// Search patrons
			patronResult = SearchPatron( library );

			if ( patronResult == "-1" ) { break; }

			bookResult = SearchCheckedOutBooks( library, patronResult );

			if ( bookResult == "-1" ) { break; }

			// try to check in book
			library.checkin( patronResult, bookResult );

			std::cout << std::endl << "Book Successfully Checked-In" << std::endl;
			getline( std::cin, input );

			break;
		}

	} while ( input != std::to_string( exitToMain ) );

	return "-1";
}


void SearchCheckout( Library& library )
{
	std::string bookInput = "";
	std::string patronResult;
	std::string bookResult = "";
	std::string input = "";


	ClearScreen();
	std::cout << std::endl;

	do
	{
		try
		{
			patronResult = SearchPatron( library );

			if ( patronResult == "-1" ) { return; }

			do {
				// list all of the books
				if ( bookResult == "" )
				{
					ClearScreen();
					std::cout << std::endl;

					library.displayBooks( std::cout );
				}

				// Get user input for the desired patron
				std::cout << std::endl << "Type \"back\" to return to the previous menu" << std::endl << "Which book would you like to check-out? ";
				getline( std::cin, input );

				ClearScreen();
				std::cout << std::endl;

				//std::ofstream temp;
				if ( input != "back" )
				{
					bookResult = library.FindBook( input, std::cout );
				}

			} while ( bookResult == "-1" && input != "back" );

			if ( input == "back" ) { return; }

			//try to check out the book
			library.checkout( patronResult, bookResult );

			std::cout << std::endl << "Book Successfully Checked-Out" << std::endl;
			getline( std::cin, input );
		}
		catch ( const std::exception& error )
		{
			ClearScreen();
			std::cout << "Error checking-out book: " << error.what() << std::endl;
			getline( std::cin, input );
			input = "-1";
		}
	} while ( input == "-1" );
}




int main( int argc, const char * argv[] ) 
{
	Library* library = new Library();
	std::string patronFileName = "patronDB.csv";
	std::string bookFileName = "bookDB.csv";
	std::string statusFileName = "status.csv";
	std::string patronResult;
	std::string input;
	std::ifstream fileIn;
	std::ofstream fileOut;


	// Load from the patron database
	fileIn.open( patronFileName );

	if ( !fileIn.is_open() )
	{
		std::cout << "No file found: " << patronFileName << std::endl;
	}
	else
	{
		library->createPatrons( fileIn );
	}

	fileIn.close();
	// End load from patron database

	// Load from the book database
	fileIn.open( bookFileName );

	if ( !fileIn.is_open() )
	{
		std::cout << "No file found: " << bookFileName << std::endl;
	}
	else
	{
		library->createBooks( fileIn );
	}

	fileIn.close();
	// End load from book database

	// Load from the staus database
	fileIn.open( statusFileName );

	if ( !fileIn.is_open() )
	{
		std::cout << "No file found: " << statusFileName << std::endl;
	}
	else
	{
		library->restorePBStatus( fileIn );
	}

	fileIn.close();
	// End load from status database

	if ( library->errorsFound() )
	{
		std::cout << "Import Errors:" << std::endl;

		library->displayErrors( std::cout );

		std::cout << std::endl;
	}

    do {
		std::cout << std::endl << "Library v" << version << std::endl << 
			"Current Date: " << library->GetCurrentDate() << std::endl << std::endl <<
			"1-Check in media" << std::endl <<
			"2-Check out media" << std::endl <<
			"3-List all media" << std::endl <<
			"4-List all overdue media" << std::endl <<
			"5-List all media checked out by a patron" << std::endl <<
			"6-Change current date" << std::endl <<
			"7-Advance the date by one day" << std::endl <<
			"8-Exit" << std::endl << std::endl <<
			"What would you like to do? ";

		getline( std::cin, input );

		try
		{
			if ( input == std::to_string( checkIN ) )
			{
				// search book, and see if it's actually checked out
				// check in
				std::cout << std::endl << "Checking in a media item.";
				SearchMenu( *library );
			}
			else if ( input == std::to_string( checkOUT ) )
			{
				// list patrons
				// get user input for patron
				// search patrons
				// search books
				// see if patron meets the requirements
				// see if patron has checked out too many books
				// checkout book
				std::cout << std::endl << "Checking out a media item.";
				SearchCheckout( *library );
			}
			else if ( input == std::to_string( listBooks ) )
			{
				std::cout << "All Library Media:" << std::endl << std::endl;

				// pass std::cout ref to function to display all books
				library->displayBooks( std::cout );

				getline( std::cin, input );
			}
			else if ( input == std::to_string( listOverdue ) )
			{
				ClearScreen();
				std::cout << std::endl << "All Overdue Media: " << std::endl;

				// pass std::cout ref to function
				// preform a LINQ query on all overdue books
				// display overdue books
				library->ListOverdueBooks( std::cout );

				getline( std::cin, input );
			}
			else if ( input == std::to_string( listByPatron ) )
			{
				// search patron
				patronResult = SearchPatron( *library );

				if ( patronResult == "-1" ) { break; }

				ClearScreen();
				std::cout << std::endl;
				// pass std::cout ref to function to display books checked by patron
				library->ListBooksByPatron( patronResult, std::cout );

				getline( std::cin, input );
			}
			else if ( input == std::to_string( changeDate ) )
			{
				std::cout << "\nPlease enter the current date" << std::endl <<
					"example: 02/04/2016 or 2/4/2016" << std::endl;

				do
				{
					try
					{
						getline( std::cin, input );

						library->setCurrentDate( input );
					}
					catch ( const std::exception& error )
					{
						std::cout << error.what() << std::endl;

						input = "-1";
					}
				} while ( input == "-1" );
			}
			else if ( input == std::to_string( addDay ) )
			{
				library->AddDayToCurrent();
			}
			else if ( input == std::to_string( exitProgram ) )
			{

			}
			else
			{
				ClearScreen();
				// If none of the main menu options have been selected
				// then attempt a smart search on the info entered
				if ( !library->SmartSearch( input, std::cout ) )
				{
					std::cout << std::endl << "Unable to complete request, type \"help\" at the main menu for more information" << std::endl <<
						"Press [ENTER] to return to main menu" << std::endl;
				}

				getline( std::cin, input );
			}
		}
		catch ( std::invalid_argument& error )
		{
			ClearScreen();
			std::cout << "Please enter a valid input" << std::endl << std::endl;
			continue;
		}
		catch ( const std::exception& error )
		{
			ClearScreen();
			std::cout << error.what() << std::endl << "Please enter a valid input" << std::endl << std::endl;
			continue;
		}

		ClearScreen();

    } while ( input != std::to_string( exitProgram ) );


	// Save the databases to their files
	std::cout << "Saving databases" << std::endl;

	// Start of code to save patron database
	fileOut.open( patronFileName );

	if ( !fileOut.is_open() )
	{
		std::cout << "Error: could not save file " << patronFileName << std::endl;
	}
	else
	{
		library->writePatronsToDB( fileOut );
	}

	fileOut.close();
	// End of code to save patron database

	// Start of code to save the book database
	fileOut.open( bookFileName );

	if ( !fileOut.is_open() )
	{
		std::cout << "Error: could not save file " << bookFileName << std::endl;
	}
	else
	{
		library->writeBooksToDB( fileOut );
	}

	fileOut.close();
	// End of code to save the book database

	// Start of code to save the checkout status database
	fileOut.open( statusFileName );

	if ( !fileOut.is_open() )
	{
		std::cout << "Error: could not save file " << statusFileName << std::endl;
	}
	else
	{
		library->writePBStatusToDB( fileOut );
	}

	fileOut.close();
	// End of code to save the checkout status database

	// Remove the library object from the heap
	delete( library );

    return 0;
}
