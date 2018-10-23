#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>

#define ENTRYSAMO 20 // Количество записей, под которое выделяется память

#define NAMESLEN 30 // Длина строк, в объекте структуры, хранящих имя, фамилию, отчество
#define NUMLEN   20 // Длина строки, в объекте структуры, хранящей телефонный номер

#define STRNAMESLEN "29" // Должно совпадать с NAMESLEN-1 (Чтобы вместился символ конца строки)
#define STRNUMLEN   "19" // Должно совпадать с NUMLEN-1 (Чтобы вместился символ конца строки)
#define SCANFMTSTR  "%" STRNAMESLEN "s %" STRNAMESLEN "s %" STRNAMESLEN "s %" STRNUMLEN "s"
#define PRINTFMTSTR "%s %s %s %s\n"

#define READ_ONLY "r"
#define ERROR_OPEN_INPUT_FILE -1 // Код ошибки чтения входного файла

#define CSFILE  "data/input.txt"  // Файл со списком контактов
#define SCSFILE "data/output.txt" // Файл с отсортированным списком контактов

typedef struct scts {
    char fname[NAMESLEN]; // First name
    char lname[NAMESLEN]; // Last name
    char ptnmc[NAMESLEN]; // Patronymic
    char tnumb[NUMLEN];   // Telephone number
} contstr;

contstr** getContacts(int *res);
int cpstrobj(const void *a, const void *b);
void printlst(const contstr** cslst, int lstlen);

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    contstr **cts = NULL; // Указатель на массив строк, прочитанных из файла
    int resgs = 0;        // Переменная, хранящая число прочитанных строк или код ошибки
    int i = 0;

    if (!(cts = getContacts(&resgs))) {
        return resgs;
    }

    printf("Initial list:\n");
    printlst(cts, resgs);

    qsort(cts, resgs, sizeof(cts), cpstrobj);

    printf("\nSorted list:\n");
    printlst(cts, resgs);

    return 0;
}

// printlst: выводит на экран содержимое массива
void printlst(const contstr** cslst, int lstlen)
{
    int i = 0;

    while (i < lstlen) {
        printf(PRINTFMTSTR, cslst[i]->fname, cslst[i]->lname, cslst[i]->ptnmc, cslst[i]->tnumb);
        ++i;
    }
}

// cpstrobj: сравнивает два объекта из множества
int cpstrobj(const void *a, const void *b)
{
    int res = 0;

    if (res = strcmp((*((contstr**)a))->fname, (*((contstr**)b))->fname)) {
        return res;
    }
    // Если имена совпадают - сравнить фамилии
    else if (res = strcmp((*((contstr**)a))->lname, (*((contstr**)b))->lname)) {
        return res;
    }
    // Если фамилии совпадают - сравнить отчества
    else if (res = strcmp((*((contstr**)a))->ptnmc, (*((contstr**)b))->ptnmc)) {
        return res;
    }
    else
        return 0; // Строки одинаковы
}

// getContacts: возвращает указатель на множество объектов структуры или NULL, в случае ошибки
// по адресу st помещает количество объектов множества, или номер ошибки
contstr** getContacts(int *res)
{
    FILE *csFile;            // Указатель на поток открытого файла
    contstr **csList = NULL; // Список указателей на объекты структуры, хранящие записи файла
    contstr entry;           // Хранит строку записи телефонной книги, считанной из файла
    int coext = 1;           // Множитель объема памяти, выделяемого для хранения строк файла
    int stcnt = 0;           // Счетчик строк, прочитанных из файла
    int i = 0;

    if ((csFile = fopen(CSFILE, READ_ONLY)) == NULL) {
        *res = ERROR_OPEN_INPUT_FILE;
        return NULL;
    }

    // Выделяем память для хранения указателей на объекты структуры
    csList = (contstr **) malloc(ENTRYSAMO * sizeof(contstr *));

    while(fscanf(csFile, SCANFMTSTR, &entry.fname, &entry.lname, &entry.ptnmc, &entry.tnumb) != EOF) {
        if (stcnt == ENTRYSAMO * coext) {
            // Увеличиваем объем выделенной памяти для хранения указателей на объекты структуры
            ++coext;
            csList = (contstr **) realloc(csList, ENTRYSAMO * coext * sizeof(contstr *));
        }

        // Выделяем память для объекта структуры
        csList[stcnt] = (contstr*) malloc(sizeof(contstr));

        // Перенос строковых данных
        memccpy(csList[stcnt]->fname, entry.fname, (int)'\0', sizeof(csList[stcnt]->fname));
        memccpy(csList[stcnt]->lname, entry.lname, (int)'\0', sizeof(csList[stcnt]->lname));
        memccpy(csList[stcnt]->ptnmc, entry.ptnmc, (int)'\0', sizeof(csList[stcnt]->ptnmc));
        memccpy(csList[stcnt]->tnumb, entry.tnumb, (int)'\0', sizeof(csList[stcnt]->tnumb));

        ++stcnt;
    }

    if (stcnt < ENTRYSAMO * coext) {
        // Уменьшить количество выделенной памяти под указатели на объекты структур
        csList = (contstr **) realloc(csList, stcnt * sizeof(contstr *));
    }

    *res = stcnt;
    return csList;
}
