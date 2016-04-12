#ifndef LIBRARY_CPP
#define LIBRARY_CPP


#include "Library.h"

// Library's default constructor
Library::Library()
{
	feeValue = FEE_INCREMENT_AMOUNT;
}

// Library's deconstructor
Library::~Library()
{
	for each ( Patron* patron in patrons )
	{
		delete( patron );
	}

	for each ( Book* book in books )
	{
		delete( book );
	}
}


/// <summary> Splits a string passed by the delimiter passed </summary>
/// <param name="input"> A character pointer to a string </param>
/// <param name="delim"> A character that will be used to split the string </param>
/// <returns> vector of strings </returns>
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


/// <summary> Passes the users input for a new date to the Date class </summary>
/// <param name="date"> A string with the new date value </param>
/// <returns> void </returns>
void Library::setCurrentDate( std::string date )
{
	Date::SetCurrentDate( date );
}


/// <summary> Creates and populates media objects </summary>
/// <param name="in"> reference to an istream buffer object </param>
/// <returns> void </returns>
void Library::createBooks( std::istream& in ) {
	std::string errorMsg;
	std::string str;

	while ( std::getline( in, str ) ) {
		try {
			books.push_back( new Book() );
			books.back()->setBookInfo( str );
		}
		catch ( const std::exception& error ) {
			errorMsg = std::string( "Unable to create book with: " ) + std::string( books.back()->GetTitle() );
			bookErrorList.push_back( errorMsg );
			delete( books.back() );
			books.pop_back();
		}
	}
}


/// <summary> Creates and populates patron objects </summary>
/// <param name="in"> reference to an istream buffer object </param>
/// <returns> void </returns>
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


/// <summary> Populates the unordered hash map object </summary>
/// <param name="in"> reference to an istream buffer object </param>
/// <returns> void </returns>
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

		if ( patronResults.size() == 0 ) { patronErrorList.push_back( "Invalid patron with checkout status" ); continue; }

		splitVector.erase( splitVector.begin() );				// remove the patron's ID from the list

																// Find the book with the matching ISBN
		auto bookResults = boolinq::from( books )
			.where( [&splitVector]( Book* a ) { return std::find( splitVector.begin(), splitVector.end(), a->GetISBN() ) != splitVector.end(); } )
			.select( []( Book* a ) { return a; } )
			.toVector();

		if ( bookResults.size() == 0 ) { patronErrorList.push_back( "Patron with no valid media checked out" ); }

		// add all of their checked out books to their unordered hash map
		for each ( Book* book in bookResults )
		{
			// update each book that was checked to to a "checked out" status
			if ( !book->GetCheckOutStatus() )
			{
				book->checkOut( std::to_string( patronResults.front()->GetID() ) );
			}

			// possible error here if a book is double checked out in the status database

			patronBooks[patronResults.front()].push_back( book );
		}
	}
}


/// <summary> Attempts to checkout a media item to a patron </summary>
/// <param name="patronID"> A string with the patron's ID </param>
/// <param name="bookISBN"> A string with the book's ISBN </param>
/// <returns> void </returns>
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
	if ( patronResults.front()->IsMinor() && bookResults.front()->GetType() != "child" )
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
	if ( !bookResults.front()->GetCheckOutStatus() )
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


