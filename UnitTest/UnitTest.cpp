#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "..\MainFiles\Prostoy.h"
#include "..\MainFiles\Slozhniy.h"
#include <random>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>


/**
 * @brief Вспомогательный класс для тестирования
 */
class RandomGen {
private:
    /// @brief Какой то генератор рандомных чисел мощный.
    std::mt19937 rng;
public:
    /// @brief Метод для генератора.
    RandomGen() : rng(std::random_device{}()) {}


    /// @brief Генерирует случайное число из нужного диапазона для теста даты.
    long long getDate(long long min, long long max) {
        std::uniform_int_distribution<long long> dist(min, max);
        return dist(rng);
    }

    /// @brief Генерирует случайное число из нужного диапазона для теста температуры.
    double getDouble(double min, double max) { 
        std::uniform_real_distribution<double> dist(min, max);
        return dist(rng);
    }


    /// @brief Генерирует случайный тип погоды.
    std::string getStatus() { 
        static const std::vector<std::string> statuses = {"Sunny", "Cloudy", "Rain", "Snow"};
        std::uniform_int_distribution<size_t> dist(0, statuses.size() - 1);
        return statuses[dist(rng)];
    }

    /// @brief Генератор случайного прогноза погоды (Случайно генерирует все поля).
    ProstoyPrognoz getForecast() {
        // Даты от 2020 до 2030 года (в unix timestamp)
        long long date = getDate(1577836800, 1893456000);
        double tempMorning = getDouble(-30.0, 35.0);
        double tempDay = getDouble(-30.0, 35.0);
        double tempEvening = getDouble(-30.0, 35.0);
        double osadki = getDouble(0.0, 1500.0);
        std::string status = getStatus();
        return ProstoyPrognoz(date, tempMorning, tempDay, tempEvening, osadki, status);
    }
};




TEST_CASE("Stress Test sortirovki vectora", "[stress][sort]") {
    RandomGen gen;
    SlozhniyPrognoz vector;

    // Генерируем 1000 случайных прогнозов
    size_t N = 1000;
    for (size_t i = 0; i < N; i++) {
        vector += gen.getForecast();
    }

    REQUIRE(vector.size() == N);

    vector.sortDates();

    bool sorted = true;
    for (size_t i = 0; i < N - 1; i++) {
        if (vector[i].getDate() > vector[i + 1].getDate()) {
            sorted = false;
            INFO("Failed at index " << i);
            INFO("Date[" << i << "] = " << vector[i].getDate());
            INFO("Date[" << i + 1 << "] = " << vector[i + 1].getDate());
            break;
        }
    }
    REQUIRE(sorted == true);
}


TEST_CASE("Test about finding the coldest day", "[search]") {
    RandomGen gen;
    SlozhniyPrognoz vector;

    // Кладем рандомные данные
    for (int i = 0; i < 500; i++) {
        ProstoyPrognoz p = gen.getForecast();
        vector += p;
    }

    // Внедряем супер холодный день
    long long targetDate = 1700000000;
    ProstoyPrognoz target(targetDate, -100.0, -100.0, -100.0, 600.0, "Snow");
    vector += target;

    ProstoyPrognoz result = vector.getColdestDay(targetDate - 1000, targetDate + 1000);

    REQUIRE(result.getAverageTemp() == -100.0);
    REQUIRE(result.getDate() == targetDate);
}


TEST_CASE("Safety Test: Cheking Errors", "[errors][stress]") {
    RandomGen gen;
    SlozhniyPrognoz vector;

    // Генерируем заведомо ошибочные прогнозы
    int errorCount = 0;
    for (int i = 0; i < 1000; i++) {
        ProstoyPrognoz p = gen.getForecast();
        p.setDayTemp(25.0);
        p.setMorningTemp(25.0);
        p.setEveningTemp(25.0);
        p.setStatus("Sunny");
        p.setOsadki(0);

        bool error = false;

        // С вероятностью 50% портим температуру
        if (gen.getDate(0, 1) == 1) {
            p.setDayTemp(-200.0);
            error = true;
        }
        else {
            p.setDayTemp(20.0);
        }

        // С вероятностью 50% и подходящими условиями портим статус
        if (p.getMorningTemp() > 0.0 && p.getDayTemp() > 0.0 && p.getEveningTemp() > 0.0) {
            if (gen.getDate(0, 1) == 1) {
                p.setStatus("Snow");
                error = true;
            }
            else {
                p.setStatus(p.getOsadki() > 0 ? "Rain" : "Sunny");
            }
        }

        if (error) {
            errorCount++;
        }

        vector += p;
    }

    vector.removeOshibki();

    REQUIRE(vector.size() == (1000 - errorCount));
}


