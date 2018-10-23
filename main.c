#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>

#define ENTRYSAMO 20 // ���������� �������, ��� ������� ���������� ������

#define NAMESLEN 30 // ����� �����, � ������� ���������, �������� ���, �������, ��������
#define NUMLEN   20 // ����� ������, � ������� ���������, �������� ���������� �����

#define STRNAMESLEN "29" // ������ ��������� � NAMESLEN-1 (����� ��������� ������ ����� ������)
#define STRNUMLEN   "19" // ������ ��������� � NUMLEN-1 (����� ��������� ������ ����� ������)
#define SCANFMTSTR  "%" STRNAMESLEN "s %" STRNAMESLEN "s %" STRNAMESLEN "s %" STRNUMLEN "s"
#define PRINTFMTSTR "%s %s %s %s\n"

#define READ_ONLY "r"
#define ERROR_OPEN_INPUT_FILE -1 // ��� ������ ������ �������� �����

#define CSFILE  "data/input.txt"  // ���� �� ������� ���������
#define SCSFILE "data/output.txt" // ���� � ��������������� ������� ���������

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

    contstr **cts = NULL; // ��������� �� ������ �����, ����������� �� �����
    int resgs = 0;        // ����������, �������� ����� ����������� ����� ��� ��� ������
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

// printlst: ������� �� ����� ���������� �������
void printlst(const contstr** cslst, int lstlen)
{
    int i = 0;

    while (i < lstlen) {
        printf(PRINTFMTSTR, cslst[i]->fname, cslst[i]->lname, cslst[i]->ptnmc, cslst[i]->tnumb);
        ++i;
    }
}

// cpstrobj: ���������� ��� ������� �� ���������
int cpstrobj(const void *a, const void *b)
{
    int res = 0;

    if (res = strcmp((*((contstr**)a))->fname, (*((contstr**)b))->fname)) {
        return res;
    }
    // ���� ����� ��������� - �������� �������
    else if (res = strcmp((*((contstr**)a))->lname, (*((contstr**)b))->lname)) {
        return res;
    }
    // ���� ������� ��������� - �������� ��������
    else if (res = strcmp((*((contstr**)a))->ptnmc, (*((contstr**)b))->ptnmc)) {
        return res;
    }
    else
        return 0; // ������ ���������
}

// getContacts: ���������� ��������� �� ��������� �������� ��������� ��� NULL, � ������ ������
// �� ������ st �������� ���������� �������� ���������, ��� ����� ������
contstr** getContacts(int *res)
{
    FILE *csFile;            // ��������� �� ����� ��������� �����
    contstr **csList = NULL; // ������ ���������� �� ������� ���������, �������� ������ �����
    contstr entry;           // ������ ������ ������ ���������� �����, ��������� �� �����
    int coext = 1;           // ��������� ������ ������, ����������� ��� �������� ����� �����
    int stcnt = 0;           // ������� �����, ����������� �� �����
    int i = 0;

    if ((csFile = fopen(CSFILE, READ_ONLY)) == NULL) {
        *res = ERROR_OPEN_INPUT_FILE;
        return NULL;
    }

    // �������� ������ ��� �������� ���������� �� ������� ���������
    csList = (contstr **) malloc(ENTRYSAMO * sizeof(contstr *));

    while(fscanf(csFile, SCANFMTSTR, &entry.fname, &entry.lname, &entry.ptnmc, &entry.tnumb) != EOF) {
        if (stcnt == ENTRYSAMO * coext) {
            // ����������� ����� ���������� ������ ��� �������� ���������� �� ������� ���������
            ++coext;
            csList = (contstr **) realloc(csList, ENTRYSAMO * coext * sizeof(contstr *));
        }

        // �������� ������ ��� ������� ���������
        csList[stcnt] = (contstr*) malloc(sizeof(contstr));

        // ������� ��������� ������
        memccpy(csList[stcnt]->fname, entry.fname, (int)'\0', sizeof(csList[stcnt]->fname));
        memccpy(csList[stcnt]->lname, entry.lname, (int)'\0', sizeof(csList[stcnt]->lname));
        memccpy(csList[stcnt]->ptnmc, entry.ptnmc, (int)'\0', sizeof(csList[stcnt]->ptnmc));
        memccpy(csList[stcnt]->tnumb, entry.tnumb, (int)'\0', sizeof(csList[stcnt]->tnumb));

        ++stcnt;
    }

    if (stcnt < ENTRYSAMO * coext) {
        // ��������� ���������� ���������� ������ ��� ��������� �� ������� ��������
        csList = (contstr **) realloc(csList, stcnt * sizeof(contstr *));
    }

    *res = stcnt;
    return csList;
}
