#pragma once
#include "Prostoy.h"

/**
 * @brief Класс-контейнер для управления массивом прогнозов погоды.
 * * Представляет собой динамический массив (аналог std::vector), который хранит
 * объекты класса ProstoyPrognoz. Реализует логику добавления, удаления,
 * сортировки, поиска и фильтрации прогнозов.
 * Управляет динамической памятью (Правило 5).
 */
class SlozhniyPrognoz
{
private:

    /// @brief Указатель на динамический массив прогнозов.
    ProstoyPrognoz* prognozi;

    /// @brief Текущее количество элементов в массиве.
    size_t count;

    /// @brief Текущая вместимость массива (сколько памяти выделено).
    size_t capacity;

    /// @brief Флаг, указывающий, отсортирован ли массив по дате (для оптимизации).
    bool sortedStatus;


    /**
     * @brief Внутренний метод для перевыделения памяти.
     * * Если newCapacity > capacity, выделяет новый блок памяти,
     * копирует туда старые данные и удаляет старый блок.
     * @param newCapacity Новый размер буфера (в элементах).
     */
    void reserve(size_t newCapacity);

public:

    /// @brief Конструктор по умолчанию. Создает пустой массив с нулевой вместимостью.
    SlozhniyPrognoz();

    /**
     * @brief Инициализирующий конструктор на основе существующего массива.
     * Копирует данные из переданного массива в новый объект.
     * @param arr Указатель на исходный массив ProstoyPrognoz.
     * @param size Количество элементов в исходном массиве.
     */
    SlozhniyPrognoz(const ProstoyPrognoz* arr, size_t size);

    /**
     * @brief Инициализирующий конструктор создания списка из одного прогноза.
     * @param prognoz Прогноз, который станет первым элементом списка.
     */
    SlozhniyPrognoz(const ProstoyPrognoz& prognoz);

    /**
     * @brief Конструктор копирования.
     * Создает копию объекта (выделяет новую память и копирует все элементы).
     * @param other Объект, который нужно скопировать.
     */
    SlozhniyPrognoz(const SlozhniyPrognoz& other);

    /**
     * @brief Конструктор перемещения.
     * Забирает ресурсы (указатель на память) у другого объекта, оставляя его пустым.
     * @param other r-value ссылка на временный объект.
     */
    SlozhniyPrognoz(SlozhniyPrognoz&& other) noexcept;

    /// @brief Деструктор. Освобождает выделенную динамическую память.
    ~SlozhniyPrognoz();




    /**
     * @brief Возвращает текущее количество прогнозов в списке.
     * @return Количество элементов (count).
     */
    size_t size() const {
        return count;
    }

 


    /**
     * @brief Оператор добавления элемента (аналог push_back).
     * Добавляет новый прогноз в конец списка. При необходимости увеличивает capacity.
     * Сбрасывает флаг sortedStatus в false.
     * @param newPrognoz Прогноз для добавления.
     * @return Ссылка на текущий объект.
     */
    SlozhniyPrognoz& operator += (const ProstoyPrognoz& newPrognoz);

    /**
     * @brief Оператор доступа по индексу (для чтения и записи).
     * @param index Индекс элемента (от 0 до count-1).
     * @return Ссылка на элемент массива.
     * @throws std::out_of_range Если индекс выходит за пределы массива.
     */
    ProstoyPrognoz& operator [] (size_t index);

    /**
     * @brief Оператор доступа по индексу (только для чтения).
     * @param index Индекс элемента.
     * @return Константная ссылка на элемент.
     * @throws std::out_of_range Если индекс выходит за пределы массива.
     */
    const ProstoyPrognoz& operator [] (size_t index) const;

    /**
     * @brief Оператор присваивания копированием.
     * Удаляет старые данные и копирует данные из other.
     * @param other Объект-источник.
     * @return Ссылка на текущий объект.
     */
    SlozhniyPrognoz& operator = (const SlozhniyPrognoz& other);

    /**
     * @brief Оператор присваивания перемещением.
     * Забирает память у other и очищает его.
     * @param other Объект-источник (r-value).
     * @return Ссылка на текущий объект.
     */
    SlozhniyPrognoz& operator = (SlozhniyPrognoz&& other) noexcept;

    /**
     * @brief Вывод всех прогнозов в поток.
     * Выводит количество элементов, а затем каждый прогноз с новой строки.
     * * @param output Поток вывода.
     * * @param vivod Ссылка на массив, который необходимо вывести.
     * @return Поток вывода.
     */
    friend std::ostream& operator << (std::ostream& output, const SlozhniyPrognoz& vivod);

    /**
     * @brief Чтение списка прогнозов из потока.
     * Сначала ожидает количество элементов, затем сами элементы.
     * * @param input Поток ввода.
     * * @param vvod Ссылка на массив, который необходимо ввести.
     * @return Поток ввода.
     */
    friend std::istream& operator >> (std::istream& input, SlozhniyPrognoz& vvod);






    /**
     * @brief Удаляет прогноз по указанному индексу.
     * Сдвигает оставшиеся элементы влево.
     * @param index Индекс удаляемого элемента.
     * @throws std::out_of_range Если индекс неверен.
     */
    void remove(size_t index);

    /**
     * @brief Ищет самый холодный день в заданном диапазоне дат.
     * Сравнивает прогнозы по средней температуре (getAverageTemp).
     * @param dateStart Начало периода (включительно).
     * @param dateEnd Конец периода (включительно).
     * @return Копия найденного прогноза с минимальной температурой.
     * @throws std::logic_error Если подходящих дней нет, или переданный массив пуст.
     */
    ProstoyPrognoz getColdestDay(long long dateStart, long long dateEnd) const;

    /**
     * @brief Находит первый солнечный день после указанной даты.
     * Ищет прогноз со статусом "Sunny" с датой > currentDate.
     * @param currentDate Дата, после которой начинать поиск.
     * @return Найденный прогноз.
     * @throws std::logic_error Если подходящих дней нет, или если переданный массив пуст.
     */
    ProstoyPrognoz getNextSunnyDay(long long currentDate) const;

    /**
     * @brief Удаляет ошибочные прогнозы из списка.
     * Проходит по всему массиву и вызывает метод oshibka() для каждого элемента.
     * Если прогноз ошибочный, он удаляется.
     */
    void removeOshibki();

    /**
     * @brief Объединяет прогнозы с одинаковой датой.
     * Если найдены два прогноза с одной датой, они складываются (operator+= класса ProstoyPrognoz),
     * результат сохраняется, а дубликат удаляется.
     */
    void mergePovtorki();

    /**
     * @brief Сортирует прогнозы по дате (по возрастанию).
     * Использует алгоритм сортировки (bubble sort).
     * Устанавливает флаг sortedStatus = true.
     */
    void sortDates();

    /**
     * @brief Создает выборку прогнозов за определенный месяц.
     * @param date Любая дата, входящая в интересующий месяц и год.
     * @return Новый объект SlozhniyPrognoz, содержащий только прогнозы того же месяца и года.
     */
    SlozhniyPrognoz getMonth(long long date) const;

};

