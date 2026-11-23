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
    // только объявление
    void print() const;
    int getSecret() const { return secret; }
};
void Point::print() const {
    cout << " [Point] print: (" << x << "," << y << ")\n";
}
class Style {
public:
    string color;
    Style() : color("black") { cout << " [Style] конструктор по умолчанию\n"; }
    Style(string c) : color(c) { cout << " [Style] конструктор с параметром " << color << "\n"; }
    Style(const Style& o) : color(o.color) { cout << " [Style] копирование " << color << "\n"; }
    ~Style() { cout << " [Style] деструктор " << color << "\n"; }
};
class Shape {
protected:
    string name;
    Point center; //Point внутри Shape
public:
    Shape() : name("noname"), center() {
        cout << " [Shape] конструктор по умолчанию\n";
    }
    Shape(string name, int cx, int cy) : name(name), center(cx, cy) {
        cout << " [Shape] конструктор с параметрами " << name << "\n";
    }
    Shape(const Shape& o) : name(o.name), center(o.center) {
        cout << " [Shape] конструктор копирования " << name << "\n";
    }
    ~Shape() {
        cout << " [Shape] деструктор " << name << "\n";
    }
    void describe() const { //без virtual
        cout << " [Shape] describe: фигура " << name
            << " в точке (" << center.x << "," << center.y << ")\n";
    }
    void shift(int dx, int dy) { center.move(dx, dy); }
};
class Circle : public Shape {
private:
    int radius;
    Style* style;
public:
    Circle() : Shape(), radius(1), style(new Style()) {
        cout << " [Circle] конструктор по умолчанию\n";
    }
    Circle(string name, int cx, int cy, int r, string color)
        : Shape(name, cx, cy), radius(r), style(new Style(color)) {
        cout << " [Circle] конструктор с параметрами r = " << radius << "\n";
    }
    Circle(const Circle& o) : Shape(o), radius(o.radius), style(new Style(*o.style)) {
        cout << " [Circle] конструктор копирования\n";
    }
    ~Circle() {
        cout << " [Circle] деструктор " << name << "\n";
        delete style;
    }
    void describe() const {
        Shape::describe();
        cout << " [Circle] describe: радиус " << radius
            << ", цвет " << style->color << "\n";
    }
    void setColor(string c) { style->color = c; }
    void testAccess() {
        cout << " [Circle] из потомка вижу protected name=" << name << "\n";
    }
};   // <-- ЭТОЙ СТРОКИ У ТЕБЯ НЕ БЫЛО

class Parent {
public:
    Parent() { cout << " [Parent] конструктор\n"; }
    ~Parent() { cout << " [Parent] деструктор\n"; }
    void hello() { cout << " [Parent] hello\n"; }
};
class ChildEmpty : public Parent {};     // своих конструктора и деструктора нет

class Base {
public:
    Base() { cout << " [Base] конструктор\n"; }
    virtual ~Base() { cout << " [Base] деструктор\n"; }
    void onlyBase() { cout << " [Base] onlyBase\n"; }
};
class Derived : public Base {
public:
    Derived() { cout << " [Derived] конструктор\n"; }
    ~Derived() { cout << " [Derived] деструктор\n"; }
    void onlyDerived() { cout << " [Derived] onlyDerived\n"; }
};

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
        // p2.secret = 5; // ошибка: private
        // p2.id = 5; // ошибка: protected
        cout << " конец блока\n";
    }
    cout << "\n2.Динамический объект\n";
    Point* pp = new Point(10, 20); // объект в куче
    pp->move(5, 5); // через указатель: стрелка ->
    pp->print();
    delete pp; //удаляем вручную
    cout << "\n3.Присваивание: объект в объект\n";
    Point a(1, 1);
    Point b = a; //конструктор копирования
    Point c;
    c = a; //присваивание без конструктора
    a.move(100, 100);
    cout << " после изменения a: a.x=" << a.x << ", b.x=" << b.x << ", c.x=" << c.x
        << " (копии независимы)\n";

    cout << "\n4.Присваивание: указатель в указатель\n";
    Point* p = new Point(7, 7);
    Point* q = p; // два указателя на один объект
    q->move(1, 1);
    cout << " p->x=" << p->x << ", q->x=" << q->x << " (один и тот же объект!)\n";
    delete p;
    cout << "\nтест Shape\n";
    {
        Shape s("sh", 1, 2);
        s.describe();
        s.shift(1, 1);
        s.describe();
    }
    cout << "\n5.Потомок: порядок конструкторов/деструкторов\n";
    {
        Circle ci("circle1", 5, 5, 10, "red");
        ci.describe();
        ci.Shape::describe(); //явный вызов версии предка
        ci.testAccess();
        cout << "конец блока, смотри порядок деструкторов\n";
    }
    cout << "\n6.Композиция: объект vs указатель\n";
    {
        Circle c1("orig", 0, 0, 5, "blue");
        cout << "копируем круг\n";
        Circle c2(c1);
        c2.setColor("green");
        c1.describe();
        c2.describe();
        cout << "конец блока\n";
    }
    return 0;
}