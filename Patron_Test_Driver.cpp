

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "Patron.h"


int main(int argc, char* argv[])
{
	std::vector<Patron*> patrons;
	std::string fileName = "output.csv";
	std::ostringstream convert;
	std::string input;
	std::ofstream fileOut;
	std::ifstream fileIn;


	// used for testing file creation
	try
	{
		for ( size_t i = 0; i < 10; i++ )
		{
			convert.str( "" );
			convert.clear();
			convert << i;
			patrons.push_back( new Patron( "Chris", "Mansfield", "1234 Street Name_Orem_UT", "4/20/1993", "3/26/2016", convert.str() ) );
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

	// used for file input
	//try
	//{
	//	for ( size_t i = 0; i < 10; i++ )
	//	{
	//		patrons.push_back( new Patron() );
	//	}
	//}
	//catch ( const std::exception& err )
	//{
	//	std::cout << err.what() << std::endl;
	//}



	//fileOut.open( fileName );

	//if ( !fileOut.is_open() )
	//{
	//	std::cout << "Error: could not create file" << fileName << std::endl;
	//}
	//else
	//{
	//	fileOut << "First Name,Last Name,Address,Birth Date,Join Date,Outstanding Balance,ID" << std::endl;

	//	for each ( Patron* patron in patrons )
	//	{
	//		patron->Write( fileOut );
	//	}
	//}

	//fileOut.close();

	//fileIn.open( fileName );

	//if ( !fileIn.is_open() )
	//{
	//	std::cout << "Error: could not open file" << fileName << std::endl;
	//}
	//else
	//{
	//	getline( fileIn, input );

	//	for each ( Patron* patron in patrons )
	//	{
	//		patron->Read( fileIn );
	//	}
	//}
	//
	//fileIn.close();

	std::cout << patrons[0]->GetFirstName() << " " << patrons[0]->GetLastName() << std::endl <<
		patrons[0]->GetAddress() << std::endl <<
		"Patron ID: " << patrons[0]->GetID() << std::endl <<
		"Join Date: " << patrons[0]->GetJoinDate() << std::endl <<
		"Outstanding Fees Due: $" << patrons[0]->GetFees() << std::endl;

	std::cout << "\nPlease enter the current date: ";

	try
	{
		do {
			getline( std::cin, input );
		} while ( input == "" );

		patrons[0]->SetCurrentDate( input );
	}
	catch ( const std::exception& error )
	{
		std::cout << error.what();

		patrons[0]->SetCurrentDate( "" );
	}

	if ( patrons[0]->IsMinor() )
	{
		std::cout << "They are a Minor" << std::endl;
	}

	for each ( Patron* patron in patrons )
	{
		delete( patron );
	}

	std::cin.clear();
	do {
		getline( std::cin, input );
	} while ( input != "" );

	return 0;
}