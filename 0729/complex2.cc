#include <iostream>
#include <limits>
using std::cout;
using std::endl;

class complex{
public:
    complex(double dreal,double dimag)
    :_dreal(dreal)
    ,_dimag(dimag)
    {
        cout << "complex(double,double)" << endl;
    }
    void display() const{
        cout << _dreal << " + " << _dimag << "i" <<endl;
    }
    //前置++
    complex & operator++(){
        ++ _dreal;
        ++ _dimag;
        return *this;
    }
    //后置++
    complex operator++(int)//后置形式在其参数中多加一个int,该int不是要传递参数，只是为了前置形式进行区分
    {
        complex tmp(*this);
        ++ _dreal;
        ++ _dimag;
        return tmp;
    }
    complex &operator+=(const complex &rhs){
        _dreal+=rhs._dreal;
        _dimag+=rhs._dimag;

        return *this;
    }
    	//运算符重载之友元(普通)函数，友元函数可以访问类的私有成员
	friend complex operator+(const complex & lhs, const complex & rhs );
	friend bool operator==(const complex & lhs, const complex & rhs);
    //输出流运算符的要求是:左操作数为流对象，右操作数为输出的内容
	//
	//因此输出流运算符不能作为成员函数形式存在
	//
	//std::ostream & operator<<(std::ostream & os);
	
	friend std::ostream & operator<<(std::ostream & os, const complex & rhs);
	friend std::istream & operator>>(std::istream & is,  complex & rhs);
private:
    double _dreal;
    double _dimag;
};
complex operator+(const complex & lhs, const complex & rhs )
{
	return complex(lhs._dreal + rhs._dreal, 
				   lhs._dimag + rhs._dimag);
}
bool operator==(const complex & lhs, const complex & rhs)
{
	return (lhs._dreal == rhs._dreal) && 
		   (lhs._dimag == rhs._dimag);
}
bool operator!=(const complex &lhs,const complex & rhs){
    return !(lhs==rhs);
}
std::ostream &operator<<(std::ostream &os,const complex &rhs){
    if(rhs._dimag == 0)
		os << rhs._dreal;
	else {
		if(rhs._dreal == 0) {
			if(rhs._dimag == 1)
				os << "i";
			else if(rhs._dimag == -1)
				os << "-i";
			else 
				os << rhs._dimag << "i";
		}
		else {
			os << rhs._dreal;
			if(rhs._dimag > 0) {
				os << " + " << rhs._dimag << "i";
			} else if(rhs._dimag < 0) {
				os << " - " << rhs._dimag * (-1) << "i";
			}
		}
	}
	return os;
}    
void readDoubleValue(std::istream &is,double &number){
    cout << "pls input valid double value:" << endl;
    while(is >> number,!is.eof()){
        if(is.bad()){
            cout << "istream has corrupted!" <<endl;
            return ;
        }
        else if(is.fail()){
            is.clear();
            is.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            cout << "pls input valid double value:" << endl;
            continue;
        }
        break;
    }
}
std::istream & operator>>(std::istream &is,complex &rhs){
    readDoubleValue(is,rhs._dreal);
    readDoubleValue(is,rhs._dimag);
    return is;
}

int main(){

    return 0;
}