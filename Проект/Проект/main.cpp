#include <iostream>
#include <cstdlib> 
#include <ctime>
#include <windows.h>

using namespace std;
//������ ���������: ��� ������� ����� �������� ��������� ������������ ��������� �� 0 �� 379 (�������� ������������ ��������� ���������� � 380 ��)
//� ����� ������ �� 0 �� 359. ������ �������� ������������ ������� �� 0 �� 15 �� � ������� ���������� �� ���������� ���� ���������. ��� ������
//������������ � ������� firstBeacon ���� ���� ���, ��� ���� ����� �������� ��������� ���������� ������� ����� � ��������� � ������� Module.
//����� ������������� ��������� ��������� �� ������� �������� �� ������� ������ ����� ��������� � ������ � ������ ������������ ���������.
//��������� ���������� deltax � deltay ��� ����������� ��������� �������� � ������� ������ ������������� ������. ��� ������ ��������� �� �����.
//������ ���������� ������ �������� ������� (��), �������� �������� � ���������� ������������. ���� ������� Time �����������������, ���������� 
//��������� ����������� �������, ��� ������ 20 ������ ����������� ��� ������� nextBeacon, �� ���� ����� �����������. nextBeacon - ��������������
//�������, ��������� �� ��� �������, ��� ����� �������� ����������� ����� ������ �������� ����������, ��������� �� ���������� ���������� ������
//���� �������� ���� ����� ����������, � ��� ��� �� ����������. ������� firstBeacon ������ ���������� ��������, ������� ����� ������������ � 
//� ������� nextBeacon �� ������� ���������, ����� ������� ���������� ���������� ���������� ��������� �������� ��� ������ ������ �������������
//������
const double pi = 3.1415926535;
double xb, yb, zb; //���������� �������� ������� ����������� �����������, ����� ������������ ����� ������ ����� �����

