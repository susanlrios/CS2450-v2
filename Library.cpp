#ifndef LIBRARY_CPP
#define LIBRARY_CPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Library.h"

Library::Library() {
	try
	{
		for (size_t i = 0; i < 10; i++)
		{
			convert.str("");
			convert.clear();
			convert << i;
			patrons.push_back(new Patron("Chris", "Mansfield", "1234 Street Name_Orem_UT", "4/20/1993", "3/26/2016", convert.str()));
		}
	}
	catch (const std::exception& err)
	{
		std::cout << err.what() << std::endl;

		std::cin.clear();
		do {
			getline(std::cin, input);
		} while (input != "");
	}
}

Library::~Library() {
	for each (Patron* patron in patrons) {
		delete(patron);
	}

	for each (Book* book in books) {
		delete(book);
	}
}

void Library::createBooks(std::istream& in) {
	std::string str;
	while (std::getline(in, str)) {
		books.push_back(new Book());
		books.back()->setBookInfo(str);
	}
}

void Library::createPatrons(std::istream& in) {
	
}

void Library::restorePBStatus(std::istream& in) {

}

void Library::writeBooksToDB(std::ostream& out) {

}

void Library::writePatronsToDB(std::ostream& out) {

}

void Library::writePBStatusToDB(std::ostream& out) {

}

void Library::tryCheckout() {
	for (int i = 0; i < 2; i++) {
		if (books[i]->checkOut() == true) {
			patronBooks[patrons[i]].push_back(books[i]);
		}
		else {
			std::cout << "Cannot check out: " << books[i]->GetTitle() << std::endl;
		}
	}

	for (int q = 0; q < patronBooks.size(); q++) {
		std::cout << "Number of books checked out to " << patrons[q]->GetFirstName() << " " << patrons[q]->GetLastName() << " " << patrons[q]->GetID() << ": " << patronBooks[patrons[q]].size() << std::endl;
	}
}

#endif