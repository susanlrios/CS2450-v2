#ifndef LIBRARY_CPP
#define LIBRARY_CPP


#include "Library.h"


Library::Library() 
{
}

// FINISHED
Library::~Library() 
{
	for each (Patron* patron in patrons) 
	{
		delete(patron);
	}

	for each (Book* book in books) 
	{
		delete(book);
	}
}


// FINISHED
std::vector<std::string> Library::Split( char* input, char delim = ' ' )
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


// FINISHED
// NOT TESTED
void Library::setCurrentDate( std::string date )
{
	Date::SetCurrentDate( date );
}


// Need to include error checking
void Library::createBooks(std::istream& in) {
	std::string str;
	while (std::getline(in, str)) {
		books.push_back(new Book());
		books.back()->setBookInfo(str);
	}
}


// FINISHED
void Library::createPatrons( std::istream& in ) 
{
	std::string errorMsg;
	std::string input;

	do
	{
		try
		{
			do {
				patrons.push_back( new Patron() );

			} while ( patrons.back()->Read( in ) );
		}
		catch ( const std::exception& error )
		{
			errorMsg = std::string( "Unable to create patron number " ) + std::to_string( patrons.size() + patronErrorList.size() ) + ". Error: " + error.what();
			patronErrorList.push_back( errorMsg );
		}

		delete( patrons.back() );
		patrons.pop_back();

	} while ( !in.eof() );

	// After collecting all of the patrons, store them into the unordered hash map
	for each ( Patron* patron in patrons )
	{
		patronBooks.insert( patronStatus( patron, std::vector<Book*>() ) );
	}
}


// FINISHED
// NOT TESTED
void Library::restorePBStatus( std::istream& in ) 
{
	std::vector<std::string> splitVector;
	//std::string errorMsg;
	std::string input;

	// while loop that gets every line
	while ( getline( in, input ) )
	{
		if ( input == "" ) { continue; }

		// split the data between the commas
		splitVector = Split( &input[0], ',' );

		// search the unordered hash map for the patron with the matching ID
		auto patronResults = boolinq::from( patrons )
			.where( [&splitVector]( Patron* a ) { return std::to_string( a->GetID() ) == splitVector[0]; } )
			.select( []( Patron* a ) { return a; } )
			.toVector();

		//if ( patronResults.size() == 0 ) { throw std::logic_error( "Invalid patron with checkout status" ); }
		if ( patronResults.size() == 0 ) { patronErrorList.push_back( "Invalid patron with checkout status" ); }

		splitVector.erase( splitVector.begin() );				// remove the patron's ID from the list

		// Find the book with the matching ISBN
		auto bookResults = boolinq::from( books )
			.where( [&splitVector]( Book* a ) { return std::find( splitVector.begin(), splitVector.end(), a->GetISBN() ) != splitVector.end(); } )
			.select( []( Book* a ) { return a; } )
			.toVector();

		//if ( bookResults.size() == 0 ) { throw std::logic_error( "Patron with no valid books checked out" ); }
		if ( bookResults.size() == 0 ) { patronErrorList.push_back( "Patron with no valid books checked out" ); }

		// add all of their checked out books to their unordered hash map
		for each ( Book* book in bookResults )
		{
			// update each book that was checked to to a "checked out" status
			if ( book->GetCheckOutStatus() )
			{
				book->checkOut( std::to_string( patronResults.front()->GetID() ) );
			}

			// possible error here if a book is double checked out in the status database

			patronBooks[patronResults.front()].push_back( book );
		}
	}
}


