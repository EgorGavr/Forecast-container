#include "Prostoy.h"
#include <stdexcept>
#include <iostream>

ProstoyPrognoz::ProstoyPrognoz():      //Пустой конструктор
    date(0), tempMorning(0.0), tempDay(0.0), tempEvening(0.0), osadki(0.0), status("Sunny") {
}


ProstoyPrognoz::ProstoyPrognoz(long long date, double tempMorning, double tempDay, double tempEvening, double osadki, std::string status):
    //Инициализирующий конструктор (Все включено)
    date(date),
    tempMorning(tempMorning),
    tempDay(tempDay),
    tempEvening(tempEvening),
    osadki(osadki),
    status(status) {
}

ProstoyPrognoz::ProstoyPrognoz(long long date, double tempMorning, double tempDay, double tempEvening, double osadki):
    //Инициализирующий конструктор (Автовычисление состояния)
    date(date),
    tempMorning(tempMorning),
    tempDay(tempDay),
    tempEvening(tempEvening),
    osadki(osadki),
    status("Sunny") {

    status = findStatus();
}

//Геттеры определил сразу

//Сеттеры
void ProstoyPrognoz::setDate(long long date) {
    this->date = date;
}
void ProstoyPrognoz::setMorningTemp(double t) {
    if (t < -273) throw std::out_of_range("Temp must be > -273");
    tempMorning = t;
}
void ProstoyPrognoz::setDayTemp(double t) {
    if (t < -273) throw std::out_of_range("Temp must be > -273");
    tempDay = t;
}
void ProstoyPrognoz::setEveningTemp(double t) {
    if (t < -273) throw std::out_of_range("Temp must be > -273");
    tempEvening = t;
}
void ProstoyPrognoz::setOsadki(double mm) {
    osadki = mm;
}
void ProstoyPrognoz::setStatus(std::string p) {
    static const std::string perechen[4] = { "Sunny", "Cloudy", "Rain", "Snow" };
    for (int i = 0; i < 4; i++) {
        if (p == perechen[i]) {
            status = p;
            return;
        }
    }
    throw std::out_of_range("Incorrect Status");
}

//Методы (остальные)
double ProstoyPrognoz::getAverageTemp() const {
    return (tempMorning + tempDay + tempEvening) / 3.0;
}

bool ProstoyPrognoz::oshibka() const {
    if ((status == "Sunny" || status == "Cloudy") && osadki != 0.0) {
        return true;
    }

    if ((status == "Rain" || status == "Snow") && osadki == 0.0) {
        return true;
    }

    if (status == "Snow") {
        if (tempMorning > 0.0 && tempDay > 0.0 && tempEvening > 0.0) {
            return true;
        }
    }

    if (status == "Rain") {
        if (tempMorning < 0.0 || tempDay < 0.0 || tempEvening < 0.0) {
            return true;
        }
    }

    if (tempMorning < -100.0 || tempMorning > 60.0) {
        return true;
    }
    if (tempDay < -100.0 || tempDay > 60.0) {
        return true;
    }
    if (tempEvening < -100.0 || tempEvening > 60.0) {
        return true;
    }

    if (osadki > 1500.0) {
        return true;
    }

    return false;
}

//Перегрузки операторов
ProstoyPrognoz& ProstoyPrognoz::operator += (const ProstoyPrognoz& r) {
    if (date != r.date) {
        throw std::invalid_argument("The date is different!");
    }

    tempMorning = (tempMorning + r.tempMorning) / 2.0;
    tempDay = (tempDay + r.tempDay) / 2.0;
    tempEvening = (tempEvening + r.tempEvening) / 2.0;
    osadki = (osadki + r.osadki) / 2.0;
    status = worstStatus(status, r.status);
    
    return *this;
}


std::strong_ordering ProstoyPrognoz::operator <=> (const ProstoyPrognoz& r) const {

    if (date < r.date) {
        return std::strong_ordering::less;
    }
    if (date > r.date) {
        return std::strong_ordering::greater;
    }

    return std::strong_ordering::equal;
}


//Дружественные и свободные функции + Прегрузка ввода/вывода

bool operator == (const ProstoyPrognoz& lhs, const ProstoyPrognoz& rhs) {
    return lhs.date == rhs.date;
}

std::ostream& operator << (std::ostream& output, const ProstoyPrognoz& vivod) {
    output << "Date: " << vivod.date << std::endl;
    output << "Morning temperature: " << vivod.tempMorning << std::endl;
    output << "Day temperature: " << vivod.tempDay << std::endl;
    output << "Evening temperature: " << vivod.tempEvening << std::endl;
    output << "Status: " << vivod.status << std::endl;
    output << "Osadki: " << vivod.osadki << std::endl;
    return output;
}

std::istream& operator >> (std::istream& input, ProstoyPrognoz& vvod) {
    long long date;
    double tmor, tday, teven, osadki;
    std::string status;

    input >> date >> tmor >> tday >> teven >> status >> osadki;

    if (!input.good()) {
        return input;  
    }

    ProstoyPrognoz temp;

    temp.setDate(date);
    temp.setOsadki(osadki);

    try {
        temp.setStatus(status);
        temp.setMorningTemp(tmor);
        temp.setDayTemp(tday);
        temp.setEveningTemp(teven);
    }
    catch (const std::out_of_range& ex) {
        std::cout << ex.what() << std::endl;
        input.setstate(std::ios::failbit);
        return input;
    }

    vvod = temp; //Дефолтное присваивание

    return input;
}


//Еще методы
std::string ProstoyPrognoz::worstStatus(std::string stat1, std::string stat2) {
    static const std::string perechen[4] = { "Sunny", "Cloudy", "Rain", "Snow" };
    int znach1 = 0;
    int znach2 = 0;
    for (int i = 0; i < 4; i++) {
        if (stat1 == perechen[i]) {
            znach1 = i;
        }
        if (stat2 == perechen[i]) {
            znach2 = i;
        }
    }

    return (znach1 >= znach2) ? stat1 : stat2;

}

std::string ProstoyPrognoz::findStatus() const {
    double averageTemp = getAverageTemp();

    if (osadki == 0.0) {
        return "Sunny";
    }
    else {
        if (averageTemp > 0.0) {
            return "Rain";
        }
        else {
            return "Snow";
        }
    }
}