/// <summary> Attempts to checkin a media item to a patron </summary>
/// <param name="patronID"> A string with the patron's ID </param>
/// <param name="bookISBN"> A string with the book's ISBN </param>
/// <returns> void </returns>
void Library::checkin( std::string patronID, std::string bookISBN )
{
	size_t index = 0;

	// Find the book with the matching ISBN
	auto bookResults = boolinq::from( books )
		.where( [bookISBN]( Book* a ) { return a->GetISBN() == bookISBN; } )
		.select( []( Book* a ) { return a; } )
		.toVector();

	if ( bookResults.size() == 0 ) { throw std::logic_error( "Could not check in book with ISBN: " + bookISBN ); }

	if ( bookResults.size() > SINGLE_OBJECT_SELECTED ) { throw std::logic_error( "Could not check in book, too many books with the same ISBN: " + bookISBN ); }

	if ( patronID == "" )
	{
		patronID = bookResults.front()->GetPatronID();
	}

	// Find the patron with the matching patronID
	auto patronResults = boolinq::from( patrons )
		.where( [patronID]( Patron* a ) { return std::to_string( a->GetID() ) == patronID; } )
		.select( []( Patron* a ) { return a; } )
		.toVector();

	if ( patronResults.size() == 0 ) { throw std::logic_error( "Could not find patron with ID: " + patronID ); }

	// If the item is overdue then increase their fees owed
	if ( bookResults.front()->Overdue() )
	{
		patronResults.front()->SetFees( std::to_string( patronResults.front()->GetFees() + FEE_INCREMENT_AMOUNT ) );
	}

	// update the book to a "checked in" status
	if ( bookResults.front()->GetCheckOutStatus() )
	{
		bookResults.front()->checkIn();
	}
	else
	{
		throw std::logic_error( bookResults.front()->GetTitle() + " is already checked in" );
	}

	// Find the book in the patron's unordered hash map and remove it
	while ( patronBooks[patronResults.front()][index] != bookResults.front() && index < patronBooks[patronResults.front()].size() )
	{
		index++;
	}

	patronBooks[patronResults.front()].erase( patronBooks[patronResults.front()].begin() + index );
}


/// <summary> Writes all of the media objects to a database flie </summary>
/// <param name="out"> reference to an ostream buffer object </param>
/// <returns> void </returns>
void Library::writeBooksToDB( std::ostream& out )
{
	for each ( Book* book in books )
	{
		book->Write( out );
	}
}


/// <summary> Writes all of the patron objects to a database flie </summary>
/// <param name="out"> reference to an ostream buffer object </param>
/// <returns> void </returns>
void Library::writePatronsToDB( std::ostream& out )
{
	for each ( Patron* patron in patrons )
	{
		patron->Write( out );
	}
}


/// <summary> Writes all of the checkout status to a database flie </summary>
/// <param name="out"> reference to an ostream buffer object </param>
/// <returns> void </returns>
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


/// <summary> Attempts to find a patron </summary>
/// <param name="input"> A string with the patron's information </param>
/// <param name="out"> reference to an ostream buffer object </param>
/// <returns> void </returns>
std::string Library::FindPatron( std::string input, std::ostream& out )
{
	bool( *contain )( std::string, std::string );
	contain = contains;

	// Find the patron with the matching patronID
	auto patronResults = boolinq::from( patrons )
		.where( [input]( Patron* a ) { return std::to_string( a->GetID() ) == input; } )
		.select( []( Patron* a ) { return a; } )
		.toVector();

	if ( patronResults.size() == 0 )
	{
		// Find the patron with any matching data
		patronResults = boolinq::from( patrons )
			.where( [input, contain]( Patron* a )
		{
			std::string temp = a->GetFirstName() + " " + a->GetLastName();
			if ( std::to_string( a->GetFees() ) == input ) return true;
			if ( contain( temp, input ) ) return true;
			if ( a->GetBirthDate() == input ) return true;
			if ( contain( a->GetAddress(), input ) ) return true;
			return false;
		} )
			.select( []( Patron* a ) { return a; } )
			.toVector();
	}

	for each ( auto patron in patronResults )
	{
		patron->Display( out );
	}

	if ( patronResults.size() != SINGLE_OBJECT_SELECTED ) { return "-1"; }

	return std::to_string( patronResults.front()->GetID() );
}


/// <summary> Attempts to find a book </summary>
/// <param name="input"> A string with the media's information </param>
/// <param name="out"> reference to an ostream buffer object </param>
/// <returns> void </returns>
std::string Library::FindBook( std::string bookInfo, std::ostream& out )
{
	bool( *contain )( std::string, std::string );
	contain = contains;

	// Find the book with the matching ISBN
	auto bookResults = boolinq::from( books )
		.where( [bookInfo, contain]( Book* a )
	{
		if ( a->GetISBN() == bookInfo ) { return true; }
		if ( contain( a->GetTitle(), bookInfo ) ) { return true; }
		if ( contain( a->GetAuthor(), bookInfo ) ) { return true; }
		if ( contain( a->GetType(), bookInfo ) ) { return true; }
		return false;
	} )
		.select( []( Book* a ) { return a; } )
		.toVector();

	for each ( Book* book in bookResults )
	{
		book->Display( out );
		out << std::endl;
	}

	if ( bookResults.size() != SINGLE_OBJECT_SELECTED ) { return "-1"; }

	return bookResults.front()->GetISBN();
}


