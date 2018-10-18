#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>

#define READ_ONLY "r"
#define ERROR_OPEN_INPUT_FILE -1 // ��� ������ ������ �������� �����

#define CSFILE  "data/input.txt"  // ���� �� ������� ���������
#define SCSFILE "data/output.txt" // ���� � ��������������� ������� ���������

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

    contstr **cts = NULL; // ��������� �� ������ �����, ����������� �� �����
    int resgs = 0;     // ����������, �������� ����� ����������� ����� ��� ��� ������
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
        printf("%3d %d\n", i, cslst[i]->n);
        ++i;
    }
}

// cpstrobj: ���������� ��� ������� �� ���������
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

// getContacts: ���������� ��������� �� ��������� �������� ��������� ��� NULL, � ������ ������
// �� ������ st �������� ���������� �������� ���������, ��� ����� ������
contstr** getContacts(int *res)
{
    FILE *csFile;            // ��������� �� ����� ��������� �����
    contstr **csList = NULL; // ������ ���������� �� ������� ���������, �������� ������ �����
    int enitem = 0;          // ������ ����� ������ ���������� �����
    int stcnt = 0;           // ������� �����, ����������� �� �����
    int i = 0;

    if ((csFile = fopen(CSFILE, READ_ONLY)) == NULL) {
        *res = ERROR_OPEN_INPUT_FILE;
        return NULL;
    }

    // �������� ������ ��� �������� ���������� �� ������� ���������
    csList = (contstr **) malloc(15 * sizeof(contstr *));

    while(fscanf(csFile, "%d", &enitem) != EOF) {
        // �������� ������ ��� ������� ���������
        csList[stcnt] = (contstr*) malloc(sizeof(contstr));
        csList[stcnt]->n = enitem;
        ++stcnt;
    }

    // ���������������� ������

    *res = stcnt;
    return csList;
}
