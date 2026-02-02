#include "dialogSlozhniy.h"
#include "Slozhniy.h"
#include <iostream>
#include <limits>

template <typename T>
T getNum(const char* message) {
    T value;
    while (true) {
        std::cout << message << std::endl;
        if (std::cin >> value) {
            return value;
        }
        std::cout << "Error! Please enter a number." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}


void testSlozhniy() {
    SlozhniyPrognoz vector;
    int choice = 0;

    while (true) {
        std::cout << "\n--- TEST SLOZHNIY CLASS ---" << std::endl;
        std::cout << "1. Add prognoz (operator >>)" << std::endl;
        std::cout << "2. Print all forecasts (operator <<)" << std::endl;
        std::cout << "3. Find the coldest day in your range" << std::endl;
        std::cout << "4. Find next sunny day" << std::endl;
        std::cout << "5. Remove errors (removeOshibki)" << std::endl;
        std::cout << "6. Merge duplicates (mergePovtorki)" << std::endl;
        std::cout << "7. Get vector of any month" << std::endl;
        std::cout << "8. Sort vector by dates" << std::endl;
        std::cout << "0. Back to Main Menu" << std::endl;

        choice = getNum<int>("Your choice: ");

        try {
            if (choice == 0) {
                return;
            }

            if (choice == 1) {
                std::cin >> vector;
                continue;
            }

            if (choice == 2) {
                std::cout << vector << std::endl;
                continue;
            }

            if (choice == 3) {
                long long start = getNum<long long>("\nEnter start date (unix timestamp): ");
                long long end = getNum<long long>("\nEnter end date (unix timestamp): ");

                ProstoyPrognoz cold = vector.getColdestDay(start, end);
                std::cout << "\nColdest day found:\n" << cold << std::endl;
                continue;
            }

            if (choice == 4) {
                long long current = getNum<long long>("Enter current date (unix timestamp): ");

                ProstoyPrognoz sunny = vector.getNextSunnyDay(current);
                std::cout << "\nNext sunny day is:\n" << sunny << std::endl;
                continue;
            }

            if (choice == 5) {
                std::cout << "Removing erroneous forecasts..." << std::endl;
                vector.removeOshibki();
                std::cout << "Done. Current size: " << vector.size() << std::endl;
                continue;
            }

            if (choice == 6) {
                std::cout << "Merging duplicates..." << std::endl;
                vector.mergePovtorki();
                std::cout << "Done. (Vector sorted and merged)" << std::endl;
                continue;
            }

            if (choice == 7) {
                long long date = getNum<long long>("Enter any date of selected month: ");

                SlozhniyPrognoz month = vector.getMonth(date);

                std::cout << "\n--- Selected Month Result ---" << std::endl;
                if (month.size() == 0) {
                    std::cout << "No forecasts found for this month." << std::endl;
                }
                else {
                    std::cout << month << std::endl;
                }
                continue;
            }

            if (choice == 8) {
                vector.sortDates();
                std::cout << "Vector sorted by date." << std::endl;
                continue;
            }

            else {
                std::cout << "Unknown command." << std::endl;
            }
        }

        catch (const std::exception& ex) {
            std::cout << "\nError: " << ex.what() << std::endl;
        }

    }
}