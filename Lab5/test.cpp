#include <vector>
#include <iostream>

using namespace std;

// Классы ингридиентов

class Dough{
public:
    void info() {
    cout << "Dough" << endl;
}
};

class Sauce{
public:
    void info() {
    cout << "Sauce" << endl;
}
};

class MozarellaCheese{
public:
    void info() {
    cout << "Mozarella cheese" << endl;
}
};

class Pepperoni{
public:
    void info() {
    cout << "Pepperoni" << endl;
}
};


class Ham{
public:
    void info() {
    cout << "Ham" << endl;
}
};

class Pineapple{
public:
    void info() {
    cout << "Pineapple" << endl;
}
};

class Bacon{
public:
    void info() {
    cout << "Bacon" << endl;
}
};

class GroundBeef{
public:
    void info() {
    cout << "Ground beef" << endl;
}
};
 
// Класс "Пицца", содержащий все ингриденты
class Pizza{
public: 
    vector<Dough> vd;
    vector<Sauce> vs; 
    vector<MozarellaCheese> vm; 
    vector<Pepperoni> vpe; 
    vector<Ham> vh;
    vector<Pineapple> vpi; 
    vector<Bacon> vb; 
    vector<GroundBeef> vgb;
    void info() {
        int i;
        for(i=0; i<vd.size(); ++i) vd[i].info();
        for(i=0; i<vs.size(); ++i) vs[i].info();
        for(i=0; i<vm.size(); ++i) vm[i].info();
        for(i=0; i<vpe.size(); ++i) vpe[i].info();
        for(i=0; i<vh.size(); ++i) vh[i].info();
        for(i=0; i<vpi.size(); ++i) vpi[i].info();
        for(i=0; i<vb.size(); ++i) vb[i].info();
        for(i=0; i<vgb.size(); ++i) vgb[i].info();
    }
};

// Базовый класс PizzaBuilder объявляет интерфейс для поэтапного
// построения армии и предусматривает его реализацию по умолчанию 
class PizzaBuilder{
protected:
    Pizza* p;
public:
    PizzaBuilder(): p(0) {} 
    virtual ~PizzaBuilder() {} 
    virtual void createPizza() {}
    virtual void buildDough() {} 
    virtual void buildSauce() {} 
    virtual void buildMozarellaCheese() {} 
    virtual void buildPepperoni() {} 
    virtual void buildHam() {}
    virtual void buildPineapple() {} 
    virtual void buildBacon() {}
    virtual void buildGroundBeef() {}
    virtual Pizza* getPizza() { return p; }
};

// Мясная пицца 
class MeatLoverPizzaBuilder: public PizzaBuilder{
public:
    void createPizza() { p = new Pizza; }
    void buildDough() { p->vd.push_back( Dough()); } 
    void buildSauce() { p->vs.push_back( Sauce()); }
    void buildMozarellaCheese() { p->vm.push_back( MozarellaCheese()); } 

    void buildBacon() { p->vb.push_back( Bacon()); }
    void buildHam() { p->vh.push_back( Ham()); }
    void buildPepperoni() { p->vpe.push_back( Pepperoni()); }
    void buildGroundBeef() { p->vgb.push_back( GroundBeef()); }
};

// Гавайская пицца
class HawaiianPizzaBuilder: public PizzaBuilder{
public:
    void createPizza() { p = new Pizza; }
    void buildDough() { p->vd.push_back( Dough()); } 
    void buildSauce() { p->vs.push_back( Sauce()); }
    void buildMozarellaCheese() { p->vm.push_back( MozarellaCheese()); } 

    void buildHam() { p->vh.push_back( Ham()); }
    void buildPineapple() { p->vpi.push_back( Pineapple()); }
};

// Пепперони пицца 
class PepperoniPizzaBuilder: public PizzaBuilder{
public:
    void createPizza() { p = new Pizza; }
    void buildDough() { p->vd.push_back( Dough()); } 
    void buildSauce() { p->vs.push_back( Sauce()); }
    void buildMozarellaCheese() { p->vm.push_back( MozarellaCheese()); } 

    void buildPepperoni() { p->vpe.push_back( Pepperoni()); }
};
 
// Класс-распорядитель, поэтапно создающий одну из пицц 
class Director{
public:
    Pizza* createPizza( PizzaBuilder & builder )
    {
        builder.createPizza();
        builder.buildDough();
        builder.buildSauce();
        builder.buildMozarellaCheese();
        builder.buildPepperoni();
        builder.buildHam(); 
        builder.buildPineapple();
        builder.buildBacon(); 
        builder.buildGroundBeef(); 
        return( builder.getPizza());
    }
};

int main()
{
    Director dir;
    PepperoniPizzaBuilder pep_builder;
    HawaiianPizzaBuilder hap_builder;
    MeatLoverPizzaBuilder mlp_builder;

    Pizza * pep = dir.createPizza( pep_builder);
    Pizza * hap = dir.createPizza( hap_builder);
    Pizza * mlp = dir.createPizza( mlp_builder);
    cout << "Pepperoni pizza:" << endl;
    pep->info();
    cout << "\nHawaiian pizza:" << endl;
    hap->info();
    cout << "\nMeat lover pizza:" << endl;
    mlp->info();

return 0;
}