/// <summary> Lists all media items checked out by a patron </summary>
/// <param name="input"> A string with the patron's ID </param>
/// <param name="out"> reference to an ostream buffer object </param>
/// <returns> void </returns>
void Library::ListBooksByPatron( std::string input, std::ostream& out )
{
	// Find the patron with the matching patronID
	auto patronResults = boolinq::from( patrons )
		.where( [input]( Patron* a ) { return std::to_string( a->GetID() ) == input; } )
		.select( []( Patron* a ) { return a; } )
		.toVector();

	out << "Patron: ";
	patronResults.front()->Display( out );
	out << std::endl << "Checked-out Media:" << std::endl;

	for each ( Book* book in patronBooks[patronResults.front()] )
	{
		book->Display( out );
		out << std::endl;
	}
}


/// <summary> Lists all media items that are overdue </summary>
/// <param name="out"> reference to an ostream buffer object </param>
/// <returns> void </returns>
void Library::ListOverdueBooks( std::ostream& out )
{
	// Find the overdue books
	auto bookResults = boolinq::from( books )
		.where( []( Book* a ) { return a->Overdue(); } )
		.select( []( Book* a ) { return a; } )
		.toVector();

	if ( bookResults.size() == 0 ) { out << "There is currently no overdue media" << std::endl; }

	out << std::endl;

	for each ( Book* book in bookResults )
	{
		book->Display( out );

		auto patronResults = boolinq::from( patrons )
			.where( [book]( Patron* a ) { return std::to_string( a->GetID() ) == book->GetPatronID(); } )
			.select( []( Patron* a ) { return a; } )
			.toVector();

		if ( patronResults.size() != SINGLE_OBJECT_SELECTED ) { throw std::logic_error( "Book checked-out with bad patron ID" ); }

		out << "\tChecked-out to Patron with ID:" << patronResults.front()->GetID() << " " <<
			patronResults.front()->GetFirstName() << " " << patronResults.front()->GetLastName() << std::endl << std::endl;
	}
}


/// <summary> Lists all overdue media items checked out by a patron </summary>
/// <param name="out"> reference to an ostream buffer object </param>
/// <returns> void </returns>
void Library::ListOverdueBooksByPatron( std::ostream& out )
{
	std::vector<Patron*> overduePatrons;

	for each ( Patron* patron in patrons )
	{
		// Find the overdue books
		auto bookResults = boolinq::from( patronBooks[patron] )
		.where( []( Book* a ) { return a->Overdue(); } )
		.select( []( Book* a ) { return a; } )
		.toVector();

		if ( bookResults.size() > 0 )
		{
			overduePatrons.push_back( patron );
		}
	}

	if ( overduePatrons.size() == 0 ) { out << "There is currently no overdue media" << std::endl; return; }

	out << std::endl << "List of patrons with overdue media: " << std::endl << std::endl;

	for each ( Patron* patron in overduePatrons )
	{
		patron->Display( out );
	}
}


/// <summary> Lists all checked out media items </summary>
/// <param name="out"> reference to an ostream buffer object </param>
/// <returns> void </returns>
void Library::ListCheckedOut( std::ostream& out )
{
	out << "All Media Currently Checked-out: " << std::endl << std::endl;

	// Find the checked out books
	auto bookResults = boolinq::from( books )
		.where( []( Book* a ) { return a->GetCheckOutStatus(); } )
		.select( []( Book* a ) { return a; } )
		.toVector();

	for each ( Book* book in bookResults )
	{
		book->Display( out );
		out << std::endl;
	}
}


/// <summary> Gets the current date from the Date class </summary>
/// <returns> const reference string </returns>
const std::string& Library::GetCurrentDate()
{
	return Date::GetCurrentDate();
}


