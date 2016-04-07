// Main v0.6

// *************************
// Todo List

// 1. Decided if the search function will be passed function pointers
// 3. Make it so the user can create a new patron
// 5. Change the book class's check out function to include a void pointer
// 8. Decide if the only patron pointers reside in the hash map
// *************************

// *************************
// Error List

// 4. Fix the issue with losing database data when running into a save error
// 5. Fix the clear sreen function so it doesn't use a system call
// *************************



#include<string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

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
	system( "cls" );							// look into removing this
	//std::cout << std::string( 100, '\n' );
}


const std::string SearchMenu( Library& library )
{
	std::string patronResult;
	std::string bookResult;
	std::string input = "";

	do
	{
		ClearScreen();

		std::cout << std::endl << " Search by patron or by book?" << std::endl <<
			"1-Search by book" << std::endl <<
			"2-Search by Patron" << std::endl <<
			"3-Return to main menu" << std::endl;

		getline( std::cin, input );

		switch ( stoi( input ) )
		{
		case searchByBook:
			// Display all books
			// Get user input for the desired book
			// Return books ISBN

			break;
		case searchByPatron:
			// Display all patrons
			library.displayPatrons( std::cout );

			
			do {
				// Get user input for the desired patron
				std::cout << std::endl << "Which patron would you like to select? ";
				getline( std::cin, input );

				// Return the Patron's ID
				patronResult = library.FindPatron( input, std::cout );

			} while ( patronResult == "-1" );

			ClearScreen();
			std::cout << std::endl;

			do {
				// list the books checked out by patron, and return the Book's ISBN
				library.ListBooksByPatron( patronResult, std::cout );

				// Get user input for the desired patron
				std::cout << std::endl << "Which book would you like to check-in? ";
				getline( std::cin, input );

				//try to check in the book entered

			} while ( bookResult == "-1" );




			break;
		}
	} while ( input != std::to_string( exitToMain ) );

	return "-1";
}



int main( int argc, const char * argv[] ) 
{
	Library* library = new Library();
	std::string patronFileName = "patronDB.csv";
	std::string bookFileName = "bookDB.csv";
	std::string statusFileName = "status.csv";
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
		std::cout << "Current Date: " << Date::GetCurrentDate() << std::endl << std::endl <<
			"1-Check in book" << std::endl <<
			"2-Check out book" << std::endl <<
			"3-List all books" << std::endl <<
			"4-List all overdue books" << std::endl <<
			"5-List all books checked out by a patron" << std::endl <<
			"6-Change current date" << std::endl <<
			"7-Advance the date by one day" << std::endl <<
			"8-Exit" << std::endl << std::endl <<
			"What would you like to do? ";

		getline( std::cin, input );

		try
		{
			switch ( stoi( input ) )
			{
			case checkIN:					// Check in a book
				std::cout << "Checking in a book.";
				SearchMenu( *library );

				// search book
				// see if it's actually checked out
				// check in
				// remove book from patron's unsorted map

				break;
			case checkOUT:					// Check out a book
				// list patrons
				// get user input for patron
				// search patrons
				// search books
				// see if patron meets the requirements
				// see if patron has checked out too many books
				// checkout book

				library->checkout( "0", "98298347" );
				library->checkout( "0", "92384723" );
				library->checkout( "0", "0131103628" );
				library->checkout( "0", "1598634682" );

				break;
			case listBooks:					// List all books
				// pass std::cout ref to function to display all books

				break;
			case listOverdue:				// List all overdue books
				// pass std::cout ref to function
				// preform a LINQ query on all overdue books
				// display overdue books

				break;
			case listByPatron:				// List all books checked out by patron
				// search patron
				// pass std::cout ref to function to display books checked by patron

				break;
			case changeDate:				// Change the current date
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

				break;

			case addDay:				// Add one day to the current date
				Date::AddDayToCurrent();
				break;
			}
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
