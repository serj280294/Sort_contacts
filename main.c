#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>

#define ENTRYSAMO 20 // ���������� �������, ��� ������� �������������� ���������� ������
#define BUFSTRLEN 15 // ������������ ����� ����� ������
#define WDSBFNUM 2   // ���������� ����, ����� ������� ��������, � ������

#define READ_ONLY "r"

#define CSFILE  "data/input.txt"  // ���� �� ������� ���������
#define SCSFILE "data/output.txt" // ���� � ��������������� ������� ���������

#define ERROR_OPEN_INPUT_FILE -1 // ��� ������ ������ �������� �����
#define ENDFILE 1   // ��������� ����� �����

// ������ ������ ����� �� �����
#define MANYWORDS 1
#define MANYNUMBS 2
#define MANYSYMBS 3
#define FEWITEMS  4

char** getContacts(int *opst);
char* getString(FILE *fp, int *st, int *ef);
void slfree(char **lst, int stramo);
int lststrcmp(const void *a, const void *b);
void toendstr(FILE *f, int *ef);

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    char **cns = NULL; // ��������� �� ������ �����, ����������� �� �����
    int stgcs = 0;      // ����������, �������� ����� ����������� ����� ��� ��� ������
    int i = 0;

    if ((cns = getContacts(&stgcs)) == NULL) {
        //if (stgcs == ERROR_OPEN_INPUT_FILE)
        //    printf("������ �������� �������� �����");

        return stgcs;
    }

    //printlst(cns, stgcs); // �������� (!!!!!!!!!!!!!!!!!!!!)

    //qsort(cns, stgcs, sizeof(char**), lststrcmp);

    // printlst(cns, stgcs);

    return 0;
}

// lststrcmp: ���������� ��� ������ �� ������� �����
int lststrcmp(const void *a, const void *b)
{
    return strcmp(*(char**)a, *(char**)b);
}

// getContacts: ���������� ��������� �� ������ ����� ��� NULL, � ������ ������
// �� ������ st �������� ���������� �����, ������������ � �������, ��� ����� ������
char** getContacts(int *opst)
{
    FILE *csFile;     // ��������� �� ����� ��������� �����
    char **cslist;    // ������ ���������� �� ������ �������� �����
    int coalloc = 1;  // ��������� ���������� ����� ���������� ������ ��� ������ ���������� �����
    int stgs = 0;     // ���������� ��� �������� ���� ���������� ���������� �������� ������ ������
    int fend = 0;     // ����������-���� ���������� �����
    char *str = NULL; // ��������� �� ������
    int i = 0, j = 1;

    if ((csFile = fopen(CSFILE, READ_ONLY)) == NULL) {
        *opst = ERROR_OPEN_INPUT_FILE;
        return NULL;
    }

    // �������� ������ ��� �������� ENTRYSAMO ���������� �� ������
    cslist = (char**) malloc(ENTRYSAMO * sizeof(char*));

    // ��������� ������ ���������� �������� �����
    str = getString(csFile, &stgs, &fend);
    while (!(fend)) {
        if (str) {

            printf("S%d: %s\n", j, str);

            if (i == ENTRYSAMO * coalloc) { // �������� ������ ������� ���������� �� ������
                ++coalloc; // ����������� ��������� ���������� �����
                // ���������������� ������ � ��������� ������ �������
                cslist = (char**)realloc(cslist, ENTRYSAMO * coalloc * sizeof(char*));
            }
            *(cslist + i) = str; // �������� ����� ������ � ������
            ++i;
        }
        else {
            printf("Err S%d: ", j);
            switch (stgs) {
            case MANYSYMBS:
                printf("��������� ���c������� ��������� ���������� �������� � ������");
                break;
            case MANYWORDS:
                printf("��������� ��������� ���������� ���� � ������");
                break;
            case MANYNUMBS:
                printf("��������� ���c������� ��������� ���������� �������");
                break;
            case FEWITEMS:
                printf("� ������ ���������� ������� ���������� �������, ��� ���������");
                break;
            }
            putchar('\n');
        }

        str = getString(csFile, &stgs, &fend);
        ++j;
    }

    // ���� ���� ���������� �� ��-�� ��������� ����� - ������
    if (stgs == ENDFILE) {
        *opst = i; // ��������� ���������� �����, ����������� �� �����
        // ���������� ����� ������� ���������� �� ������, �������� ����� ��������� ���������
        return (char**)realloc(cslist, i * sizeof(char*));
    }
    else {
        *opst = stgs;
        slfree(cslist, i); //  ���������� ������ ������ � ������ ����������
        return NULL;
    }
}

