#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>

#define READ_ONLY "r"
#define ERROR_OPEN_INPUT_FILE -1 // Код ошибки чтения входного файла

#define CSFILE  "data/input.txt"  // Файл со списком контактов
#define SCSFILE "data/output.txt" // Файл с отсортированным списком контактов

typedef struct scts {
    int n;
} contstr;

contstr** getContacts(int *res);
int cpstrobj(const void *a, const void *b);
void printlst(const contstr** cslst, int lstlen);

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    contstr **cts = NULL; // Указатель на массив строк, прочитанных из файла
    int resgs = 0;     // Переменная, хранящая число прочитанных строк или код ошибки
    int i = 0;

    if (!(cts = getContacts(&resgs)))
        return resgs;

    printf("Исходный список:\n");
    printlst(cts, resgs); // Выводит на консоль содержимое массива

    qsort(cts, resgs, sizeof(cts), cpstrobj);

    printf("\nОтсортированный список:\n");
    printlst(cts, resgs); // Выводит на консоль содержимое массива

    return 0;
}

// printlst: выводит на экран содержимое массива
void printlst(const contstr** cslst, int lstlen)
{
    int i = 0;

    while (i < lstlen) {
        printf("%3d %d\n", i, cslst[i]->n);
        ++i;
    }
}

// cpstrobj: сравнивает два объекта из множества
int cpstrobj(const void *a, const void *b)
{
    int cmpval_a, cmpval_b;
    cmpval_a = (*((contstr**)a))->n;
    cmpval_b = (*((contstr**)b))->n;

    if (cmpval_a > cmpval_b)
        return 1;
    else if (cmpval_a < cmpval_b)
        return -1;
    else if (cmpval_a == cmpval_b)
        return 0;
}

// getContacts: возвращает указатель на множество объектов структуры или NULL, в случае ошибки
// по адресу st помещает количество объектов множества, или номер ошибки
contstr** getContacts(int *res)
{
    FILE *csFile;            // Указатель на поток открытого файла
    contstr **csList = NULL; // Список указателей на объекты структуры, хранящие записи файла
    int enitem = 0;          // Хранит пункт записи телефонной книги
    int stcnt = 0;           // Счетчик строк, прочитанных из файла
    int i = 0;

    if ((csFile = fopen(CSFILE, READ_ONLY)) == NULL) {
        *res = ERROR_OPEN_INPUT_FILE;
        return NULL;
    }

    // Выделяем память для хранения указателей на объекты структуры
    csList = (contstr **) malloc(15 * sizeof(contstr *));

    while(fscanf(csFile, "%d", &enitem) != EOF) {
        // Выделяем память для объекта структуры
        csList[stcnt] = (contstr*) malloc(sizeof(contstr));
        csList[stcnt]->n = enitem;
        ++stcnt;
    }

    // Перераспределить память

    *res = stcnt;
    return csList;
}