void Module(double proecRange, double azimuth, double xa, double ya, double za) //���������� ������ ����
{
    double deltax, deltay, pr, pt, gt, ar, sigma,slantRange;
    slantRange = sqrt(pow(proecRange,2)+pow(zb-za,2)); //��������� ��������� ��������� �� ��������
    deltax = slantRange * cos(azimuth * pi / 180); //������ ���������� x � y
    deltay = slantRange * sin(azimuth * pi / 180);
    xb = xa + deltax; //���������� ��������� ��������
    yb = ya + deltay;
    cout << "��������� ��������� = " << slantRange << " ��" << endl; //����� �������� �� �������
    cout << "������������ ��������� = " << proecRange << " ��" << endl;
    cout << "������ = " << (int)azimuth << " ��������" << endl;
    cout << "deltax = " << deltax << endl;
    cout << "deltay = " << deltay << endl;
    cout << "���������� ����������: X = " << xa << ", Y = " << ya << ", Z = " << za << endl;
    cout << "���������� ��������: X = " << xb << ", Y = " << yb << ", Z = " << zb << endl;
    if (deltax >= 0 && deltay >= 0) //����������� �����, � ������� ��������� �������
    {
        cout << "���� - �� (1 ��������)" << endl << endl;
    }
    else if (deltax < 0 && deltay >= 0)
    {
        cout << "���� - �� (2 ��������)" << endl << endl;
    }
    else if (deltax < 0 && deltay < 0)
    {
        cout << "���� - �� (3 ��������)" << endl << endl;
    }
    else if (deltax >= 0 && deltay < 0)
    {
        cout << "���� - �� (4 ��������)" << endl << endl;
    }
    else return;
    cout << "������� ������������ �������� ������� � ������� �� ����� ������ � ����� ������� - Pt, Gt, Ar, sigma" << endl; //������ �������� �������
    cin >> pt >> gt >> ar >> sigma;
    pr = (pt * gt * ar * sigma) / (pow(4 * pi, 2) * pow(slantRange, 4));
    cout << "�������� ������� (Pr) = " << pr << " ��" << endl << endl;

}
double* beaconSwapped() //������� ��� ������� ��������� ��������� ����������
{
    static double beacxy[3];
    double maxxy = 100, minxy = -100, maxz = 1;
    for (int i = 0; i < 2; i++)
    {
        beacxy[i] = minxy + static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / (maxxy-minxy))); 
    }
    beacxy[2] = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / maxz)); //������ ������������ ����� �� ������ 1 ��
    return beacxy;
}
void firstBeacon(double proecRange, double azimuth) //���������� ����������
{
    cout << endl;
    double* beacxy = beaconSwapped(); //����� ������� ��� ��������� ��������� �����
    Module(proecRange, azimuth, beacxy[0], beacxy[1], beacxy[2]); //�������� ������ ������ ����
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
int getQuart(double xb, double yb, double zb, double xa, double ya, double za) //������� ��� ����, ����� ��������� ������� ��������, � ������� ��������� �������
{                                                                              //P.S. ������� �������� ���� ����� ��������� ������� ���������� ����, ������� ���������� ��� �������
    double redX2 = xb - xa;  //������������� �����������, ��� ������ ��������� � ����� ����������, � ������� "���������" ������������ ���, ��� ��������� ����� ������� �������� � ������������ ������ �������� � �������
    double redY2 = yb - ya;
    if (redX2 && redY2 < 0) //������ ��� 1 � 2 �������� �� �����, �� � ���� ������ ������ �� ������������
    {
        return 180; //������������ 180 ��������, ���� ������� ��������� � ������� ��������
    }
    else if (redX2 > 0 && redY2 < 0)
    {
        return 270; //���������� ���������� 270, ���� ��������� � 4 ��������
    }
    else return 0;
}
void nextBeacon(double xb, double yb, double zb) //����������� ����� ����� �������
{
    cout << endl;
    double* beacxy = beaconSwapped(); //��������� ����� ��������� �����
    double blueX2, blueX1, blueY2, blueY1, redX2, redX1, redY2, redY1,x1,x2,y1,y2,cosa,proecRange,modab,azimuth;
    blueX1 = beacxy[0]; //a=(x1,y1),b=(x2,y2)
    blueX2 = blueX1 + 200; //������ ������������ ������ ������, ����� ������������ ��� ��� �������� ����
    blueY1 = beacxy[1];  
    blueY2 = blueY1;
    redX1 = blueX1; //������ ������������ ������ - ������������ ���������
    redX2 = xb;
    redY1 = blueY1;
    redY2 = yb;
    x1 = blueX2 - blueX1;
    y1 = blueY2 - blueY1;
    x2 = redX2 - redX1;
    y2 = redY2 - redY1;
    modab = sqrt(x1 * x1 + y1 * y1)* sqrt(x2 * x2 + y2 * y2);
    cosa = (x1 * x2 + y1 * y2) / modab; 
    azimuth = (acos(cosa) * 180 / pi)+ getQuart(xb, yb, zb, beacxy[0], beacxy[1], beacxy[2]);//�� ������� ���� ����� ��������� ������� ������ 
    proecRange = sqrt(pow(redX2 - redX1, 2) + pow(redY2 - redY1, 2)); //������� ������������ ��������� �� ������� ����� �������
    Module(proecRange, azimuth, beacxy[0], beacxy[1], beacxy[2]); //�������� ������ ������ ����
}
int main()
{
    setlocale(LC_ALL, "Russian");
    cout << "��� ���������� �������� � ���������� (��)" << endl;
    srand((unsigned)time(0)); 
    double maxRange = 379, maxz = 15;
    double proecRange = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / maxRange)); //������������ ������������ ��������� ��� ������� �����
    double azimuth = rand() % 360; //������������ ������� ��� ������� �����
    zb = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / maxz)); //������ ������ �������� ��������� �� 0 �� 15 ��
    firstBeacon(proecRange, azimuth);
    //������� ������� ���� ����� Time ��� ��������� ����������� �������
    //Time(0); 
    cout << endl << "--����� ����������--" << endl;
    for(int i = 0;i < 100;i++) //������ ���� ��� ���������� ���������� ����� ����� ������ 20 ���
    {
        nextBeacon(xb, yb, zb);
        //Time(i+1);
        cout << endl << "--����� ����������--" << endl;
    }
    system("pause");
    return 0;
}