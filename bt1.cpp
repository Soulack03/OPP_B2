#include<bits/stdc++.h>
using namespace std ;
class Car{
public:
string brand ;
int year;
void displayInfo(){
    cout << "Brand: " << brand << endl;
    cout << "Year: " << year << endl;
}
};
int main(){
    Car car1;
    car1.brand = "Toyota";
    car1.year = 2020;
    car1.displayInfo();
    return 0;
}