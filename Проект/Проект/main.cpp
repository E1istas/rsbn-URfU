#include <iostream>
#include <cstdlib> 
#include <ctime>
#include <windows.h>

using namespace std;

void Module(double slantRange, double azimuth, double xa, double ya) //���������� ������ ����
{
    double xb, yb, deltax, deltay, pr, pt, gt, ar, sigma;
    const double pi = 3.1415926535;
    deltax = slantRange * cos(azimuth * pi / 180); //������� �������� � �������
    deltay = slantRange * sin(azimuth * pi / 180);
    xb = xa + deltax; //���������� ��������� ��������
    yb = ya + deltay;
    if (deltax >= 0 && deltay >= 0) //����������� �����, � ������� ��������� �������
    {
        cout << "���� - �� (1 ��������)" << endl;
    }
    else if (deltax < 0 && deltay >= 0)
    {
        cout << "���� - �� (2 ��������)" << endl;
    }
    else if (deltax < 0 && deltay < 0)
    {
        cout << "���� - �� (3 ��������)" << endl;
    }
    else if (deltax >= 0 && deltay < 0)
    {
        cout << "���� - �� (4 ��������)" << endl;
    }
    else return;
    
    cout << "���������� ��������: X = " << xb <<", Y = "<< yb << endl <<endl;
    cout << "������� ������������ �������� ������� ����� ������ � ����� ������� - Pt, Gt, Ar, sigma" << endl; //������ �������� �������
    cin >> pt >> gt >> ar >> sigma;
    pr = (pt * gt * ar * sigma) / (pow(4 * pi, 2) * pow(slantRange, 4));
    cout << "�������� ������� (Pr) = " << pr << endl << endl;
}
double* beaconSwapped() //������� ��� ����� ����������
{
    static double beacxy[2];
    srand(time(0));
    double x = 1000;
    for (int i = 0; i < 2; i++)
    {
        beacxy[i] = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / x)); //������� ��������� ��������� �����
    }
    return beacxy;
}
void Beacon(int slantRange, int azimuth) //���������� ����������
{
    cout << endl;
    double* beacxy = beaconSwapped(); //����� ������� ��� ������� ��������� �����
    cout << "��������� ��������� = " << slantRange <<" ��"<< endl;
    cout << "������ = " << azimuth <<" ��������"<< endl;
    cout << "���������� ����������: X = " << beacxy[0]<< ", Y = "<< beacxy[1] << endl;
    Module(slantRange, azimuth, beacxy[0], beacxy[1]); //�������� ������ ������ ����
}
void Time(int n) //������� ��� ����������� ������� ������ ���������
{
    int start, end, q = 5;
    n = n * 20;
    for (int i = 0; i < 4; i++)
    {
        start = clock();
        Sleep(5000);
        end = clock();
        if (((end - start) / CLK_TCK) == 5)
            cout << "����� ������ ��������� - " << q * (i+1) + n << " ���" << endl;
    }
}
int main()
{
    setlocale(LC_ALL, "Russian");
    cout << "��� ���������� �������� � ���������� (��)" << endl;
    srand(time(0));
    double slantRange = rand() % 381; //������������ ��������� ���������
    double azimuth = rand() % 360; //������������ �������
    for(int i = 0;i < 100;i++) //������ ���� ��� ���������� ���������� ����� ����� ������ 20 ���
    {
        Beacon(slantRange, azimuth);
        Time(i);
        cout << endl << "--����� ����������--" << endl;
    }
    system("pause");
    return 0;
}