TEST_CASE("Benchmark: Merge Povtorki", "[performance][stress]") {
    RandomGen gen;
    SlozhniyPrognoz vector;

    // Создаем много дубликатов
    // Генерируем всего 10 уникальных дат
    for (int i = 0; i < 1000; i++) {
        ProstoyPrognoz p = gen.getForecast();
        p.setDate(1000 + (i % 10));
        vector += p;
    }

    REQUIRE(vector.size() == 1000);

    vector.mergePovtorki();

    // Типа их должно остаться ровно 10 штук
    REQUIRE(vector.size() == 10);
}


//Тесты для простого класса

TEST_CASE("Testing setters and operators, Prostoy", "[operators][setters]") {
    RandomGen gen;

    // Проверяем, что геттеры возвращают то, что мы записали
    SECTION("Setters/Getters work") {
        ProstoyPrognoz p;

        for (int i = 0; i < 1000; ++i) {
            long long rndDate = gen.getDate(-2000000000, 2000000000);
            double tempMorning = gen.getDouble(-100.0, 100.0);
            double tempDay = gen.getDouble(-100.0, 100.0);
            double tempEvening = gen.getDouble(-100.0, 100.0);
            double osadki = gen.getDouble(0.0, 1500.0);
            std::string rndStatus = gen.getStatus();
            double average = ((tempMorning + tempDay + tempEvening) / 3.0);

            p.setDate(rndDate);
            p.setMorningTemp(tempMorning);
            p.setDayTemp(tempDay);
            p.setEveningTemp(tempEvening);
            p.setOsadki(osadki);
            p.setStatus(rndStatus);

            REQUIRE(p.getDate() == rndDate);
            REQUIRE(p.getAverageTemp() == average);
            REQUIRE(p.getStatus() == rndStatus);
            REQUIRE(p.getOsadki() == osadki);
        }
    }

    // Тест сравнения
    SECTION("Comparison Operators Test") {
        for (int i = 0; i < 1000; ++i) {
            ProstoyPrognoz p1 = gen.getForecast();
            ProstoyPrognoz p2 = gen.getForecast();

            if (p1.getDate() < p2.getDate()) {
                REQUIRE(((p1 <=> p2) < 0));
                REQUIRE(((p2 <=> p1) > 0));
                REQUIRE_FALSE((p1 == p2));
            }
            else if (p1.getDate() > p2.getDate()) {
                REQUIRE(((p1 <=> p2) > 0));
                REQUIRE(((p2 <=> p1) < 0));
                REQUIRE_FALSE((p1 == p2));
            }
            else {
                REQUIRE((p1 == p2));
                REQUIRE_FALSE(((p1 <=> p2) != 0));
                REQUIRE_FALSE(((p1 <=> p2) != 0));
            }
        }
    }
}

TEST_CASE("Test of Errors cheking, Prostoy Class", "[errors]") {
    RandomGen gen;


    for (int i = 0; i < 1000; ++i) {
        ProstoyPrognoz p1 = gen.getForecast();

        p1.setDayTemp(25.0);
        p1.setMorningTemp(25.0);
        p1.setEveningTemp(25.0);


        p1.setStatus("Sunny");
        p1.setOsadki(0);


        if (gen.getDate(0, 2) == 1) {
            p1.setDayTemp(-200.0);         // Проверка граничных условий температуры
            REQUIRE(p1.oshibka() == true);
        }
        else {
            REQUIRE(p1.oshibka() == false);
        }




        p1.setDayTemp(-10.0);
        p1.setStatus("Sunny");
        p1.setOsadki(0);

        if (gen.getDate(0, 2) == 1) {
            p1.setDayTemp(-30.0);            // Проверка дождя при минусовой температуре
            p1.setStatus("Rain");
            REQUIRE(p1.oshibka() == true);
        }
        else {
            REQUIRE(p1.oshibka() == false);
        }






        p1.setDayTemp(25.0);
        p1.setOsadki(300.0);
        p1.setStatus("Rain");

        if (gen.getDate(0, 2) == 1) {
            p1.setOsadki(300.0);                // Проверка солнца при больших осадках
            p1.setStatus("Sunny");
            REQUIRE(p1.oshibka() == true);
        }
        else {
            REQUIRE(p1.oshibka() == false);
        }

    }
}