/// <summary> Begins the Smart Search logic tree, tries to complete a user's request </summary>
/// <param name="request"> string containing the user's request </param>
/// <param name="out"> reference to an ostream buffer object </param>
/// <returns> bool </returns>
bool Library::SmartSearch( const std::string& request, std::ostream& out )
{
	std::regex base_regex( COMMAND_PATTERN );
	std::ssub_match base_sub_match;
	std::smatch base_match;
	std::string command;

	if ( std::regex_match( request, base_match, base_regex ) )
	{
		if ( base_match.size() >= MIN_NUM_ARG ) {
			for ( size_t i = 0; i < base_match.size(); i++ )
			{
				if ( base_match[i].matched )
				{
					base_sub_match = base_match[i];
					command = base_sub_match.str();
				}
			}
		}
		else
		{
			return false;
		}

		try
		{
			switch ( SelectedCommand( command ) )
			{
			case Help:
				HelpLogicTree( request, out );
				break;
			case List:
				ListLogicTree( request, out );
				break;
			case CheckOut:
				CheckOutLogicTree( request, out );
				break;
			case CheckIn:
				CheckInLogicTree( request, out );
				break;
			case Add:
				AddLogicTree( request, out );
				break;
			case Change:
				ChangeLogicTree( request, out );
				break;
			default:
				return false;
				break;
			}
		}
		catch ( const std::exception& error )
		{
			out << error.what() << std::endl;
			return false;
		}

		return true;
	}
	else
	{
		return false;
	}

	return false;
}


/// <summary> End of the help logic tree, prints out the smart search commands </summary>
/// <param name="request"> string containing the user's request </param>
/// <param name="out"> reference to an ostream buffer object </param>
/// <returns> void </returns>
void Library::HelpLogicTree( const std::string& request, std::ostream&  out )
{
	out << std::endl << "Smart Search Usage:" << std::endl << std::endl <<
		"Command: ***List, list, ls*** [optional]" << std::endl <<
		"\tlist [all] [overdue] book[s]" << std::endl <<
		"\tlist [all] [overdue] patron[s]" << std::endl <<
		"\tlist [all] (adult, child) book[s]" << std::endl <<
		"\tlist book[s] [checked out] [by] (patron's information)" << std::endl << std::endl <<
		"Command: ***Change, change***" << std::endl <<
		"\tchange patron (patron's information) (fees, first name, last name, adress) to (desired value)" << std::endl <<
		"\tchange fee amount to (desired value)" << std::endl << std::endl <<
		"Command: ***Checkout, checkout, check-out, out***" << std::endl <<
		"\tout (book's information) to (patron's information)" << std::endl << std::endl <<
		"Command: ***Checkin, checkin, check-in, in***" << std::endl <<
		"\tin (book's information)" << std::endl << std::endl <<
		"Command: ***Add, add*** [optional]" << std::endl <<
		"\tadd (dollar amount) to [patron] (patron's information) fees" << std::endl;
}


