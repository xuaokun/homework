
#include <stdio.h>

#include <iostream>
using std::cout;
using std::endl;

class Line{
    class LineImpl;
public:
    Line(int,int,int,int);
    ~Line();

    void printLine() const;
private:
    LineImpl * _pimpl;
};