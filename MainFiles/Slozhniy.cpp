#include "Slozhniy.h"
#include <utility>
#include <iostream>
#include <algorithm>
#include <ctime>

void SlozhniyPrognoz::reserve(size_t newCapacity) {
    if (newCapacity <= capacity) return;

    ProstoyPrognoz* newPrognozi = new ProstoyPrognoz[newCapacity];

    for (size_t i = 0; i < count; i++) {
        newPrognozi[i] = std::move(prognozi[i]);
    }

    delete[] prognozi;

    prognozi = newPrognozi;
    capacity = newCapacity;
}

SlozhniyPrognoz::SlozhniyPrognoz():
    prognozi(nullptr),
    count(0),
    capacity(0),
    sortedStatus(true) {
}

SlozhniyPrognoz::SlozhniyPrognoz(const ProstoyPrognoz* arr, size_t size):
    count(size), capacity(size), sortedStatus(false) {

    if (size > 0) {
        prognozi = new ProstoyPrognoz[size];
        std::copy_n(arr, size, prognozi);
    }
    else {
        prognozi = nullptr;
        sortedStatus = true;
    }
}

SlozhniyPrognoz::SlozhniyPrognoz(const ProstoyPrognoz& prognoz):
    count(1), capacity(1), sortedStatus(true) {
    prognozi = new ProstoyPrognoz[1];
    prognozi[0] = prognoz;
}

SlozhniyPrognoz::SlozhniyPrognoz(const SlozhniyPrognoz& other):
    count(other.count), capacity(other.capacity), sortedStatus(other.sortedStatus) {

    if (capacity > 0) {
        prognozi = new ProstoyPrognoz[capacity];
        std::copy_n(other.prognozi, count, prognozi);
    }
    else {
        prognozi = nullptr;
    }
}

SlozhniyPrognoz::SlozhniyPrognoz(SlozhniyPrognoz&& other) noexcept:
    prognozi(other.prognozi), count(other.count), capacity(other.capacity), sortedStatus(other.sortedStatus) {

    other.prognozi = nullptr;
    other.count = 0;
    other.capacity = 0;
    other.sortedStatus = true;
}


SlozhniyPrognoz::~SlozhniyPrognoz() {
    delete[] prognozi;
}

SlozhniyPrognoz& SlozhniyPrognoz::operator += (const ProstoyPrognoz& newPrognoz) {
    if (count == capacity) {
        reserve(capacity == 0 ? 1 : capacity * 2);
    }
    prognozi[count] = newPrognoz;
    count++;

    if (count > 1) {
        sortedStatus = false;
    }

    return *this;
}

ProstoyPrognoz& SlozhniyPrognoz::operator [] (size_t index) {
    if (index >= count) throw std::out_of_range("Index out of range");
    sortedStatus = false;
    return prognozi[index];
}

const ProstoyPrognoz& SlozhniyPrognoz::operator [] (size_t index) const {
    if (index >= count) throw std::out_of_range("Index out of range");
    return prognozi[index];
}

SlozhniyPrognoz& SlozhniyPrognoz::operator = (const SlozhniyPrognoz& other) {
    if (this == &other) return *this; 

    delete[] prognozi;

    count = other.count;
    capacity = other.capacity;

    if (capacity > 0) {
        prognozi = new ProstoyPrognoz[capacity];
        std::copy_n(other.prognozi, count, prognozi);
        
    }
    else {
        prognozi = nullptr;
    }

    return *this;
}

SlozhniyPrognoz& SlozhniyPrognoz::operator = (SlozhniyPrognoz&& other) noexcept {
    if (this == &other) return *this;

    delete[] prognozi;

    prognozi = other.prognozi;
    count = other.count;
    capacity = other.capacity;

    other.prognozi = nullptr;
    other.count = 0;
    other.capacity = 0;

    return *this;
}


std::ostream& operator << (std::ostream& output, const SlozhniyPrognoz& vivod) {
    output << "Forecast Vector (Size: " << vivod.count << "):" << std::endl;
    for (size_t i = 0; i < vivod.count; i++) {
        output << "[" << i << "] " << vivod.prognozi[i] << std::endl;
    }
    return output;
}


