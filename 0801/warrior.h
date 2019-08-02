

class warrior{
public:
    warrior(num,hp)
    :_number(num)
    ,_HP(hp)
    {}
private:
    int _number;//编号
    int _HP;//生命值
};

class dragon
:public warrior
{
public:
    dragon()
    :warrior(num,hp)
    ,_morale(totalHP/hp)//士气
    ,_weapon(num%3)//武器
    {}
private:
    double _morale;
    int _weapon;
}
int main (){
    int rTotalHP=10000;
    int bTotalHP=10000;
}