/// <summary> Begins the List logic tree, tries to complete a user's list request </summary>
/// <param name="request"> string containing the user's request </param>
/// <param name="out"> reference to an ostream buffer object </param>
/// <returns> void </returns>
void Library::ListLogicTree( const std::string& request, std::ostream&  out)
{
	std::vector<std::string> commandTree;
	std::regex base_regex( LIST_PATTERN );
	std::ssub_match base_sub_match;
	std::smatch base_match;

	if ( std::regex_match( request, base_match, base_regex ) )
	{
		if ( base_match.size() >= REQUIRED_ARG_NUM ) {
			for ( size_t i = REQUIRED_ARG_NUM; i < base_match.size(); i++ )
			{
				if ( base_match[i].matched )
				{
					base_sub_match = base_match[i];
					commandTree.push_back( base_sub_match.str() );
				}
			}

			if ( commandTree.size() < MIN_NUM_ARG ) { throw std::logic_error( "Not enough arguments for list" ); }

			switch ( SelectedParmOne( commandTree.front() ) )
			{
			case OverdueP1:
				if ( commandTree.size() < REQUIRED_ARG_NUM ) { throw std::logic_error( "Not enough arguments for \"list overdue <parm2>\"" ); }

				switch ( SelectedParmTwo( commandTree[1] ) )
				{
				case BooksP2:		// list all overdue books
					ListOverdueBooks( out );

					break;
				case PatronsP2:		// list all overdue patrons
					ListOverdueBooksByPatron( out );

					break;
				default:
					throw std::logic_error( "Bad argument passed for \"list overdue <parm2>\"" );
					break;
				}

				break;
			case ChildP1:
				if ( commandTree.size() < REQUIRED_ARG_NUM ) { throw std::logic_error( "Not enough arguments for \"list child <parm2>\"" ); }

				switch ( SelectedType(commandTree[1]) )
				{
				case BooksType:		// list all children books
					FindBook( "child", out );

					break;
				default:
					throw std::logic_error( "Bad argument passed for \"list [all] children <parm2>\"" );

					break;
				}

				break;
			case AdultP1:
				if ( commandTree.size() < REQUIRED_ARG_NUM ) { throw std::logic_error( "Not enough arguments for \"list adult <parm2>\"" ); }

				switch ( SelectedType( commandTree[1] ) )
				{
				case BooksType:		// list all children books
					FindBook( "adult", out );

					break;
				default:
					throw std::logic_error( "Bad argument passed for \"list [all] adult <parm2>\"" );

					break;
				}

				break;
			case VideoP1:			// list all videos
				FindBook( "video", out );

				break;
			case DVDP1:			// list all dvds
				FindBook( "dvd", out );

				break;
			case BooksP1:			// list all books
				if ( commandTree.size() == REQUIRED_ARG_NUM )
				{
					out << "All Media by Author: " << commandTree[1] << std::endl;
					FindBook( commandTree[1], out );
				}
				else
				{
					displayBooks( out );
				}

				break;
			case PatronsP1:
				displayPatrons( out );

				break;
			case AuthorP1:
				if ( commandTree.size() < REQUIRED_ARG_NUM ) { throw std::logic_error( "Not enough arguments for \"list author <parm2>\"" ); }
				FindBook( commandTree[2], out );

				break;
			default:
				throw std::logic_error( "Bad argument passed for \"list <parm1>\"" );
				break;
			}

		}
		else
		{
			throw std::logic_error( "Not enough arguments for list" );
		}
	}
	else
	{
		throw std::logic_error( "Bad argument passed for \"list <parm1>\"" );
	}
}


/// <summary> Begins the Check out logic tree, tries to complete a user's check out request </summary>
/// <param name="request"> string containing the user's request </param>
/// <param name="out"> reference to an ostream buffer object </param>
/// <returns> void </returns>
void Library::CheckOutLogicTree( const std::string& request, std::ostream& out )
{
	std::vector<std::string> commandTree;
	std::regex base_regex( CHECK_OUT_PATTERN );
	std::ssub_match base_sub_match;
	std::string patronResult = "-1";
	std::string bookResult = "";
	std::smatch base_match;
	std::ofstream nothing;

	if ( std::regex_match( request, base_match, base_regex ) )
	{
		if ( base_match.size() >= REQUIRED_ARG_NUM ) {
			for ( size_t i = REQUIRED_ARG_NUM; i < base_match.size(); i++ )
			{
				if ( base_match[i].matched )
				{
					base_sub_match = base_match[i];
					commandTree.push_back( base_sub_match.str() );
				}
			}

			if ( commandTree.size() < REQUIRED_ARG_NUM ) { throw std::logic_error( "Not enough arguments for out" ); }

			// search for the book
			bookResult = FindBook( commandTree[0], nothing );

			if ( bookResult == "-1" ) { throw std::logic_error( "Too many or too few media items found, please provide more specific information about the item" ); }

			patronResult = FindPatron( commandTree[1], nothing );

			if ( patronResult == "-1" ) { throw std::logic_error( "Too many or too few patrons found, please provide more specific information about the patron" ); }

			//try to check out the book
			checkout( patronResult, bookResult );

			out << std::endl << "Media \"" << commandTree[0] << "\" was successfully checked-out to " << commandTree[1] << std::endl <<
				"Press [ENTER] to return to main menu" << std::endl;
		}
		else
		{
			throw std::logic_error( "Not enough arguments for out" );
		}
	}
	else
	{
		throw std::logic_error( "Bad argument passed for \"out <parm1>\"" );
	}
}


