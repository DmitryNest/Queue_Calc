/*
 ============================================================================
 Name        : Queue_Calc.c
 Author      : D.Nest
 Version     : 1.0
 Description : Калькулятор с использованием очереди и получением задач из файла для использования обратной польской нотации.
 В Input.txt задачи заполняются по форме:
 Первое_Число Второе_Число Оператор & ...
 Пример:
 2 5 * & 5 3 + & 9 ! &
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct list {
    char ch[10];
    struct list* next;
} list;

list* last = NULL;
list* head = NULL;
int _sys = 1;
typedef struct queue {
    float data;
    char ch;
    struct queue* next_queue;
} queue;

queue* lastFqueue = NULL;
queue* headFqueue = NULL;

//Функция начала и конца чтения входящего файла

void InitFile(FILE* input)
{

    list* futureLast = (list*)calloc(1, sizeof(list)); // Выделяем память
    futureLast->next = NULL;
    fscanf(input, "%c", &futureLast->ch[0]);
    int size = 0;
    if (futureLast->ch[0] == '\0') // Если файл заканчивается
    {
        free(futureLast); //Освобождаем память
        _sys = 0;
    }
    else {
        while (futureLast->ch[size] != ' ') {
            size++;
            fscanf(input, "%c", &futureLast->ch[size]);
        }

        if (last != NULL) {

            last->next = futureLast;
            futureLast->next = NULL;
        }
        // Инициализация очереди при первом запуске
        else {
            head = futureLast;
        }
    }
    last = futureLast;
}
float number_calc(char operation, float value1, float value2)
{
    float result_elem; //Возвращаем указатель
    float res; //Локальная переменная для факториала
    if (operation == '+') {
        result_elem = value1 + value2;
    }
    if (operation == '-') {
        result_elem = value1 - value2;
    }
    if (operation == '*') {
        result_elem = value1 * value2;
    }
    if (operation == '/') {
        if (value2 == 0)
            result_elem = 0;
        else
            result_elem = value1 / value2;
    }
    if (operation == '^') {
        if (value2 != 0) {
            result_elem = powf(value1, value2);
        }
        else {
            result_elem = 1;
        }
    }
    if (operation == '!') {
        res = 1;
        for (int i = 1; i <= value1; i++) {
            res = res * i;
        }
        result_elem = res;
    }
    return result_elem;
}
int main(void)
{
    FILE *input, *output; //Файлы на ввод и вывод
    input = fopen("input.txt", "r"); //Открываем файл на ввод
    while (_sys) { // Пока элементы в файле не кончатся
        InitFile(input); // Читаем его
    }
    //Когда файл прочтён
    fclose(input); //Закрываем его
    // Открываем файл для вывода
    output = fopen("output.txt", "w");
    while (head != NULL) // Перебор очереди с элементами
    {
        if (head->ch[0] != '&') {
            if (head->ch[0] == '+' || head->ch[0] == '*' || head->ch[0] == '/' || head->ch[0] == '!' || head->ch[0] == '^' || (head->ch[0] == '-' && head->ch[1] == ' ')) {
                if (head->ch[0] != '!') {
                    headFqueue->next_queue->data = number_calc(head->ch[0], headFqueue->next_queue->data, headFqueue->data);
                }
                else {
                    headFqueue->next_queue->data = number_calc(head->ch[0], headFqueue->data, '0');
                }
                headFqueue->next_queue->ch = head->ch[0];
                //Очистка очереди
                lastFqueue = headFqueue;
                headFqueue = headFqueue->next_queue;
                free(lastFqueue);
            }
            else { // Если элемент - число
                queue* tmp = (queue*)malloc(sizeof(queue));
                tmp->data = atof(head->ch);
                tmp->next_queue = headFqueue;
                headFqueue = tmp;
            }
            // Переход на следующй элемент
        }
        else {
            // Запись результата в файл
            switch (headFqueue->ch) {
            case '+':
                fprintf(output, "Результат сложения: %.2f\n", headFqueue->data);
                break;
            case '-':
                fprintf(output, "Результат вычитания: %.2f\n", headFqueue->data);
                break;
            case '*':
                fprintf(output, "Результат умножения: %.2f\n", headFqueue->data);
                break;
            case '/':
                fprintf(output, "Результат деления: %.2f\n", headFqueue->data);
                break;
            case '!':
                fprintf(output, "Факториал: %.2f\n", headFqueue->data);
                break;
            case '^':
                fprintf(output, "Результат возведения в степень: %.2f\n", headFqueue->data);
                break;
            }
            free(headFqueue); // Удаление последнего элемента из очереди
        }
        head = head->next;
    }
    fclose(output); //Закрытие файла на вывод
    return 0;
}
