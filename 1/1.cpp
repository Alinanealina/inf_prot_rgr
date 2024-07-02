#include <iostream>
#include <time.h>
#include <fstream>
#pragma warning(disable : 4996)
using namespace std;
const int N = 20;
void print_GH(int n, int** G = NULL, int* H = NULL)
{
    int i, j;
    if (G != NULL)
    {
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
                cout << " " << G[i][j];
            cout << endl;
        }
    }
    if (H != NULL)
    {
        for (i = 0; i < n; i++)
        {
            cout << " " << H[i];
            if (i < n - 1)
                cout << " -";
        }
    }
    cout << endl;
}

void shuf(int n, int** G, int* H, int* A, int* B)
{
    int i, j, a, b = -1, obm;
    for (i = 0; i < n; i++)
    {
        a = rand() % n;
        while ((b < 0) || (b == a))
            b = rand() % n;
        for (j = 0; j < n; j++)
        {
            obm = G[a][j];
            G[a][j] = G[b][j];
            G[b][j] = obm;

            if (H[j] == a + 1)
                H[j] = b + 1;
            else if (H[j] == b + 1)
                H[j] = a + 1;
        }
        for (j = 0; j < n; j++)
        {
            obm = G[j][a];
            G[j][a] = G[j][b];
            G[j][b] = obm;
        }
        A[n - 1 - i] = a;
        B[n - 1 - i] = b;
    }
}

void goback(int n, int** G, int* A, int* B)
{
    int i, j, obm;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            obm = G[A[i]][j];
            G[A[i]][j] = G[B[i]][j];
            G[B[i]][j] = obm;
        }
        for (j = 0; j < n; j++)
        {
            obm = G[j][A[i]];
            G[j][A[i]] = G[j][B[i]];
            G[j][B[i]] = obm;
        }
    }
}

bool check_cycle(int n, int* H, int** G)
{
    int i, j;
    for (i = 1; i < n; i++)
    {
        for (j = 0; j < i; j++)
        {
            if (H[i] == H[j])
                return false;
        }
    }

    for (i = 0; i < n - 1; i++)
    {
        if ((G[H[i] - 1][H[i + 1] - 1] != 1) || ((G[H[i + 1] - 1][H[i] - 1] != 1)))
            return false;
    }
    return true;
}
bool check_gr(int n, int** G, int** G2)
{
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (G[i][j] != G2[i][j])
                return false;
        }
    }
    return true;
}

void gam_cycle(int n, int** G, int* H)
{
    int i, j, k, * A, * B, ** G2, * H2, a, c = 0;
    A = new int[n];
    B = new int[n];
    H2 = new int[n];
    G2 = new int* [n];
    for (i = 0; i < n; i++)
        G2[i] = new int[n];

    for (k = 0; k < N; k++)
    {
        cout << " Проверка " << k + 1 << ":\n";
        for (i = 0; i < n; i++)
        {
            H2[i] = H[i];
            for (j = 0; j < n; j++)
                G2[i][j] = G[i][j];
        }

        cout << "\n А делает перестановку в графе и в Гамильтонове цикле:\n";
        shuf(n, G2, H2, A, B);
        print_GH(n, G2);

        a = rand() % 2;
        cout << "\n Б: " << a << ", получает от А граф с перестановкой и ";

        if (a == 0)
        {
            cout << "саму перестановку и вычисляет граф:\n";
            goback(n, G2, A, B);
            print_GH(n, G2);

            cout << "\n Б проверил: ";
            if (check_gr(n, G, G2))
            {
                cout << "это действительно изоморфный граф.";
                c++;
            }
            else
                cout << "это не изоморфный граф.";
        }
        else
        {
            cout << "Гамильтонов цикл изоморфного графа:\n";
            print_GH(n, NULL, H2);

            cout << "\n Б проверил: ";
            if (check_cycle(n, H2, G2))
            {
                cout << "это действительно Гамильтонов цикл.";
                c++;
            }
            else
                cout << "это не Гамильтонов цикл.";
        }
        cout << "\n_______________________\n";
    }
    cout << " Проверок пройдено: " << c << "/" << N;

    delete[] A;
    delete[] B;
    delete[] H2;
    for (i = 0; i < n; i++)
        delete[] G2[i];
}

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(time(0));
    int i, j, n, m, ** G, * H, a, b;
    char str[256];
    ifstream f("..\\..\\file.txt");
    f >> str;
    sscanf(str, "%d,%d", &n, &m);

    G = new int* [n];
    for (i = 0; i < n; i++)
    {
        G[i] = new int[n];
        for (j = 0; j < n; j++)
            G[i][j] = 0;
    }
    for (i = 0; i < m; i++)
    {
        f >> str;
        sscanf(str, "%d,%d", &a, &b);
        G[a - 1][b - 1] = 1;
    }

    H = new int[n];
    for (i = 0; i < n; i++)
    {
        f >> str;
        H[i] = atoi(str);
    }
    f.close();

    print_GH(n, G);
    gam_cycle(n, G, H);

    for (i = 0; i < n; i++)
        delete[] G[i];
    delete[] H;
    return 0;
}