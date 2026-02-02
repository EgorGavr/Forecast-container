#include "dialogProstoy.h"
#include <iostream>
#include <limits>
#include "dialogSlozhniy.h"


int main() {
    int choice = 0;

    while (true) {
        std::cout << "\n================ MENU ================" << std::endl;
        std::cout << "1. Test Prostoy Class" << std::endl;
        std::cout << "2. Test Slozhniy Class" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Choose your option: " << std::endl;

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (choice == 0) {
            std::cout << "Exit..." << std::endl;
            break;
        }
        else if (choice == 1) {
            testProstoy();
        }
        else if (choice == 2) {
            testSlozhniy();
        }
        else {
            std::cout << "Wrong choice!" << std::endl;
        }
    }

    return 0;
}