#pragma once
#include <string>

/**
 * @brief Класс для хранения прогноза погоды на один день.
 * * Хранит дату, температуру в разное время суток, количество осадков
 * и текстовое описание погоды (статус). Поддерживает арифметические операции
 * для объединения прогнозов и сравнения по дате.
 */
class ProstoyPrognoz
{
private:
    /// @brief Дата прогноза (ожидается в формате Unix timestamp).
    long long date;
    /// @brief Температура утром (в градусах Цельсия).
    double tempMorning;
    /// @brief Температура днем.
    double tempDay;
    /// @brief Температура вечером.
    double tempEvening;
    /// @brief Количество осадков в мм.
    double osadki;
    /// @brief Статус погоды в виде строки (например, "Sunny", "Rain").
    std::string status;

public:

    /// @brief Конструктор по умолчанию. Инициализирует поля нулями и пустыми строками.
    ProstoyPrognoz();

    /**
     * @brief Инициализирующий конструктор с ручным вводом всех полей.
     * * @param date Дата прогноза.
     * @param tempMorning Температура утром.
     * @param tempDay Температура днем.
     * @param tempEvening Температура вечером.
     * @param osadki Количество осадков.
     * @param status Статус погоды в виде строки.
     */
    ProstoyPrognoz(long long date, double tempMorning, double tempDay, double tempEvening, double osadki, std::string status);

    /**
     * @brief Инициализирующий конструктор с автоматическим вычислением статуса.
     * * Статус ("Rain", "Snow", "Sunny") вычисляется автоматически
     * на основе температуры и количества осадков.
     * * @param date Дата прогноза.
     * @param tempMorning Температура утром.
     * @param tempDay Температура днем.
     * @param tempEvening Температура вечером.
     * @param osadki Количество осадков.
     */
    ProstoyPrognoz(long long date, double tempMorning, double tempDay, double tempEvening, double osadki);

    /// @brief Деструктор по умолчанию.
    ~ProstoyPrognoz() = default;





    /// @brief Конструктор копирования (по умолчанию).
    ProstoyPrognoz(const ProstoyPrognoz&) = default;


    /// @brief Конструктор перемещения (по умолчанию).
    ProstoyPrognoz(ProstoyPrognoz&&) = default;





    /// @brief Возвращает дату прогноза.
    long long getDate() const {
        return date;
    }

    /// @brief Возвращает утреннюю температуру.
    double getMorningTemp() const {
        return tempMorning;
    }

    /// @brief Возвращает дневную температуру.
    double getDayTemp() const {
        return tempDay;
    }

    /// @brief Возвращает вечернюю температуру.
    double getEveningTemp() const {
        return tempEvening;
    }

    /// @brief Возвращает количество осадков.
    double getOsadki() const {
        return osadki;
    }

    /// @brief Возвращает статус погоды в виде строки.
    std::string getStatus() const {
        return status;
    }





    /**
     * @brief Устанавливает дату прогноза.
     * @param date Новая дата.
     */
    void setDate(long long date);

    /**
     * @brief Устанавливает утреннюю температуру.
     * @param t Новая утренняя температура.
     * @throws std::out_of_range Если температура ниже абсолютного нуля.
     */
    void setMorningTemp(double t);

    /**
     * @brief Устанавливает дневную температуру.
     * @param t Новая дневная температура.
     * @throws std::out_of_range Если температура ниже абсолютного нуля.
     */
    void setDayTemp(double t);

    /**
     * @brief Устанавливает вечернюю температуру.
     * @param t Новая вечерняя температура.
     * @throws std::out_of_range Если температура ниже абсолютного нуля.
     */
    void setEveningTemp(double t);

    /**
     * @brief Устанавливает количество осадков.
     * @param mm Новое количество осадков.
     */
    void setOsadki(double mm);

    /**
     * @brief Устанавливает статус погоды.
     * @param p Новый статус погоды в виде строки.
     * @throws std::out_of_range Если статус неизвестен.
     */
    void setStatus(std::string p);






    /**
     * @brief Вычисляет среднюю температуру за день.
     * @return Среднее арифметическое температур в течение дня, то есть (Утро + День + Вечер) / 3.
     */
    double getAverageTemp() const;


    /**
     * @brief Проверяет прогноз на логические ошибки.
     * * Ищет невозможные ситуации, например:
     * - Дождь при температуре ниже 0.
     * - Температуру ниже абсолютного нуля.
     * - Огромное количество осадков. (> 1500)
     * * @return true, если найдены ошибки, иначе false.
     */
    bool oshibka() const;


 


    /**
     * @brief Оператор объединения прогнозов (+=).
     * * Усредняет температуры и осадки текущего и переданного прогноза.
     * Статус выбирается как "худший" из двух.
     * * @param r Прогноз, который нужно добавить к текущему.
     * @return Ссылка на измененный текущий прогноз.
     * @throws std::invalid_argument Если даты прогнозов разные.
     */
    ProstoyPrognoz& operator += (const ProstoyPrognoz& r);

    /**
     * @brief Оператор трехстороннего сравнения (Spaceship operator).
     * * Сравнивает прогнозы по дате.
     * * @param r Прогноз для сравнения.
     * @return std::strong_ordering::less, equal или greater.
     */
    std::strong_ordering operator <=> (const ProstoyPrognoz& r) const;

    /**
     * @brief Оператор проверки на равенство.
     * * Сравнивает прогнозы по дате.
     * * @param lhs Первый прогноз для сравнения.
     * * @param rhs Второй прогноз для сравнения.
     * @return true, если даты равны, иначе false.
     */
    friend bool operator == (const ProstoyPrognoz& lhs, const ProstoyPrognoz& rhs);






    /// @brief Оператор присваивания копированием (по умолчанию).
    ProstoyPrognoz& operator = (const ProstoyPrognoz&) = default;
        
    /// @brief Оператор присваивания перемещением (по умолчанию).
    ProstoyPrognoz& operator = (ProstoyPrognoz&&) = default;






    /**
     * @brief Вывод прогноза в поток (например, в консоль или файл).
     * Формат вывода: "Date: [date] T:[morning]/[day]/[evening] Rain:[osadki] Status:[status]"
     * * @param output Поток вывода.
     * * @param vivod Ссылка на прогноз, который необходимо вывести.
     * @return Поток вывода.
     */
    friend std::ostream& operator << (std::ostream& output, const ProstoyPrognoz& vivod);

    /**
     * @brief Чтение прогноза из потока.
     * Ожидает данные в порядке, соответствующем выводу (обычно в порядке: Date Morning Day Evening Osadki Status).
     * * @param input Поток ввода.
     * * @param vvod Ссылка на прогноз, который необходимо ввести.
     * @return Поток ввода.
     */
    friend std::istream& operator >> (std::istream& input, ProstoyPrognoz& vvod);

private: 


    /**
     * @brief Выбирает "худший" погодный статус из двух.
     * Например, если один статус "Rain", а другой "Sunny", вернет "Rain".
     * * @param stat1 Первый статус (в виде строки).
     * * @param stat2 Второй статус (в виде строки)
     * @return Худший статус из двух в виде строки.
     */
    static std::string worstStatus(std::string stat1, std::string stat2);

    /**
     * @brief Автоматически определяет статус погоды.
     * Анализирует текущие температуры и осадки объекта.
     * @return Статус в виде строки (например, "Snow", "Rain", "Cloudy", "Sunny").
     */
    std::string findStatus() const;
};

