#include <iostream>
#include <cstdlib> 
#include <ctime>
#include <windows.h>

using namespace std;
//Логика программы: для первого маяка задается случайная проекционная дистанция от 0 до 379 (учитывая максимальное наклонное расстояние в 380 км)
//и также азимут от 0 до 359. Высота самолета генерируется вначале от 0 до 15 км и остется постоянной на протяжение всей программы. Эти данные
//отправляются в функцию firstBeacon лишь один раз, для того чтобы получить случайные координаты первого маяка и отправить в функцию Module.
//Далее высчитывается наклонная дальность по теореме Пифагора из разницы высоты между самолетом и маяком и данной проекционной дистанции.
//Считаются приращения deltax и deltay для определения координат самолета с помощью прямой геодезической задачи. Все данные выводятся на экран.
//Дальше происходит расчет мощности сигнала (Вт), учитывая введеные с клавиатуры коэффициенты. Если функции Time раскомментированы, происходит 
//симуляция прохождения времени, где каждые 20 секунд запускается уже функция nextBeacon, то есть смена радиомаяков. nextBeacon - альтернативная
//функция, сделанная по той причине, что одной функцией реализовать смену маяков попросту невозможно, поскольку со случайными значениями коорди
//наты самолета тоже будут случайными, а это нас не устраивает. Функция firstBeacon выдает координаты самолета, которые позже используются в 
//в функции nextBeacon по другому алгоритму, таким образом происходит постоянный перерасчет координат самолета при помощи прямой геодезической
//задачи
const double pi = 3.1415926535;
double xb, yb, zb; //координаты самолета сделали глобальными переменными, чтобы использовать после первой смены маяка