std::istream& operator >> (std::istream& input, SlozhniyPrognoz& vvod) {
    size_t kolvo;
    std::cout << "Input format: date(unix time stamp) MorningTemp DayTemp EveningTemp Status(Sunny/Cloudy/Rain/Snow) Osadki" << std::endl;
    std::cout << "Enter number of forecasts to add:" << std::endl;
    if (!(input >> kolvo)) {
        return input;
    }

    SlozhniyPrognoz temp;

    for (size_t i = 0; i < kolvo; i++) {
        ProstoyPrognoz elem;
        std::cout << "Prognoz #" << (i + 1) << ":" << std::endl;
        input >> elem;
        if (input.fail()) {
            return input;
        }
        temp += elem;
    }

    for (size_t i = 0; i < temp.size(); i++) {
        vvod += temp[i];
    }
    return input;
}


void SlozhniyPrognoz::remove(size_t index) {
    if (index >= count) throw std::out_of_range("Index out of range");

    for (size_t i = index; i < count - 1; i++) {
        prognozi[i] = std::move(prognozi[i + 1]);
    }
    count--;
}

ProstoyPrognoz SlozhniyPrognoz::getColdestDay(long long dateStart, long long dateEnd) const {
    if (count == 0) throw std::logic_error("Class is empty");

    ProstoyPrognoz coldest;
    bool found = false;
    double minimum = 0.0;

    for (size_t i = 0; i < count; i++) {
        long long date = prognozi[i].getDate();

        if (date >= dateStart && date <= dateEnd) {
            double average = prognozi[i].getAverageTemp();

            if (!found || (average < minimum)) {
                minimum = average;
                coldest = prognozi[i];
                found = true;
            }
        }
    }

    if (!found) {
        throw std::logic_error("No forecasts found in your date range");
    }

    return coldest;
}

ProstoyPrognoz SlozhniyPrognoz::getNextSunnyDay(long long currentDate) const {
    ProstoyPrognoz next;
    bool found = false;

    for (size_t i = 0; i < count; i++) {
        if (prognozi[i].getDate() >= currentDate && prognozi[i].getStatus() == "Sunny") {
            if (!found || ((prognozi[i] <=> next) < 0)) {
                next = prognozi[i];
                found = true;
            }
        }
    }

    if (!found) throw std::logic_error("No sunny days found");
    return next;
}


void SlozhniyPrognoz::removeOshibki() {

    if (count == 0) return;

    for (size_t i = count; i > 0; i--) {
        size_t index = i - 1;  //Типо size_t беззнаковый и сравнивать с 0 ну такооооое
        if (prognozi[index].oshibka()) {
            remove(index);
        }
    }
}


void SlozhniyPrognoz::sortDates() {

    if (sortedStatus) return;

    if (count < 2) return;

    for (size_t i = 0; i < count - 1; i++) {
        for (size_t j = 0; j < count - i - 1; j++) {
            if (prognozi[j] <=> prognozi[j + 1] > 0) {   // Можно и через swap, без разницы
                ProstoyPrognoz temp = std::move(prognozi[j]);
                prognozi[j] = std::move(prognozi[j + 1]);
                prognozi[j + 1] = std::move(temp);
            }
        }
    }

    sortedStatus = true;
}

void SlozhniyPrognoz::mergePovtorki() {
    if (count < 2) return;

    sortDates();

    size_t i = 0;
    while (i < count - 1) {
        if (prognozi[i] == prognozi[i + 1]) {
            prognozi[i] += prognozi[i + 1];

            remove(i + 1);
        }
        else {
            i++;
        }
    }
}

SlozhniyPrognoz SlozhniyPrognoz::getMonth(long long date) const {
    SlozhniyPrognoz podmnozh;

    time_t rawtime = (time_t)date;
    struct tm start;
    localtime_s(&start, &rawtime);

    start.tm_mday = 1;
    start.tm_hour = 0;
    start.tm_min = 0;
    start.tm_sec = 0;

    long long startMonth = (long long)mktime(&start);

    start.tm_mon += 1;
    long long endMonth = (long long)mktime(&start);


    for (size_t i = 0; i < count; i++) {
        long long datePrognoz = prognozi[i].getDate();

        if (datePrognoz >= startMonth && datePrognoz < endMonth) {
            podmnozh += prognozi[i];
        }
    }

    podmnozh.sortDates();
    return podmnozh;
}