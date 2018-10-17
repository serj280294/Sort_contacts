#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>

#define ENTRYSAMO 20 // Количество записей, под которое предварительно выделяется память
#define BUFSTRLEN 30 // Максимальная длина одной записи
#define WDSBFNUM 2   // Количество слов, перед номером телефона, в записи

#define READ_ONLY "r"

#define CSFILE  "data/input.txt"  // Файл со списком контактов
#define SCSFILE "data/output.txt" // Файл с отсортированным списком контактов

#define ERROR_OPEN_INPUT_FILE -1 // Код ошибки чтения входного файла
#define ENDFILE 1   // Достигнут конец файла

// Ошибки чтения строк из файла
#define MANYWORDS 1
#define MANYNUMBS 2
#define MANYSYMBS 3
#define FEWITEMS  4

typedef struct scts {
    int n;
} contstr;

contstr** getContacts(int *res);
void slfree(char **lst, int stramo);
int lststrcmp(const void *a, const void *b);
void toendstr(FILE *f, int *ef);

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    char **cts = NULL; // Указатель на массив строк, прочитанных из файла
    int resgs = 0;     // Переменная, хранящая число прочитанных строк или код ошибки
    int i = 0;

    cts = getContacts(&resgs);

    //printlst(cns, stgcs); // Временно (!!!!!!!!!!!!!!!!!!!!)

    //qsort(cns, stgcs, sizeof(char**), lststrcmp);

    // printlst(cns, stgcs);

    return 0;
}

// getContacts: возвращает указатель на массив строк или NULL, в случае ошибки
// по адресу st помещает количество строк, содержащихся в массиве, или номер ошибки
contstr** getContacts(int *res)
{
    FILE *csFile;            // Указатель на поток открытого файла
    contstr **csList = NULL; // Список указателей на объекты структуры, хранящие записи файла
    contstr *str = NULL;     // Указатель на объект структуры, хранящий строку телефонной книги
    int enitem = 0;          // Временное хранилище пункта записи телефонной книги
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

// lststrcmp: сравнивает две строки из массива строк
int lststrcmp(const void *a, const void *b)
{
    return strcmp(*(char**)a, *(char**)b);
}

// Возвращает указатель на область данных, содержащую строку или NULL, в случае ошибки или конца файла
// По адресу gsstat помещает номер ошибки или информацию о конце файла
char* getString(FILE *fp, int *st, int *ef)
{
    char *str, c;
    int stitms = 0; // Счетчик пунктов, содержащихся в записи телефонной книги
    int len = 0;
    int i = 0;

    *st = 0; // Сброс состояния переменной статуса операции

    // Выделяем память под строку
    str = (char*) malloc(BUFSTRLEN * sizeof(char));

    while ((c = fgetc(fp)) != EOF && c != '\n') {
        if (i == BUFSTRLEN-1) {
            toendstr(fp, ef);
            *st = MANYSYMBS; // Превышено макимально возможное количество символов в строке
            free(str);
            return NULL;
        }

        if (isspace(c)) { // Заменить!!!
            if (i != 0 && !(isspace(str[i-1])) && stitms < WDSBFNUM+1) {
                str[i] = ' ';
                ++i;
            }
        }
        else if (isalpha(c)) { // Заменить на isletter
            if (i == 0 || isspace(str[i-1]))
                ++stitms;

            if (stitms > WDSBFNUM) {
                toendstr(fp, ef);
                *st = MANYWORDS; // Превышено количество слов, содержащхся в строке
                free(str);
                return NULL;
            }
            str[i] = c;
            ++i;
        }
        else if (isdigit(c)) { // Заменить на isnumber
            if (isspace(str[i-1]) && stitms == WDSBFNUM) { // Начало номера
                ++stitms;
            }
            else if (stitms > WDSBFNUM+1) {
                toendstr(fp, ef);
                *st = MANYNUMBS; // Номер уже был прочитан
                free(str);
                return NULL;
            }
            str[i] = c;
            ++i;
        }
    }

    if (c == EOF) {
        *ef = ENDFILE;
    }

    if (stitms < WDSBFNUM+1) {
        *st = FEWITEMS; // Пунктов в записи меньше, чем необходимо
        free(str);
        return NULL;
    }

    str[i] = '\0';

    return (char*) realloc(str, i * sizeof(char)); // Перераспределяем память по размеру строки
}

// isspacesmb: Проверяет, является ли символ пробельным
int isspacesmb(char c)
{
    return (c == ' ' || c == ',' || c == '.' || c == '_'|| c == '\t') ? 1 : 0;
}

// isletter: Проверяет, является ли символ буквой русского или английского алфавита
int isletter(char c)
{
    return (c >= (unsigned char)'А' && c <= (unsigned char)'я') ||
           (c >= (unsigned char)'A' && c <= (unsigned char)'z') ? 1 : 0;
}

// isnumber: Проверяет, является ли символ пробельным
int isnumber(char c)
{
    return ((c >= 0 && c <=9) || c == '+' || c == '-' || c == '(' || c == ')') ? 1 : 0;
}

// toendstr: сдвигает указатель файла на символ, следующий за концом строки
void toendstr(FILE *f, int *ef)
{
    int c;
    while ((c=fgetc(f))!=EOF && c != '\n')
        ;

    // В случае конца файла - установить флаг
    if (c == EOF)
        *ef = ENDFILE;
}

// slfree: освобождает память, выделенную на строки списка и на массив указателей
void slfree(char **lst, int stramo)
{
    int i = 0;

    while (i < stramo) {
        free(*(lst+i));
        ++i;
    }
    free(lst);
}

// printlst: выводит на экран массив строк
void printlst(const char** cslst, int lstlen)
{
    int i = 0;

    while (i < lstlen) {
        printf("%d %s\n", i, *(cslst+i));
        ++i;
    }
}
