#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>

#define NAMESLEN  30 // ����� �����, �������� ���, �������, ��������
#define NUMLEN    20 // ����� ������, �������� ���������� �����

#define READ_ONLY "r"
#define ERROR_OPEN_INPUT_FILE -1 // ��� ������ ������ �������� �����

#define CSFILE  "data/input.txt"  // ���� �� ������� ���������
#define SCSFILE "data/output.txt" // ���� � ��������������� ������� ���������

typedef struct scts {
    char fname[NAMESLEN]; // ������ ���
    char lname[NAMESLEN]; // ������ �������
    char ptnmc[NAMESLEN]; // ������ ��������
    char tnumb[NUMLEN];   // ������ ���������� �����
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

    if (!(cts = getContacts(&resgs)))
        return resgs;

    printf("�������� ������:\n");
    printlst(cts, resgs); // ������� �� ������� ���������� �������

    qsort(cts, resgs, sizeof(cts), cpstrobj);

    printf("\n��������������� ������:\n");
    printlst(cts, resgs); // ������� �� ������� ���������� �������

    return 0;
}

// printlst: ������� �� ����� ���������� �������
void printlst(const contstr** cslst, int lstlen)
{
    int i = 0;

    while (i < lstlen) {
        printf("%s %s %s %s\n", cslst[i]->fname, cslst[i]->lname, cslst[i]->ptnmc, cslst[i]->tnumb);
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
    int stcnt = 0;           // ������� �����, ����������� �� �����

    int i = 0;

    if ((csFile = fopen(CSFILE, READ_ONLY)) == NULL) {
        *res = ERROR_OPEN_INPUT_FILE;
        return NULL;
    }

    // �������� ������ ��� �������� ���������� �� ������� ���������
    csList = (contstr **) malloc(15 * sizeof(contstr *));

    while(fscanf(csFile, "%s %s %s %s", &entry.fname, &entry.lname, &entry.ptnmc, &entry.tnumb) != EOF) {
        // �������� ������ ��� ������� ���������
        csList[stcnt] = (contstr*) malloc(sizeof(contstr));

        // ������� ��������� ������
        memccpy(csList[stcnt]->fname, entry.fname, (int)'\0', sizeof(csList[stcnt]->fname));
        memccpy(csList[stcnt]->lname, entry.lname, (int)'\0', sizeof(csList[stcnt]->lname));
        memccpy(csList[stcnt]->ptnmc, entry.ptnmc, (int)'\0', sizeof(csList[stcnt]->ptnmc));
        memccpy(csList[stcnt]->tnumb, entry.tnumb, (int)'\0', sizeof(csList[stcnt]->tnumb));

        ++stcnt;
    }

    // ���������������� ������

    *res = stcnt;
    return csList;
}
