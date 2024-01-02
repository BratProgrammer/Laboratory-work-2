#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// Проверка на то что символы являются числом типа float
bool is_it_float_number(char *str[]) {
    unsigned int dot_number = 0;
    for (int i = 0; i < (int)strlen((*str)); i++) {

        // Игнорирование \n последнего числа в строке матрицы
        if ((*str)[i] == '\n' && i == (int)strlen(*str) - 1) continue;

        // Проверка на то, что минус может стоять только на 1-ой позиции и при этом он не может быть единственным символом
        if ((*str)[i] == '-' && i == 0) {
            if (strlen((*str)) > 1) continue;
            else return false;
        } else if ((*str)[i] == '-') return false;

        // Проверка на наличие других обрабатываемых исключений (см коментарии из блока в if { ... })
        if (!(('0' <= (*str)[i] && (*str)[i] <= '9') || (*str)[i] == '.')) {

            // проверка на то, что точка может встречаться только 1 раз
            if ((*str)[i] == '.') {
                dot_number += 1;
                if (dot_number > 1) {
                    return false;
                }
            } else if (!('0' <= (*str)[i] && (*str)[i] <= '9')) {
                return false;
            }
        }
    }
    return true;
}

// Проверка на то, что набор символов является целым числом
bool is_it_uint_number(char *str[]) {
    for (int i = 0; i < (int)strlen(*str); i++) {
        if (!('0' <= (*str)[i] && (*str)[i] <= '9')) {
            return false;
        }
    }
    return true;
}

// Функция преобразовывающая строку в unsigned int
unsigned int get_uint_from_string(char *str[]) {
    int power_of_ten = 0;
    int index = strlen(*str);
    unsigned int value = 0;
    // Пройдём по всем элементам полученной строки и переведём их в соответствующие им числовые значения
    while (index) {
        // Прибавляем к value значение каждого символа домноженое на его степень 10
        value += (pow(10, power_of_ten)) * (int)((*str)[index - 1] - '0'); 
        power_of_ten += 1;
        index -= 1;
    }
    return value;
}

// Функция считающая характеристику строки матрицы
int get_characteristic(float *matrix_string[], int *width) {
    long double sum = 0;
    for (int i = 0; i < *width; i++) {
        sum += (*matrix_string)[i];
    }
    return sqrt(sum);
}

int main(int argc, char *argv[]) {
    // Проверка количесвта введённых аргументов
    if (argc < 3 || (argc == 3 && (strcmp(argv[1], "-m") == 0))) {
        printf("Ошибка: отсутствует обязательный аргумент.\n");
        exit(1);
    } 
    // Проверяем на поддерживающуюся опцию в начале ввода
    if (argv[1][0] == '-' && (strcmp(argv[1], "-m") != 0)) {
        printf("Ошибка: опция '%s' не поддерживается.\n", argv[1]);
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        if (i == 1 && strcmp(argv[1], "-m") == 0) continue;
        // Проверка на поддерживающуюся опцию не в первом аргументе
        else if (argv[i][0] == '-') {
            printf("Ошибка: опция '%s' не поддерживается.\n", argv[i]);
            exit(1);
        }
        // Проверка на корректность введённых чисел
        else if (is_it_uint_number(&argv[i]) == false) {
            printf("Ошибка: '%s' не является числом.\n", argv[i]);
            exit(1);
        }
    }

    int height = 0;
    int width = 0;

    //Считывание размера матрицы
    if (strcmp(argv[1], "-m") == 0) {
        height = get_uint_from_string(&argv[2]);
        width = get_uint_from_string(&argv[3]);
    } else {
        height = get_uint_from_string(&argv[1]);
        width = get_uint_from_string(&argv[2]);
    }
    
    // Создаём массив указателей на строки матрицы (массив указателей на указатели типа float)
    float **matrix;

    // Создание массива указателей
    matrix = malloc(height * sizeof(float*));

    // Создание строк
    // если был передан -m то начинаем считывать строки
    if (strcmp(argv[1], "-m") == 0) {
        printf("Введите строки матрицы:\n");
        for (int i = 0; i < height; i++) {

            // Создаём массив типа float в куче, который будет строкой матрицы
            float *new_str = malloc(width * sizeof(float));
            // Создаём массив, в который считаем введённую строку
            char input_str[200];

            // Получаем строку
            fgets(input_str, 200, stdin);
            // Чистим буфер для корректного считывания следующих строк
            fflush(stdin);

            // Разбиваем введённую строку по пробелам и кладём каждую такую подстроку в value
            char *value = strtok(input_str, " ");
            // Номер числа в строке
            int value_index = 0;

            // Пока в value есть значение проверяем его на то что оно число нужного формата и записываем в строку
            while (value != NULL) {
                if (is_it_float_number(&value)) {
                    new_str[value_index] = strtof(value, NULL);
                    value_index++;
                    value = strtok(NULL, " ");
                }
                //Выводи ошибку если value не число
                else {
                    printf("Ошибка: '%s' не является числом.\n", value);
                    exit(1);
                }
            }
            // Записываем указатель на строку в нашу матрицу (массив указателей на строки матрицы в куче)
            matrix[i] = new_str;
        }   

    //Иначе заполняем строки рандомными значениями
    } else {
        for (int i = 0; i < height; i++) {
            // Выделяем пямять под строку в куче
            float *new_str = malloc(width * sizeof(float));
            // Рандомизируем стартовое значение rand()
            srand(time(NULL) + i);
            for (int j = 0; j < width; j++) {
                // Получение рандомного float
                float random_value = (float)rand() / RAND_MAX + (float)(rand() % 100);
                new_str[j] = random_value;
            }
            // Записываем указатель на строку в нашу матрицу (массив указателей на строки матрицы в куче)
            matrix[i] = new_str;
        }
    }

    printf("Исходная матрица:\n");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%f ", matrix[i][j]);
        }
        printf("\n");
    }

    // Сортируем строки пузырьком по значению их характеристик
    for (int i = height - 1; i > 0; i--) {
        for (int j = height - 1 - i; j > 0; j--) {
            if (get_characteristic(&matrix[j], &width) < get_characteristic(&matrix[j + 1], &width)) {
                // Меняем местами указатели на строки
                float *buf_p = matrix[j];
                matrix[j] = matrix[j + 1];
                matrix[j + 1] = buf_p;
            }
        }
    }

    printf("Результат:\n");
    // Выводим полученный результат
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%f ", matrix[i][j]);
        }
        printf("\n");

        // После вывода строки матрицы очищаем память в куче, которую она занимала
        free(matrix[i]);
    }

    // Очищаем память в куче, которую занимал массив указателей на строки
    free(matrix);

    return 0;    
}