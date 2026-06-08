#include<iostream>
using namespace std;
int main(){
    //TODO
    int units = 5;
    double price = 12.5;
    int idnumber = 12583;
    double cost = price*units;
    cout <<idnumber<<" "<<units<<" "<<price<<" "<<cost<<" "<<endl;
    double tax = cost*0.06;
    double total = cost + tax;
    cout <<tax<<" "<<total<<endl;
    return 0;
}
