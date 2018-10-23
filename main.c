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

#define READ_ONLY  "r"
#define FILE_WRITE "w"
#define ERROR_OPEN_INPUT_FILE -1 // ��� ������ �������� �������� �����
#define ERROR_OPEN_OUTPUT_FILE -2 // ��� ������ �������� ��������� �����

#define CSFILE  "data/input.txt"  // ���� �� ������� ���������
#define SCSFILE "data/output.txt" // ���� � ��������������� ������� ���������

typedef struct scts {
    char lname[NAMESLEN]; // Last name
    char fname[NAMESLEN]; // First name
    char ptnmc[NAMESLEN]; // Patronymic
    char tnumb[NUMLEN];   // Telephone number
} contstr;

contstr** getContacts(int *res);
int cpstrobj(const void *a, const void *b);
int saveList(const contstr** cslst, int lstlen);

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    contstr **cts = NULL; // ��������� �� ������ �����, ����������� �� �����
    int resgs = 0;        // ����� ����������� ����� ��� ��� ������
    int wstat = 0;        // ����� ���������� � ���� ����� ��� ��� ������
    int i = 0;

    if (!(cts = getContacts(&resgs))) {
        return resgs;
    }

    qsort(cts, resgs, sizeof(cts), cpstrobj);

    if ((wstat = saveList(cts, resgs)) > 0) {
        printf("Sorted list successfully written to file");
    }
    else {
        return wstat;
    }

    return 0;
}

// cpstrobj: ���������� ��� ������� �� ���������
int cpstrobj(const void *a, const void *b)
{
    int res = 0;

    if (res = strcmp((*((contstr**)a))->lname, (*((contstr**)b))->lname)) {
        return res;
    }
    // ���� ����� ��������� - �������� �������
    else if (res = strcmp((*((contstr**)a))->fname, (*((contstr**)b))->fname)) {
        return res;
    }
    // ���� ������� ��������� - �������� ��������
    else if (res = strcmp((*((contstr**)a))->ptnmc, (*((contstr**)b))->ptnmc)) {
        return res;
    }
    else
        return 0; // ������ ���������
}

int saveList(const contstr** cslst, int lstlen)
{
    FILE *csFile; // ��������� �� ����� ��������� �����
    int i = 0;

    if ((csFile = fopen(SCSFILE, FILE_WRITE)) == NULL) {
        return ERROR_OPEN_OUTPUT_FILE;
    }

    while (i < lstlen) {
        fprintf(csFile, PRINTFMTSTR, cslst[i]->lname, cslst[i]->fname, cslst[i]->ptnmc, cslst[i]->tnumb);
        ++i;
    }

    fclose(csFile);

    return i;

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

    while(fscanf(csFile, SCANFMTSTR, &entry.lname, &entry.fname, &entry.ptnmc, &entry.tnumb) != EOF) {
        if (stcnt == ENTRYSAMO * coext) {
            // ����������� ����� ���������� ������ ��� �������� ���������� �� ������� ���������
            ++coext;
            csList = (contstr **) realloc(csList, ENTRYSAMO * coext * sizeof(contstr *));
        }

        // �������� ������ ��� ������� ���������
        csList[stcnt] = (contstr*) malloc(sizeof(contstr));

        // ������� ��������� ������
        memccpy(csList[stcnt]->lname, entry.lname, (int)'\0', sizeof(csList[stcnt]->lname));
        memccpy(csList[stcnt]->fname, entry.fname, (int)'\0', sizeof(csList[stcnt]->fname));
        memccpy(csList[stcnt]->ptnmc, entry.ptnmc, (int)'\0', sizeof(csList[stcnt]->ptnmc));
        memccpy(csList[stcnt]->tnumb, entry.tnumb, (int)'\0', sizeof(csList[stcnt]->tnumb));

        ++stcnt;
    }

    if (stcnt < ENTRYSAMO * coext) {
        // ��������� ���������� ���������� ������ ��� ��������� �� ������� ��������
        csList = (contstr **) realloc(csList, stcnt * sizeof(contstr *));
    }

    fclose(csFile);

    *res = stcnt;
    return csList;
}
