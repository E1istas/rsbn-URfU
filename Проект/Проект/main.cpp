#include <iostream>
#include <cstdlib> 
#include <ctime>
#include <windows.h>

using namespace std;

void Module(double slantRange, double azimuth, double xa, double ya) //реализация модуля рсбн
{
    double xb, yb, deltax, deltay, pr, pt, gt, ar, sigma;
    const double pi = 3.1415926535;
    deltax = slantRange * cos(azimuth * pi / 180); //перевод градусов в радианы
    deltay = slantRange * sin(azimuth * pi / 180);
    xb = xa + deltax; //вычисление координат самолета
    yb = ya + deltay;
    if (deltax >= 0 && deltay >= 0) //Определение румба, в котором находится самолет
    {
        cout << "Румб - СВ (1 четверть)" << endl;
    }
    else if (deltax < 0 && deltay >= 0)
    {
        cout << "Румб - ЮВ (2 четверть)" << endl;
    }
    else if (deltax < 0 && deltay < 0)
    {
        cout << "Румб - ЮЗ (3 четверть)" << endl;
    }
    else if (deltax >= 0 && deltay < 0)
    {
        cout << "Румб - СЗ (4 четверть)" << endl;
    }
    else return;
    
    cout << "Координаты самолета: X = " << xb <<", Y = "<< yb << endl <<endl;
    cout << "Введите коэффициенты мощности сигнала через пробел в такое порядке - Pt, Gt, Ar, sigma" << endl; //расчет мощности сигнала
    cin >> pt >> gt >> ar >> sigma;
    pr = (pt * gt * ar * sigma) / (pow(4 * pi, 2) * pow(slantRange, 4));
    cout << "Мощность сигнала (Pr) = " << pr << endl << endl;
}
double* beaconSwapped() //функция для смены радиомаяка
{
    static double beacxy[2];
    srand(time(0));
    double x = 1000;
    for (int i = 0; i < 2; i++)
    {
        beacxy[i] = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / x)); //задание случайных координат маяку
    }
    return beacxy;
}
void Beacon(int slantRange, int azimuth) //реализация радиомаяка
{
    cout << endl;
    double* beacxy = beaconSwapped(); //вызов функции для задания координат маяка
    cout << "Наклонная дистанция = " << slantRange <<" км"<< endl;
    cout << "Азимут = " << azimuth <<" градусов"<< endl;
    cout << "Координаты радиомаяка: X = " << beacxy[0]<< ", Y = "<< beacxy[1] << endl;
    Module(slantRange, azimuth, beacxy[0], beacxy[1]); //передача данных модулю рсбн
}
void Time(int n) //функция для отображения времени работы программы
{
    int start, end, q = 5;
    n = n * 20;
    for (int i = 0; i < 4; i++)
    {
        start = clock();
        Sleep(5000);
        end = clock();
        if (((end - start) / CLK_TCK) == 5)
            cout << "Время работы программы - " << q * (i+1) + n << " сек" << endl;
    }
}
int main()
{
    setlocale(LC_ALL, "Russian");
    cout << "Все координаты выражены в километрах (км)" << endl;
    srand(time(0));
    double slantRange = rand() % 381; //рандомизация наклонной дистанции
    double azimuth = rand() % 360; //рандомизация азимута
    for(int i = 0;i < 100;i++) //долгий цикл для реализации постоянной смены маяка каждые 20 сек
    {
        Beacon(slantRange, azimuth);
        Time(i);
        cout << endl << "--Смена радиомаяка--" << endl;
    }
    system("pause");
    return 0;
}