TEST_CASE("Test of += operator, Prostoy Class", "[operators]") {
    
    RandomGen gen;

    for (int i = 0; i < 1000; ++i) {
        ProstoyPrognoz p1 = gen.getForecast();
        ProstoyPrognoz p2 = gen.getForecast();

        p1.setDate(100);
        p2.setDate(100);

        double averageMor = (p1.getMorningTemp() + p2.getMorningTemp()) / 2.0;
        double averageDay = (p1.getDayTemp() + p2.getDayTemp()) / 2.0;
        double averageEven = (p1.getEveningTemp() + p2.getEveningTemp()) / 2.0;
        double averageOsadki = (p1.getOsadki() + p2.getOsadki()) / 2.0;
        p2.setStatus("Snow");

        p1 += p2;

        REQUIRE(p1.getAverageTemp() == (averageMor + averageDay + averageEven) / 3.0);

        REQUIRE(p1.getDate() == 100);

        REQUIRE(p1.getStatus() == "Snow");
    }
}



TEST_CASE("Test of constructors", "[constructors]") {
    RandomGen gen;

    for (int i = 0; i < 1000; ++i) {
        SlozhniyPrognoz vector;
        ProstoyPrognoz p1 = gen.getForecast();
        std::string status = p1.getStatus();
        ProstoyPrognoz p2 = gen.getForecast();

        vector += p1;
        vector += p2;

        SECTION("Copy Constructor") {
            SlozhniyPrognoz copy(vector);
            REQUIRE(copy.size() == vector.size());
            REQUIRE(copy[0].getStatus() == status);
        }

        SECTION("Copy operator =") {
            SlozhniyPrognoz copy;
            copy = vector;
            copy = copy;
            REQUIRE(copy.size() == vector.size());
            REQUIRE(copy[0].getStatus() == vector[0].getStatus());
        }

        SECTION("Move Constructor") {
            SlozhniyPrognoz source;
            ProstoyPrognoz p3 = gen.getForecast();
            source += p3;
            SlozhniyPrognoz target(std::move(source));
            REQUIRE(target.size() == 1);
            REQUIRE(source.size() == 0);
        }

        SECTION("Move operator =") {
            SlozhniyPrognoz source;
            ProstoyPrognoz p4 = gen.getForecast();
            source += p4;
            SlozhniyPrognoz target;
            target = std::move(source);
            REQUIRE(target.size() == 1);
            REQUIRE(source.size() == 0);
        }
    }
}


TEST_CASE("Input / Output Operators", "[operators]") {

    RandomGen gen;
    for (int i = 0; i < 1000; ++i) {

        SlozhniyPrognoz vector;
        std::stringstream input;
        ProstoyPrognoz p1 = gen.getForecast();

        long long date = p1.getDate();
        double tempMorning = p1.getMorningTemp();
        double tempDay = p1.getDayTemp();
        double tempEvening = p1.getEveningTemp();
        double osadki = p1.getOsadki();
        std::string status = p1.getStatus();

        input << "1\n";
        input << date << " " << tempMorning << " " << tempDay << " " << tempEvening << " " << status << " " << osadki << "\n";

        input >> vector;

        REQUIRE(vector.size() == 1);
        REQUIRE(vector[0].getStatus() == status);

        std::stringstream output;
        output << vector;

        REQUIRE(output.str().length() > 0);
        REQUIRE(output.str().find(status) != std::string::npos);
    }
}