// ���������� ��������� �� ������� ������, ���������� ������ ��� NULL, � ������ ������ ��� ����� �����
// �� ������ gsstat �������� ����� ������ ��� ���������� � ����� �����
char* getString(FILE *fp, int *st, int *ef)
{
    char *str, c;
    int stitms = 0; // ������� �������, ������������ � ������ ���������� �����
    int i = 0, len = 0;

    *st = 0; // ����� ��������� ���������� ������� ��������

    // �������� ������ ��� ������
    str = (char*) malloc(1000 * sizeof(char));

    while ((c = fgetc(fp)) != EOF && c != '\n') {
        if (i == BUFSTRLEN-1) {
            toendstr(fp, ef);
            *st = MANYSYMBS; // ��������� ���������� ��������� ���������� �������� � ������
            free(str);
            return NULL;
        }

        if (isspace(c)) { // ��������!!!
            if (i != 0 && !(isspace(str[i-1])) && stitms < WDSBFNUM+1) {
                str[i] = ' ';
                ++i;
            }
        }
        else if (isalpha(c)) { // �������� �� isletter
            if (i == 0 || isspace(str[i-1]))
                ++stitms;

            if (stitms > WDSBFNUM) {
                toendstr(fp, ef);
                *st = MANYWORDS; // ��������� ���������� ����, ����������� � ������
                free(str);
                return NULL;
            }
            str[i] = c;
            ++i;
        }
        else if (isdigit(c)) { // �������� �� isnumber
            if (isspace(str[i-1]) && stitms == WDSBFNUM) { // ������ ������
                ++stitms;
            }
            else if (stitms > WDSBFNUM+1) {
                toendstr(fp, ef);
                *st = MANYNUMBS; // ����� ��� ��� ��������
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
        *st = FEWITEMS; // ������� � ������ ������, ��� ����������
        free(str);
        return NULL;
    }

    str[i] = '\0';

    return str;

    //if (stsc > 0) {
       // printf("%s\n", str);
    //}

    // �������� �� ����� ������

    // ���������������� ������

    //stsc = fscanf(fp, "%s%n", str, &a);

    //if (stsc > 0) {
        //printf("%s\n", str);
    //}

    //if (stsc > 0) {
    //    return str;
    //}
    //else {
        // ��������� ������
//        switch (stsc) {
//        case EOF:
//            *st = ENDFILE;
//            break;
//        case 0:
//            *st = STRERR;
//            break;
//        }
//        free(str);
//        return NULL;
    //}
}

// toendstr: �������� ��������� ����� �� ������, ��������� �� ������ ������
void toendstr(FILE *f, int *ef)
{
    int c;
    while ((c=fgetc(f))!=EOF && c != '\n')
        ;

    // � ������ ����� ����� - ���������� ����
    if (c == EOF)
        *ef = ENDFILE;
}

// slfree: ����������� ������, ���������� �� ������ ������ � �� ������ ����������
void slfree(char **lst, int stramo)
{
    int i = 0;

    while (i < stramo) {
        free(*(lst+i));
        ++i;
    }
    free(lst);
}

// printlst: ������� �� ����� ������ �����
void printlst(const char** cslst, int lstlen)
{
    int i = 0;

    while (i < lstlen) {
        printf("%d %s\n", i, *(cslst+i));
        ++i;
    }
}