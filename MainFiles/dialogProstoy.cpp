#include "dialogProstoy.h"
#include <iostream>
#include <limits>
#include "Prostoy.h"

void testProstoy() {
    ProstoyPrognoz prognoz1, prognoz2;

    std::cout << "\n--- Test of simple class ---" << std::endl;
    std::cout << "Input format: date(unix time stamp) MorningTemp DayTemp EveningTemp Status(Sunny/Cloudy/Rain/Snow) Osadki" << std::endl;

    // Тут вводим первый прогноз
    while (true) {
        std::cout << "\nInput data for Prognoz #1:" << std::endl;
        if (std::cin >> prognoz1) {
            break;
        }
        else {
            std::cout << "Input Error!\n" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    // Ввод второго прогноза
    while (true) {
        std::cout << "\nInput data for Prognoz #2:" << std::endl;
        if (std::cin >> prognoz2) {
            break;
        }
        else {
            std::cout << "Input Error!\n" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::cout << "\n-----------------------------------" << std::endl;
    std::cout << "Prognozi:" << std::endl;
    std::cout << "1: " << prognoz1 << std::endl;
    std::cout << "2: " << prognoz2 << std::endl;

    // Чекаем ошибки
    std::cout << "\n[Checking for mistakes]" << std::endl;
    if (prognoz1.oshibka()) std::cout << "-> Prognoz #1 is wrong" << std::endl;
    else std::cout << "-> Prognoz #1 is good" << std::endl;

    if (prognoz2.oshibka()) std::cout << "-> Prognoz #2 is wrong" << std::endl;
    else std::cout << "-> Prognoz #2 is good" << std::endl;

    // Сравниваем
    std::cout << "\n[Comparing]" << std::endl;
    if (prognoz1 == prognoz2) {
        std::cout << "-> Prognoz #1 and prognoz #2 have same dates" << std::endl;
    }
    else {
        auto result = prognoz1 <=> prognoz2;
        if (result < 0) {
            std::cout << "-> Prognoz #1 is earlier than prognoz #2" << std::endl;
        }
        else if (result > 0) {
            std::cout << "-> Prognoz #1 is later than prognoz #2" << std::endl;
        }
    }

    // Объединяем
    std::cout << "\n[Merging prognoz #1 and prognoz #2 (prognoz1 += prognoz2)]" << std::endl;
    try {
        prognoz1 += prognoz2;
        std::cout << "Result of merging:\n" << prognoz1 << std::endl;
    }
    catch (const std::invalid_argument& ex) {
        std::cout << "Meging error: " << ex.what() << std::endl;
        std::cout << "(You cant merge objects with different dates)" << std::endl;
    }
}