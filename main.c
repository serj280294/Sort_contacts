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

char** getContacts(int *opst);
char* getString(FILE *fp, int *st, int *ef);
void slfree(char **lst, int stramo);
int lststrcmp(const void *a, const void *b);
void toendstr(FILE *f, int *ef);
int isspacesmb(char c);
int isletter(char c);
int isnumber(char c);

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    char **cns = NULL; // Указатель на массив строк, прочитанных из файла
    int stgcs = 0;      // Переменная, хранящая число прочитанных строк или код ошибки
    int i = 0;

    if ((cns = getContacts(&stgcs)) == NULL) {
        //if (stgcs == ERROR_OPEN_INPUT_FILE)
        //    printf("Ошибка открытия входного файла");

        return stgcs;
    }

    //printlst(cns, stgcs); // Временно (!!!!!!!!!!!!!!!!!!!!)

    //qsort(cns, stgcs, sizeof(char**), lststrcmp);

    // printlst(cns, stgcs);

    return 0;
}

// lststrcmp: сравнивает две строки из массива строк
int lststrcmp(const void *a, const void *b)
{
    return strcmp(*(char**)a, *(char**)b);
}

// getContacts: возвращает указатель на массив строк или NULL, в случае ошибки
// по адресу st помещает количество строк, содержащихся в массиве, или номер ошибки
char** getContacts(int *opst)
{
    FILE *csFile;     // Указатель на поток открытого файла
    char **cslist;    // Массив указателей на строки записной книги
    int coalloc = 1;  // Множитель количества ячеек выделяемой памяти под строки телефонной книги
    int stgs = 0;     // Переменная для хранения кода результата выполнения операции чтения строки
    int fend = 0;     // Переменная-флаг завершения файла
    char *str = NULL; // Указатель на строку
    int i = 0, j = 1;

    if ((csFile = fopen(CSFILE, READ_ONLY)) == NULL) {
        *opst = ERROR_OPEN_INPUT_FILE;
        return NULL;
    }

    // Выделяем память для хранения ENTRYSAMO указателей на строки
    cslist = (char**) malloc(ENTRYSAMO * sizeof(char*));

    // Заполняем массив указателей адресами строк
    str = getString(csFile, &stgs, &fend);
    while (!(fend)) {
        if (str) {

            printf("S%d: %s %d\n", j, str, strlen(str));

            if (i == ENTRYSAMO * coalloc) { // Превышен размер массива указателей на строки
                ++coalloc; // Увеличиваем множитель количества ячеек
                // Перераспределяем массив с указанием нового размера
                cslist = (char**)realloc(cslist, ENTRYSAMO * coalloc * sizeof(char*));
            }
            *(cslist + i) = str; // Помещаем адрес строки в ячейку
            ++i;
        }
        else {
            printf("Err S%d: ", j);
            switch (stgs) {
            case MANYSYMBS:
                printf("Превышено макcимально возможное количество символов в строке");
                break;
            case MANYWORDS:
                printf("Превышено ожидаемое количество слов в записи");
                break;
            case MANYNUMBS:
                printf("Превышено макcимально возможное количество номеров");
                break;
            case FEWITEMS:
                printf("В строке содержится меньшее количество пунктов, чем ожидалось");
                break;
            }
            putchar('\n');
        }

        str = getString(csFile, &stgs, &fend);
        ++j;
    }

    // Если цикл завершился не из-за окончания файла - ошибка
    if (stgs == ENDFILE) {
        *opst = i; // Размещаем количество строк, прочитанных из файла
        // Возвращаем адрес массива указателей на строки, сократив число незанятых элементов
        return (char**)realloc(cslist, i * sizeof(char*));
    }
    else {
        *opst = stgs;
        slfree(cslist, i); //  Освободить каждую строку и массив указателей
        return NULL;
    }
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
        if (isspacesmb(c)) {
            if (i != 0 && !(isspace(str[i-1])) && stitms < WDSBFNUM+1) {
                str[i] = ' ';
                ++i;
            }
        }
        else if (isletter(c)) {
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
        else if (isnumber(c)) {
            if (isspace(str[i-1]) && stitms == WDSBFNUM) {
                ++stitms;
            }

            else if (stitms > WDSBFNUM+1) {
                toendstr(fp, ef);
                *st = MANYNUMBS;
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

// isnumber: Проверяет, относится ли символ к номеру
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