// FINISHED
void Library::checkout( std::string patronID, std::string bookISBN )
{
	// Find the patron with the matching patronID
	auto patronResults = boolinq::from( patrons )
		.where( [patronID]( Patron* a ) { return std::to_string( a->GetID() ) == patronID; } )
		.select( []( Patron* a ) { return a; } )
		.toVector();

	if ( patronResults.size() == 0 ) { throw std::logic_error( "Could not find patron with ID: " + patronID ); }

	// Find the book with the matching ISBN
	auto bookResults = boolinq::from( books )
		.where( [bookISBN]( Book* a ) { return a->GetISBN() == bookISBN; } )
		.select( []( Book* a ) { return a; } )
		.toVector();

	if ( bookResults.size() == 0 ) { throw std::logic_error( "Could not check out book with ISBN: " + bookISBN ); }

	// check to make sure they can check out the book
	if ( patronResults.front()->IsMinor() && bookResults.front()->GetType() != TYPE_CHILD )
	{
		throw std::logic_error( "Patron must be an adult to check out this item" );
	}

	// check to make sure they have not exceeded their checkout limit
	if ( patronResults.front()->IsMinor() && patronBooks[patronResults.front()].size() >= MAX_CHILD_CHECKOUT )
	{
		throw std::logic_error( "Patron has checked out too many items, check-in something before checking out new items" );
	}

	if ( !patronResults.front()->IsMinor() && patronBooks[patronResults.front()].size() >= MAX_ADULT_CHECKOUT )
	{
		throw std::logic_error( "Patron has checked out too many items, check-in something before checking out new items" );
	}

	// update the book to a "checked out" status
	if ( bookResults.front()->GetCheckOutStatus() )
	{
		bookResults.front()->checkOut( std::to_string( patronResults.front()->GetID() ) );
	}
	else
	{
		throw std::logic_error( bookResults.front()->GetTitle() + " is already checked out" );
	}

	// add the book pointer to the patron's unordered hash map
	patronBooks[patronResults.front()].push_back( bookResults.front() );
}


// NOT FINISHED
void Library::writeBooksToDB(std::ostream& out) 
{
	for each ( Book* book in books )
	{
		book->Write( out );
	}
}


// FINISHED
void Library::writePatronsToDB( std::ostream& out )
{
	for each ( Patron* patron in patrons )
	{
		patron->Write( out );
	}
}


// FINISHED
void Library::writePBStatusToDB( std::ostream& out ) 
{
	// iterate through the unordered hash map and save the patron's 
	// ID and books they've checked out on a single line
	for ( auto iterator = patronBooks.begin(); iterator != patronBooks.end(); iterator++ )
	{
		if ( iterator->second.size() > 0 )						// Check to make sure this patron has checked out books
		{
			out << iterator->first->GetID();

			for each ( Book* book in iterator->second )
			{
				out << "," << book->GetISBN();
			}

			out << std::endl;
		}
	}
}


std::vector<Patron*> Library::FindPatron( std::string input )
{
	// Find the patron with the matching patronID
	auto patronResults = boolinq::from( patrons )
		.where( [input]( Patron* a ) 
			{ 
				if ( std::to_string( a->GetID() ) == input ) return true;
				if ( std::to_string( a->GetFees() ) == input ) return true;
				if ( a->GetFirstName() + " " + a->GetLastName() == input ) return true;
				if ( a->GetFirstName() == input ) return true;
				if ( a->GetLastName() == input ) return true;
				if ( a->GetBirthDate() == input ) return true;
				if ( a->GetAddress() == input ) return true;
				return false; 
			} )
		.select( []( Patron* a ) { return a; } )
		.toVector();

	return patronResults;
}


// FINISHED
bool Library::errorsFound()
{
	return patronErrorList.size() + bookErrorList.size() + checkoutErrorList.size();
}


// FINISHED
void Library::displayErrors( std::ostream& out )
{
	for each ( std::string error in patronErrorList )
	{
		out << error << std::endl;
	}

	// List the book import errors
	for each ( std::string error in bookErrorList )
	{
		out << error << std::endl;
	}

	// List the PBStatus import errors
	for each ( std::string error in checkoutErrorList )
	{
		out << error << std::endl;
	}
}


// FINISHED
void Library::displayPatrons( std::ostream& out )
{
	out << std::endl << "Patrons:" << std::endl;

	for each ( Patron* patron in patrons )
	{
		patron->Display( out );
		//out << std::endl;
	}
}


// NOT FINISHED
void Library::displayBooks( std::ostream& out )
{
	for each ( Book* book in books )
	{
		//book->Display( out );
		out << std::endl;
	}
}



#endif