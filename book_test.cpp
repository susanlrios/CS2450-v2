#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "Patron.h"
#include "Book.h"

int main(int argc, char* argv[])
{
	std::vector<Patron*> patrons;
	std::vector<Book*> books;
	std::string fileName = "output.csv";
	std::ostringstream convert;
	std::string input;
	std::ofstream fileOut;
	std::ifstream fileIn;


	// used for testing file creation
	try
	{
		for (size_t i = 0; i < 2; i++)
		{
			convert.str("");
			convert.clear();
			convert << i;
			patrons.push_back(new Patron("Chris", "Mansfield", "1234 Street Name_Orem_UT", "3/10/1995", "3/26/2016", convert.str()));
		}
	}
	catch (const std::exception& err)
	{
		std::cout << err.what() << std::endl;
	}

	books.push_back(new Book("Harry Potter", "J.K. Rowling", "Adult", 982374233));
	books.push_back(new Book("How to make great YouTube videos", "Bob Smith", "Video", 293489243));

	books[0]->checkOut(patrons[0]);
	books[0]->checkOut(patrons[1]);
	books[1]->checkOut(patrons[1]);
	books[0]->printInfo();
	books[1]->printInfo();
	books[0]->checkIn();
	books[1]->checkIn();
	books[0]->printInfo();
	books[1]->printInfo();

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

	/*fileOut.open(fileName);

	if (!fileOut.is_open())
	{
		std::cout << "Error: could not create file" << fileName << std::endl;
	}
	else
	{
		fileOut << "First Name,Last Name,Address,Birth Date,Join Date,Outstanding Balance,ID" << std::endl;

		for each (Patron* patron in patrons)
		{
			patron->Write(fileOut);
		}
	}

	fileOut.close();

	fileIn.open(fileName);

	if (!fileIn.is_open())
	{
		std::cout << "Error: could not open file" << fileName << std::endl;
	}
	else
	{
		getline(fileIn, input);

		for each (Patron* patron in patrons)
		{
			patron->Read(fileIn);
		}
	}

	fileIn.close();*/

	/*if (patrons[0]->IsMinor())
	{
		std::cout << "They are a Minor" << std::endl;
	}*/

	for each (Book* book in books)
	{
		delete(book);
	}

	for each (Patron* patron in patrons)
	{
		delete(patron);
	}

	std::cin.clear();
	do {
		getline(std::cin, input);
	} while (input != "");

	return 0;
}