/// <summary> Begins the Check in logic tree, tries to complete a user's check in request </summary>
/// <param name="request"> string containing the user's request </param>
/// <param name="out"> reference to an ostream buffer object </param>
/// <returns> void </returns>
void Library::CheckInLogicTree( const std::string& request, std::ostream& out )
{
	std::vector<std::string> commandTree;
	std::regex base_regex( CHECK_IN_PATTERN );
	std::ssub_match base_sub_match;
	std::string bookResult = "";
	std::smatch base_match;
	std::ofstream nothing;

	if ( std::regex_match( request, base_match, base_regex ) )
	{
		if ( base_match.size() >= REQUIRED_ARG_NUM ) {
			for ( size_t i = REQUIRED_ARG_NUM; i < base_match.size(); i++ )
			{
				if ( base_match[i].matched )
				{
					base_sub_match = base_match[i];
					commandTree.push_back( base_sub_match.str() );
				}
			}

			if ( commandTree.size() < MIN_NUM_ARG ) { throw std::logic_error( "Not enough arguments for in" ); }

			// search for the book
			bookResult = FindBook( commandTree[0], nothing );

			if ( bookResult == "-1" ) { throw std::logic_error( "Too many or too few media items found, please provide more specific information about the item" ); }

			//try to check in the book
			checkin( "", bookResult );

			out << std::endl << "Media \"" << commandTree[0] << "\" was successfully checked-in" << std::endl <<
				"Press [ENTER] to return to main menu" << std::endl;
		}
		else
		{
			throw std::logic_error( "Not enough arguments for in" );
		}
	}
	else
	{
		throw std::logic_error( "Bad argument passed for \"in <parm1>\"" );
	}
}


/// <summary> Begins the Add logic tree, tries to complete a user's add request </summary>
/// <param name="request"> string containing the user's request </param>
/// <param name="out"> reference to an ostream buffer object </param>
/// <returns> void </returns>
void Library::AddLogicTree( const std::string& request, std::ostream& out )
{
	std::vector<std::string> commandTree;
	std::regex base_regex( ADD_PATTERN );
	std::string patronResult = "-1";
	std::ssub_match base_sub_match;
	std::smatch base_match;
	std::ofstream nothing;
	double newFeeAmount;

	if ( std::regex_match( request, base_match, base_regex ) )
	{
		if ( base_match.size() >= REQUIRED_ARG_NUM ) {
			for ( size_t i = REQUIRED_ARG_NUM; i < base_match.size(); i++ )
			{
				if ( base_match[i].matched )
				{
					base_sub_match = base_match[i];
					commandTree.push_back( base_sub_match.str() );
				}
			}

			if ( commandTree.size() < REQUIRED_ARG_NUM ) { throw std::logic_error( "Not enough arguments for add" ); }

			// Find the patron
			patronResult = FindPatron( commandTree[1], nothing );

			if ( patronResult == "-1" ) { throw std::logic_error( "Too many or too few patrons found, please provide more specific information about the patron" ); }

			auto patronResults = boolinq::from( patrons )
				.where( [patronResult]( Patron* a ) { return std::to_string( a->GetID() ) == patronResult; } )
				.select( []( Patron* a ) { return a; } )
				.toVector();

			if ( patronResult == "-1" ) { throw std::logic_error( "Too many or too few patrons found, please provide more specific information about the patron" ); }

			newFeeAmount = patronResults.front()->GetFees() + stod( commandTree[0] );

			patronResults.front()->SetFees( std::to_string( newFeeAmount ) );

			out << std::endl << "$" << commandTree[0] << " was added to " << commandTree[1] << "'s fees" << std::endl <<
				"Press [ENTER] to return to main menu" << std::endl;
		}
		else
		{
			throw std::logic_error( "Not enough arguments for add" );
		}
	}
	else
	{
		throw std::logic_error( "Bad argument passed for \"add <parm1>\"" );
	}
}


