// Main v0.2

// *************************
// Todo List

// 1. Decided if I want one search function but pass it function pointers
// 2. Setup the current date stuff in the library class
// 3. Make it so the user can create a new patron
// 4. Make it so the program will save the databases at exit
// 5. Change the book class's check out function to include a patron pointer
// 7. Change book's ISBN type to string
// 8. Decide if I only want the patron pointers in the hash map
// *************************

// *************************
// Error List

// 1. Improve book class' import error checking
// 2. change book class' ISBN type to string
// 3. Fix the current date getter in the patron class
// 4. Fix the issue with losing database data when running into a save error
// 5. Fix the clear sreen function so it doesn't use a system call
// 6. Fix the enum / switch statement in main
// *************************



#include<string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Library.h"


void ClearScreen()
{
	system( "cls" );							// look into removing this
	//std::cout << std::string( 100, '\n' );
}




int main( int argc, const char * argv[] ) 
{
	enum Option { checkIN = 1, checkOUT, listBooks, listOverdue, listByPatron, changeDate, exit };
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


	// Load from the patron-bookCheckout-status database



	if ( library->errorsFound() )
	{
		std::cout << "Import Errors:" << std::endl;

		library->displayErrors( std::cout );

		std::cout << std::endl;
	}

    do {
        std::cout << "1-Check in book" << std::endl;
        std::cout << "2-Check out book" << std::endl;
        std::cout << "3-List all books" << std::endl;
        std::cout << "4-List all overdue books" << std::endl;
        std::cout << "5-List all books checked out by a patron" << std::endl;
        std::cout << "6-Change current date" << std::endl;
		std::cout << "7-Exit" << std::endl << std::endl;
		std::cout << "What would you like to do? ";
		getline( std::cin, input );

		try
		{
			switch ( stoi( input ) )
			{
			case 1:				// Check in a book
				// search book
				// see if it's actually checked out
				// check in
				// remove book from patron's unsorted map

				break;
			case 2:				// Check out a book
				// list patrons
				// get user input for patron
				// search patrons
				// search books
				// see if patron meets the requirements
				// see if patron has checked out too many books
				// checkout book

				library->checkout( "0", "98298347" );

				break;
			case 3:				// List all books
				// pass std::cout ref to function to display all books

				break;
			case 4:				// List all overdue books
				// pass std::cout ref to function
				// preform a LINQ query on all overdue books
				// display overdue books

				break;
			case 5:				// List all books checked out by patron
				// search patron
				// pass std::cout ref to function to display books checked by patron

				break;
			case 6:				// Change the current date
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
			}
		}
		catch ( const std::exception& error )
		{
			ClearScreen();
			std::cout << error.what() << std::endl << "Please enter a valid input" << std::endl << std::endl;
			continue;
		}

		ClearScreen();

    } while ( input != "7" );


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
	/*
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
	*/
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
