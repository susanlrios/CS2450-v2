// Library v1.1

#ifndef LIBRARY_H
#define LIBRARY_H


#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

#include "Date.h"
#include "Book.h"
#include "Patron.h"
#include "Boolinq.h"

typedef std::pair<Patron*, std::vector<Book*>> patronStatus;


class Library
{
private:
	enum Command { Help = 1, List = 2, CheckOut = 3, CheckIn = 4, Add = 5, Change = 6 };
	enum Parm1 { ErrorP1 = 0, OverdueP1 = 1, ChildP1 = 2, AdultP1 = 3, VideoP1 = 4, DVDP1 = 5, BooksP1 = 6, PatronsP1 = 7, AuthorP1 = 8, MediaP1 = 9, FeesP1 = 10 };
	enum Parm2 { ErrorP2 = 0, BooksP2 = 1, PatronsP2 = 2, ByP2 = 3, FeesP2 = 4, FirstNameP2 = 5, LastNameP2 = 6, AddressP2 = 7 };
	enum Type { ErrorType = 0, BooksType = 1, VideoType = 2, DVDType = 3 };
	//	const std::string CHECK_OUT_PATTERN = R"(^([Oo]ut|[cC]heck[ -]?out) ?(?:.*)(?:[Bb]ooks?|[Dd][Vv][Dd]s?|[Vv]ideo(?:tape)?|[Mm]edia)? (.*) (?:to) (.*)$)";
	// 	const std::string CHECK_IN_PATTERN = R"(^([Ii]n|[cC]heck[ -]?in) ([Bb]ooks?|[Dd][Vv][Dd]s?|[Vv]ideo(?:tape)?|[Mm]edia) (.*)$)";
	const std::string COMMAND_PATTERN = R"(^(([Hh]elp)|([Ll]ist|ls)|([Oo]ut|[Cc]heck[ -]?out)|([Ii]n|[Cc]heck[ -]?in)|([Aa]dd)|([Cc]hange)).*$)";
	const std::string LIST_PATTERN = R"(^([Ll]ist|ls).*([Oo]verdue)? ?([Cc]hild(?:ren)?|[Aa]dult)? ([Bb]ooks?|[Dd][Vv][Dd]s?|[Vv]ideo(?:tape)?|[Pp]atrons?) ?(?:by)?(.*)$)";
	const std::string ADD_PATTERN = R"(^([Aa]dd) \$?([0-9]{1,8}|[0-9]{1,8}\.[0-9]{1,2}) (?:to) ?(?:[Pp]atrons?)? (.*) (?:[Ff]ees?))";
	const std::string CHANGE_PATRON_PATTERN = R"(^([Cc]hange) ([Pp]atron) (.*) ([Ff]ees|([Ff]irst|[Ll]ast)(?: ?[Nn]ame)?|[Aa]ddress) ?(?:to)? (.*)$)";
	const std::string CHANGE_PATTERN = R"(^([Cc]hange) ([Ff]ee|[Pp]atron) ?(.*) ([Aa]mount|[Ff]irst ?name|[Ll]ast ?name|[Ff]ees|[Aa]ddress|) (?:to) \$?(.*)$)";
	const std::string CHECK_OUT_PATTERN = R"(^([Oo]ut|[cC]heck[ -]?out) (.*) (?:to) (.*)$)";
	const std::string COMMAND_CHECKOUT_PATTERN = R"(^([Oo]ut|[Cc]heck[ -]?out)$)";
	const std::string PARM_VIDEO_PATTERN = R"([Dd][Vv][Dd]s?|[Vv]ideo(?:tape)?)";
	const std::string COMMAND_CHECKIN_PATTERN = R"(^([Ii]n|[Cc]heck[ -]?in)$)";
	const std::string CHECK_IN_PATTERN = R"(^([Ii]n|[cC]heck[ -]?in) (.*)$)";
	const std::string PARM_FIRST_NAME_PATTERN = R"([Ff]irst ?name)";
	const std::string PARM_LAST_NAME_PATTERN = R"([Ll]ast ?name)";
	const std::string PARM_CHILD_PATTERN = R"([Cc]hild(?:ren)?)";
	const std::string PARM_PATRON_PATTERN = R"([Pp]atrons?)";
	const std::string PARM_BOOK_PATTERN = R"([Bb]ooks?)";
	const std::string PARM_FEE_PATTERN = R"([Ff]ees?)";
	const double FEE_INCREMENT_AMOUNT = 5.00;
	const size_t SINGLE_OBJECT_SELECTED = 1;
	const size_t MAX_ADULT_CHECKOUT = 6;
	const size_t MAX_CHILD_CHECKOUT = 3;
	const size_t REQUIRED_ARG_NUM = 2;
	const size_t MIN_NUM_ARG = 1;


	std::unordered_map<Patron*, std::vector<Book*>> patronBooks;
	std::vector<std::string> checkoutErrorList;
	std::vector<std::string> patronErrorList;
	std::vector<std::string> bookErrorList;
	std::vector<Patron*> patrons;
	std::vector<Book*> books;
	double feeValue;

	std::vector<std::string> Split( char*, char );
	static bool contains( std::string, std::string );
	Command SelectedCommand( const std::string& );
	Parm1 SelectedParmOne( const std::string& );
	Parm2 SelectedParmTwo( const std::string& );
	Type SelectedType( const std::string& );
	void HelpLogicTree( const std::string&, std::ostream& );
	void ListLogicTree( const std::string&, std::ostream& );
	void CheckOutLogicTree( const std::string&, std::ostream& );
	void CheckInLogicTree( const std::string&, std::ostream& );
	void AddLogicTree( const std::string&, std::ostream& );
	void ChangeLogicTree( const std::string&, std::ostream& );

public:
	Library();
	~Library();
	void setCurrentDate( std::string );
	void createBooks( std::istream& );
	void createPatrons( std::istream& );
	void restorePBStatus( std::istream& );
	void checkout( std::string, std::string );
	void checkin( std::string, std::string );
	void writeBooksToDB( std::ostream& );
	void writePatronsToDB( std::ostream& );
	void writePBStatusToDB( std::ostream& );
	std::string FindPatron( std::string, std::ostream& );
	std::string FindBook( std::string, std::ostream& );
	void ListBooksByPatron( std::string, std::ostream& );
	void ListOverdueBooks( std::ostream& );
	void ListOverdueBooksByPatron( std::ostream& );
	void ListCheckedOut( std::ostream& );
	const std::string& GetCurrentDate();
	bool SmartSearch( const std::string&, std::ostream& );
	void AddDayToCurrent();
	bool errorsFound();
	void displayErrors( std::ostream& );
	void displayPatrons( std::ostream& );
	void displayBooks( std::ostream& );
};



#endif