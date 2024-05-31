#include <iostream>
#include "model.hxx"
using namespace std;

int main()
{
    Model& m = Model::instance();
    m.evaluate(300, 1000, 1);
    cout<<"Success";
    return 0;
}