void Module(double proecRange, double azimuth, double xa, double ya, double za) //реализация модуля рсбн
{
    double deltax, deltay, pr, pt, gt, ar, sigma,slantRange;
    slantRange = sqrt(pow(proecRange,2)+pow(zb-za,2)); //наклонная дальность считается по Пифагору
    deltax = slantRange * cos(azimuth * pi / 180); //расчет приращения x и y
    deltay = slantRange * sin(azimuth * pi / 180);
    xb = xa + deltax; //вычисление координат самолета
    yb = ya + deltay;
    cout << "Наклонная дальность = " << slantRange << " км" << endl; //вывод значений на консоль
    cout << "Проекционная дистанция = " << proecRange << " км" << endl;
    cout << "Азимут = " << (int)azimuth << " градусов" << endl;
    cout << "deltax = " << deltax << endl;
    cout << "deltay = " << deltay << endl;
    cout << "Координаты радиомаяка: X = " << xa << ", Y = " << ya << ", Z = " << za << endl;
    cout << "Координаты самолета: X = " << xb << ", Y = " << yb << ", Z = " << zb << endl;
    if (deltax >= 0 && deltay >= 0) //Определение румба, в котором находится самолет
    {
        cout << "Румб - СВ (1 четверть)" << endl << endl;
    }
    else if (deltax < 0 && deltay >= 0)
    {
        cout << "Румб - ЮВ (2 четверть)" << endl << endl;
    }
    else if (deltax < 0 && deltay < 0)
    {
        cout << "Румб - ЮЗ (3 четверть)" << endl << endl;
    }
    else if (deltax >= 0 && deltay < 0)
    {
        cout << "Румб - СЗ (4 четверть)" << endl << endl;
    }
    else return;
    cout << "Введите коэффициенты мощности сигнала в системе СИ через пробел в такое порядке - Pt, Gt, Ar, sigma" << endl; //расчет мощности сигнала
    cin >> pt >> gt >> ar >> sigma;
    pr = (pt * gt * ar * sigma) / (pow(4 * pi, 2) * pow(slantRange, 4));
    cout << "Мощность сигнала (Pr) = " << pr << " Вт" << endl << endl;

}
double* beaconSwapped() //функция для задания случайных координат радиомаяку
{
    static double beacxy[3];
    double maxxy = 100, minxy = -100, maxz = 1;
    for (int i = 0; i < 2; i++)
    {
        beacxy[i] = minxy + static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / (maxxy-minxy))); 
    }
    beacxy[2] = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / maxz)); //высота расположения маяка не больше 1 км
    return beacxy;
}
void firstBeacon(double proecRange, double azimuth) //реализация радиомаяка
{
    cout << endl;
    double* beacxy = beaconSwapped(); //вызов функции для получения координат маяка
    Module(proecRange, azimuth, beacxy[0], beacxy[1], beacxy[2]); //передача данных модулю рсбн
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
int getQuart(double xb, double yb, double zb, double xa, double ya, double za) //функция для того, чтобы корректно оценить четверть, в которой находится самолет
{                                                                              //P.S. формула подсчета угла между векторами считает наименьший угол, поэтому используем эту функцию
    double redX2 = xb - xa;  //математически представлем, что начало координат в точке радиомаяка, а самолет "смещается" относительно них, это позволяет точно оценить четверть и приплюсовать нужное значение к азимуту
    double redY2 = yb - ya;
    if (redX2 && redY2 < 0) //расчет для 1 и 2 четверти не нужен, тк в этом случае ничего не прибавляется
    {
        return 180; //прибавляется 180 градусов, если самолет находится в третьей четверти
    }
    else if (redX2 > 0 && redY2 < 0)
    {
        return 270; //аналогично прибавляем 270, если находится в 4 четверти
    }
    else return 0;
}
void nextBeacon(double xb, double yb, double zb) //последующие маяки после первого
{
    cout << endl;
    double* beacxy = beaconSwapped(); //получение новых координат маяка
    double blueX2, blueX1, blueY2, blueY1, redX2, redX1, redY2, redY1,x1,x2,y1,y2,cosa,proecRange,modab,azimuth;
    blueX1 = beacxy[0]; //a=(x1,y1),b=(x2,y2)
    blueX2 = blueX1 + 200; //строим направляющий вектор севера, чтобы использовать его при подсчете угла
    blueY1 = beacxy[1];  
    blueY2 = blueY1;
    redX1 = blueX1; //второй направляющий вектор - проекционная дистанция
    redX2 = xb;
    redY1 = blueY1;
    redY2 = yb;
    x1 = blueX2 - blueX1;
    y1 = blueY2 - blueY1;
    x2 = redX2 - redX1;
    y2 = redY2 - redY1;
    modab = sqrt(x1 * x1 + y1 * y1)* sqrt(x2 * x2 + y2 * y2);
    cosa = (x1 * x2 + y1 * y2) / modab; 
    azimuth = (acos(cosa) * 180 / pi)+ getQuart(xb, yb, zb, beacxy[0], beacxy[1], beacxy[2]);//по формуле угла между векторами находим азимут 
    proecRange = sqrt(pow(redX2 - redX1, 2) + pow(redY2 - redY1, 2)); //считаем проекционную дистанцию по формуле длины вектора
    Module(proecRange, azimuth, beacxy[0], beacxy[1], beacxy[2]); //передача данных модулю рсбн
}
int main()
{
    setlocale(LC_ALL, "Russian");
    cout << "Все координаты выражены в километрах (км)" << endl;
    srand((unsigned)time(0)); 
    double maxRange = 379, maxz = 15;
    double proecRange = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / maxRange)); //рандомизация проекционной дистанции для первого маяка
    double azimuth = rand() % 360; //рандомизация азимута для первого маяка
    zb = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / maxz)); //делаем высоту самолета случайной от 0 до 15 км
    firstBeacon(proecRange, azimuth);
    //удалите двойной слеш перед Time для симуляции прохождения времени
    //Time(0); 
    cout << endl << "--Смена радиомаяка--" << endl;
    for(int i = 0;i < 100;i++) //долгий цикл для реализации постоянной смены маяка каждые 20 сек
    {
        nextBeacon(xb, yb, zb);
        //Time(i+1);
        cout << endl << "--Смена радиомаяка--" << endl;
    }
    system("pause");
    return 0;
}