/// <summary> Begins the Change logic tree, tries to complete a user's change request </summary>
/// <param name="request"> string containing the user's request </param>
/// <param name="out"> reference to an ostream buffer object </param>
/// <returns> void </returns>
void Library::ChangeLogicTree( const std::string& request, std::ostream& out )
{
	std::vector<std::string> commandTree;
	std::regex base_regex( CHANGE_PATTERN );
	std::string patronResult = "-1";
	std::ssub_match base_sub_match;
	std::vector<Patron*> patronPtr;
	std::smatch base_match;
	std::ofstream nothing;

	if ( std::regex_match( request, base_match, base_regex ) )
	{
		if ( base_match.size() >= REQUIRED_ARG_NUM ) {
			for ( size_t i = REQUIRED_ARG_NUM; i < base_match.size(); i++ )
			{
				if ( base_match[i].matched )
				{
					base_sub_match = base_match[i];
					commandTree.push_back( base_sub_match.str() );
				}
			}

			if ( commandTree.size() < MIN_NUM_ARG ) { throw std::logic_error( "Not enough arguments for change" ); }

			switch ( SelectedParmOne( commandTree.front() ) )
			{
			case FeesP1:			// change the amount of fees added for each overdue media
				if ( commandTree.size() < 4 ) { throw std::logic_error( "Not enough arguments for \"change fee amount <value>\"" ); }

				feeValue = stod( commandTree[3] );

				out << std::endl << "Changed the fee amount added for overdue media to $" << commandTree[3] << std::endl <<
					"Press [ENTER] to return to main menu" << std::endl;
				return;

				break;
			case PatronsP1:
				if ( commandTree.size() < 4 ) { throw std::logic_error( "Not enough arguments for \"change patron (patron info) (attribute to alter) to <value>\"" ); }

				// Find the patron
				patronResult = FindPatron( commandTree[1], nothing );

				if ( patronResult == "-1" ) { throw std::logic_error( "Too many or too few patrons found, please provide more specific information about the patron" ); }

				// Find the patron with the matching patronID
				patronPtr = boolinq::from( patrons )
					.where( [patronResult]( Patron* a ) { return std::to_string( a->GetID() ) == patronResult; } )
					.select( []( Patron* a ) { return a; } )
					.toVector();

				if ( patronPtr.size() != SINGLE_OBJECT_SELECTED ) { throw std::logic_error( "Too many or too few patrons found, please provide more specific information about the patron" ); }

				switch ( SelectedParmTwo( commandTree[2] ) )
				{
				case FeesP2:
					patronPtr.front()->SetFees( commandTree[3] );

					break;
				case FirstNameP2:
					patronPtr.front()->SetFirstName( commandTree[3] );

					break;
				case LastNameP2:
					patronPtr.front()->SetLastName( commandTree[3] );
					
					break;
				case AddressP2:
					patronPtr.front()->SetAddress( commandTree[3] );

					break;
				default:
					throw std::logic_error( "Bad argument passed for \"change" + commandTree[2] + " to <parm2>\"" );
					break;
				}

				break;
			default:
				throw std::logic_error( "Bad argument passed for \"change <parm1>\"" );
				break;
			}

			out << std::endl << "Changed patron " << commandTree[1] << "'s " << commandTree[2] << " to \"" << commandTree[3] << "\"" << std::endl <<
				"Press [ENTER] to return to main menu" << std::endl;
		}
		else
		{
			throw std::logic_error( "Not enough arguments for change" );
		}
	}
	else
	{
		throw std::logic_error( "Bad argument passed for \"change <parm1>\"" );
	}
}


/// <summary> Adds a single day to the current date stored in the Date class </summary>
/// <returns> void </returns>
void Library::AddDayToCurrent()
{
	Date::AddDayToCurrent();
}


/// <summary> Checks to see if the first parm contains the second parm </summary>
/// <param name="original"> string to check against </param>
/// <param name="strToFind"> string to find in the first parm </param>
/// <returns> bool </returns>
bool Library::contains( std::string original, std::string strToFind )
{
	size_t findIndex = 0;
	size_t index = 0;

	while ( strToFind[findIndex] == ' ' )
	{
		findIndex++;
	}

	while ( tolower( original[index] ) != tolower( strToFind[findIndex] ) && index < original.length() )
	{
		index++;
	}

	for ( size_t i = findIndex; i < strToFind.length(); i++ )
	{
		if ( index >= original.length() ) { return false; }

		if ( tolower( original[index] ) != tolower( strToFind[i] ) ) { return false; }

		index++;
	}

	return true;
}


