
#include <iostream>

int main(int argc, const char * argv[]) {

    std::string choice;

    do {
        std::cout << "What would you like to do?";
        std::cin >> choice;
        std::cout << "1-Check in book" << std::endl;
        std::cout << "2-Check out book" << std::endl;
        std::cout << "3-List all books" << std::endl;
        std::cout << "4-List all overdue books" << std::endl;
        std::cout << "5-List all books checked out by a patron" << std::endl;
        std::cout << "6-Advance Time" << std::endl;
        std::cout << "7-Exit" << std::endl;
    } while (choice != "7");
    return 0;
}
