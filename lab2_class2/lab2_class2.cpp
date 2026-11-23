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
    Point center; // композиция объектом: Point внутри Shape
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
    void describe() const { // без virtual
        cout << " [Shape] describe: фигура " << name
            << " в точке (" << center.x << "," << center.y << ")\n";
    }
    void shift(int dx, int dy) { center.move(dx, dy); }
};

class Circle : public Shape {
private:
    int radius;
    Style* style; // композиция указателем
public:
    Circle() : Shape(), radius(1), style(new Style()) {
        cout << " [Circle] конструктор по умолчанию\n";
    }
    Circle(string name, int cx, int cy, int r, string color)
        : Shape(name, cx, cy), radius(r), style(new Style(color)) {
        cout << " [Circle] конструктор с параметрами r = " << radius << "\n";
    }
    // глубокое копирование: создаём новый Style
    Circle(const Circle& o) : Shape(o), radius(o.radius), style(new Style(*o.style)) {
        cout << " [Circle] конструктор копирования\n";
    }
    ~Circle() {
        cout << " [Circle] деструктор " << name << "\n";
        delete style;
    }
    void describe() const {
        Shape::describe(); // вызов метода предка
        cout << " [Circle] describe: радиус " << radius
            << ", цвет " << style->color << "\n";
    }
    void setColor(string c) { style->color = c; }
    void testAccess() {
        cout << " [Circle] из потомка вижу protected name=" << name << "\n";
    }
};

// Предок с конструктором и деструктором, потомок без них
class Parent {
public:
    Parent() { cout << " [Parent] конструктор\n"; }
    ~Parent() { cout << " [Parent] деструктор\n"; }
    void hello() { cout << " [Parent] hello\n"; }
};
class ChildEmpty : public Parent {}; // своих конструктора и деструктора нет

// Base и Derived: по три конструктора у каждого, виртуальный деструктор
class Base {
protected:
    int value;
public:
    Base() : value(0) { cout << " [Base] конструктор\n"; }
    Base(int v) : value(v) { cout << " [Base] конструктор с параметром " << v << "\n"; }
    Base(const Base& o) : value(o.value) { cout << " [Base] конструктор копирования\n"; }
    virtual ~Base() { cout << " [Base] деструктор\n"; }
    void onlyBase() { cout << " [Base] onlyBase\n"; }
};
class Derived : public Base {
public:
    Derived() : Base() { cout << " [Derived] конструктор\n"; }
    Derived(int v) : Base(v) { cout << " [Derived] конструктор с параметром\n"; }
    Derived(const Derived& o) : Base(o) { cout << " [Derived] конструктор копирования\n"; }
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
        // p2.id = 5;     // ошибка: protected
        cout << " конец блока\n";
    }

    cout << "\n2.Динамический объект\n";
    Point* pp = new Point(10, 20); // объект в куче
    pp->move(5, 5);                // через указатель: стрелка ->
    pp->print();
    delete pp;                     // удаляем вручную

    cout << "\n3.Присваивание: объект в объект\n";
    Point a(1, 1);
    Point b = a;                   // конструктор копирования
    Point c;
    c = a;                         // присваивание, конструктор не вызывается
    a.move(100, 100);              // меняем только a
    cout << " после изменения a: a.x=" << a.x << ", b.x=" << b.x << ", c.x=" << c.x << "\n";
    b.move(5, 5);                  // теперь меняем b
    cout << " после изменения b: a.x=" << a.x << ", b.x=" << b.x << ", c.x=" << c.x
        << " (копии независимы)\n";

    cout << "\n4.Присваивание: указатель в указатель\n";
    Point* p = new Point(7, 7);
    Point* q = p;                  // два указателя на один объект
    q->move(1, 1);                 // меняем через q
    cout << " после изменения через q: p->x=" << p->x << ", q->x=" << q->x << "\n";
    p->move(10, 10);               // меняем через p
    cout << " после изменения через p: p->x=" << p->x << ", q->x=" << q->x
        << " (один и тот же объект!)\n";
    delete p;                      // удаляем один раз

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
        ci.Shape::describe(); // явный вызов версии предка
        ci.testAccess();
        cout << " конец блока, смотри порядок деструкторов\n";
    }

    cout << "\n6.Композиция: объект vs указатель\n";
    {
        Circle c1("orig", 0, 0, 5, "blue");
        cout << " копируем круг\n";
        Circle c2(c1);
        c2.setColor("green");
        c1.describe();
        c2.describe();
        cout << " конец блока\n";
    }

    cout << "\n6.1.Динамический потомок и конструктор по умолчанию\n";
    Circle* pc = new Circle(); // динамически, конструктор по умолчанию
    pc->describe();
    delete pc;                 // деструкторы: Circle, Style, Shape, Point

    cout << "\n7.Предок с ctor/dtor, потомок без\n";
    {
        Parent par;
        ChildEmpty ch;
        ch.hello();
    }

    cout << "\n8.Указатель на базовый класс\n";
    cout << " new Base() \n";
    Base* b1 = new Base();     // создаётся только Base
    b1->onlyBase();
    delete b1;                 // вызовется только ~Base
    cout << "  new Derived() в указателе Base* \n";
    Base* bp = new Derived();  // создаётся Derived, адрес в Base*
    bp->onlyBase();            // можно
    // bp->onlyDerived();      // ошибка: через Base* виден только Base
    static_cast<Derived*>(bp)->onlyDerived(); // можно, если привести тип
    delete bp;                 // из-за virtual вызовутся ~Derived и ~Base

    cout << "\n9.Срезка (slicing)\n";
    {
        Base sliced = Derived(); // копируется только часть Base
        cout << " sliced живёт, это уже просто Base\n";
    }

    cout << "\nКОНЕЦ main\n";
    return 0;
}