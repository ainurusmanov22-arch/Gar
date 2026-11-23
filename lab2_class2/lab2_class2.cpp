#include <iostream>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;
class Point {
private:
    int secret;
protected:
    int id;
public:
    int x, y;

    // 1) Конструктор без параметров
    Point() : secret(0), id(0), x(0), y(0) {
        cout << " [Point] конструктор по умолчанию (" << x << "," << y << ")\n";
    }
    // 2) Конструктор с параметрами
    Point(int x, int y) : secret(42), id(1), x(x), y(y) {
        cout << " [Point] конструктор с параметрами (" << x << "," << y << ")\n";
    }
    // 3) Конструктор копирования
    Point(const Point& other) : secret(other.secret), id(other.id), x(other.x), y(other.y) {
        cout << " [Point] конструктор копирования (" << x << "," << y << ")\n";
    }
    // деструктор
    ~Point() {
        cout << " [Point] деструктор (" << x << "," << y << ")\n";
    }
    // реализация прямо в классе
    void move(int dx, int dy) {
        x += dx; y += dy;
        cout << " [Point] move -> (" << x << "," << y << ")\n";
    }
    // только обьявление 
    void print() const;
    int getSecret() const { return secret; }
};
void Point::print() const {
    cout << " [Point] print: (" << x << "," << y << ")\n";
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
#endif
    cout << "\n1.Статический объект\n";
    {
        Point p1;
        Point p2(3, 4);
        Point p3(p2);
        p2.move(1, 1);
        p2.print();
        cout << " public x = " << p2.x << ", private через метод = " << p2.getSecret() << "\n";
        p2.secret = 5; // ошибка: private
        // p2.id = 5; // ошибка: protected
        cout << " конец блока\n";
    }
    return 0;
}