/// <summary> Finds the command that was passed to the Smart Search function </summary>
/// <param name="request"> string containing the user's command </param>
/// <returns> Library::Command </returns>
Library::Command Library::SelectedCommand( const std::string& command )
{
	if ( command == "Help" || command == "help" ) { return Help; }
	if ( command == "List" || command == "list" || command == "ls" ) { return List; }
	if ( std::regex_match( command, std::regex( COMMAND_CHECKOUT_PATTERN ) ) ) { return CheckOut; }
	if ( std::regex_match( command, std::regex( COMMAND_CHECKIN_PATTERN ) ) ) { return CheckIn; }
	if ( command == "Add" || command == "add" ) { return Add; }
	if ( command == "Change" || command == "change" ) { return Change; }
	return Help;
}


/// <summary> Finds the parm passed after the command in the Smart Search </summary>
/// <param name="request"> string containing the first parm </param>
/// <returns> Library::Parm1 </returns>
Library::Parm1 Library::SelectedParmOne( const std::string& parm )
{
	if ( parm == "Overdue" || parm == "overdue" ) { return OverdueP1; }
	if ( std::regex_match( parm, std::regex( PARM_CHILD_PATTERN ) ) ) { return ChildP1; }
	if ( parm == "Adult" || parm == "adult" ) { return AdultP1; }
	if ( std::regex_match( parm, std::regex( PARM_VIDEO_PATTERN ) ) ) { return VideoP1; }
	if ( parm == "DVD" || parm == "dvd" ) { return DVDP1; }
	if ( std::regex_match( parm, std::regex( PARM_BOOK_PATTERN ) ) ) { return BooksP1; }
	if ( std::regex_match( parm, std::regex( PARM_PATRON_PATTERN ) ) ) { return PatronsP1; }
	if ( parm == "Author" || parm == "author" ) { return AuthorP1; }
	if ( std::regex_match( parm, std::regex( PARM_FEE_PATTERN ) ) ) { return FeesP1; }
	return ErrorP1;
}


/// <summary> Finds the parm passed second after the command in the Smart Search </summary>
/// <param name="request"> string containing the second parm </param>
/// <returns> Library::Parm2 </returns>
Library::Parm2 Library::SelectedParmTwo( const std::string& parm )
{
	if ( std::regex_match( parm, std::regex( PARM_BOOK_PATTERN ) ) ) { return BooksP2; }
	if ( std::regex_match( parm, std::regex( PARM_PATRON_PATTERN ) ) ) { return PatronsP2; }
	if ( parm == "By" || parm == "by" ) { return ByP2; }
	if ( std::regex_match( parm, std::regex( PARM_FEE_PATTERN ) ) ) { return FeesP2; }
	if ( std::regex_match( parm, std::regex( PARM_FIRST_NAME_PATTERN ) ) ) { return FirstNameP2; }
	if ( std::regex_match( parm, std::regex( PARM_LAST_NAME_PATTERN ) ) ) { return LastNameP2; }
	if ( parm == "Address" || parm == "address" ) { return AddressP2; }
	return ErrorP2;
}


/// <summary> Finds the media type requested </summary>
/// <param name="request"> string containing the type </param>
/// <returns> Library::Type </returns>
Library::Type Library::SelectedType( const std::string& type )
{
	if ( std::regex_match( type, std::regex( PARM_BOOK_PATTERN ) ) ) { return BooksType; }
	if ( std::regex_match( type, std::regex( PARM_VIDEO_PATTERN ) ) ) { return VideoType; }
	if ( type == "DVD" || type == "dvd" ) { return DVDType; }
	return ErrorType;
}


/// <summary> Checks to see if there were any import errors </summary>
/// <returns> bool </returns>
bool Library::errorsFound()
{
	return patronErrorList.size() + bookErrorList.size() + checkoutErrorList.size();
}


/// <summary> Displays any import errors </summary>
/// <param name="out"> reference to an ostream buffer object </param>
/// <returns> void </returns>
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


/// <summary> Displays all patrons </summary>
/// <param name="out"> reference to an ostream buffer object </param>
/// <returns> void </returns>
void Library::displayPatrons( std::ostream& out )
{
	out << std::endl << "Patrons:" << std::endl;

	for each ( Patron* patron in patrons )
	{
		patron->Display( out );
		//out << std::endl;
	}
}


/// <summary> Displays all media items </summary>
/// <param name="out"> reference to an ostream buffer object </param>
/// <returns> void </returns>
void Library::displayBooks( std::ostream& out )
{
	for each ( Book* book in books )
	{
		book->DisplayReduced( out );
		out << std::endl;
	}
}



#endif