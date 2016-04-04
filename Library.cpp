#ifndef LIBRARY_CPP
#define LIBRARY_CPP


#include "Library.h"


Library::Library() 
{
}

// NOT FINISHED
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


// NOT FINISHED
void Library::setCurrentDate( std::string date )
{
	Patron::SetCurrentDate( date );

	// Set the book's current Date
}


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

void Library::restorePBStatus( std::istream& in ) 
{
	// while loop that gets every line
	// getline
	// split the data between the commas
	// search the unordered hash map for the patron with the matching ID
	// add all of their checked out books to their unordered hash map
	// update each book that was checked to to a "checked out" status
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
		.where( [bookISBN]( Book* a ) { return std::to_string( a->GetISBN() ) == bookISBN; } )
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
	if ( !bookResults.front()->checkOut() )
	{
		throw std::logic_error( bookResults.front()->GetTitle() + " is already checked out" );
	}

	// add the book pointer to the patron's unordered hash map
	patronBooks[patronResults.front()].push_back( bookResults.front() );
}


void Library::tryCheckout() {
	for ( int i = 0; i < 2; i++ ) {
		if ( books[i]->checkOut() == true ) {
			patronBooks[patrons[i]].push_back( books[i] );
		}
		else {
			std::cout << "Cannot check out: " << books[i]->GetTitle() << std::endl;
		}
	}

	for ( int q = 0; q < patronBooks.size(); q++ ) {
		std::cout << "Number of books checked out to " << patrons[q]->GetFirstName() << " " << patrons[q]->GetLastName() << " " << patrons[q]->GetID() << ": " << patronBooks[patrons[q]].size() << std::endl;
	}
}


void Library::writeBooksToDB(std::ostream& out) 
{

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


// NOT FINISHED
bool Library::errorsFound()
{
	// include the other errorlists here
	return patronErrorList.size();
}


// NOT FINISHED
void Library::displayErrors( std::ostream& out )
{
	for each ( std::string error in patronErrorList )
	{
		out << error << std::endl;
	}

	// List the book import errors

	// List the PBStatus import errors
}



#endif