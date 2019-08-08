#include <math.h>
#include <functional>
#include <iostream>
using std::cout;
using std::endl;

class Figure{
public:
    using DisplayCallback=std::function<void()>;//定义函数类型
    using AreaCallback=std::function<double()>;//定义函数类型

    void setDisplayCallback(const DisplayCallback &cb){//注册回调函数
        _displayCb=cb;
    }
    void setAreaCallback(const AreaCallback & cb){//注册回调函数
        _areaCb=cb;
    }
    void handleDisplayCallback(){//执行回调函数
        if(_displayCb)
            _displayCb();
    }
    double handleAreaCallback(){//执行回调函数
        if(_areaCb)
            return _areaCb();
        else
        {
            return 0;
        }
    }
private:
    DisplayCallback _displayCb;
    AreaCallback _areaCb;
};

void display(Figure &fig){
    fig.handleDisplayCallback();
    cout << "的面积是：" << fig.handleAreaCallback() << endl;
}

class Rectangle
{
public:
	Rectangle(double length, double width)
	: _length(length)
	, _width(width)
	{
		cout << "Rectangle()" << endl;
	}

	void display() const
	{
		cout << "rectangle";
	}

	double area() const
	{	
		return _length * _width;
	}
private:
	double _length;
	double _width;
};


class Circle
{
public:
	Circle(double radius)
	: _radius(radius)
	{
		cout << "Circle()" << endl;
	}

	void print() const
	{	cout << "circle";	}

	double calcArea() const
	{
		return 3.14159 * _radius * _radius;
	}

private:
	double _radius;
};

class Triangle
{
public:
	Triangle(double a, double b, double c)
	: _a(a)
	, _b(b)
	, _c(c)
	{
		cout << "Triangle()" << endl;
	}

	void show() const
	{
		cout << "triangle";
	}

	double getArea() const
	{
		double p = (_a + _b + _c)/2;
		return sqrt(p * (p - _a) * (p - _b) * (p - _c));
	}

private:
	double _a;
	double _b;
	double _c;
};


class Cube
{
public:
	Cube(double a)
	: _a(a)
	{
		cout << "Cube()" << endl;
	}

	void display() const
	{	cout << "cube";}

	double area() const
	{
		return 6 * _a * _a;
	}

private:
	double _a;
};

 
int main(void)
{
	Figure figure;

	Rectangle rectangle(3, 4);
	Circle circle(10);
	Triangle triangle(3, 4, 5);

	figure.setDisplayCallback(std::bind(&Rectangle::display, &rectangle));
	figure.setAreaCallback(std::bind(&Rectangle::area, &rectangle));
	display(figure);

	figure.setDisplayCallback(std::bind(&Circle::print, &circle));
	figure.setAreaCallback(std::bind(&Circle::calcArea, &circle));
	display(figure);

	figure.setDisplayCallback(std::bind(&Triangle::show, &triangle));
	figure.setAreaCallback(std::bind(&Triangle::getArea, &triangle));
	display(figure);

	Cube cube(10);
	figure.setDisplayCallback(std::bind(&Cube::display, &cube));
	figure.setAreaCallback(std::bind(&Cube::area, &cube));
	display(figure);

	return 0;
}
