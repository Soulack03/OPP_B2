#include<bits/stdc++.h>
 using namespace std;
 class Laptop {
 public:
 string brand;
 string model;
 int ram; // Dung ượlng RAM tính ằbng GB
 int storage; // Dung ượlng ổứ cng tính ằbng GB
 float gpu;
  void displayInfo() {
 cout << "Brand: " << brand << endl;
 cout << "Model: " << model << endl;
 cout << "RAM: " << ram << " GB" << endl;
 cout << "Storage: " << storage << " GB" << endl;
 }
 void checkGPU(float requiredGPU ){
    if (gpu >= requiredGPU) {
   cout << "This laptop has enough GPU to run the game." << endl;
   } else {
   cout << "This laptop does not have enough GPU to run the game." << endl;
   }
  }
   
 void checkRAM(int requiredRAM) {
   if (ram >= requiredRAM) {
   cout << "This laptop has enough RAM to run the software." << endl;
   } else {
   cout << "This laptop does not have enough RAM to run the software." << endl;
   }
  }
 void upgradeRAM(int additionalRAM){
    ram+=additionalRAM;
    cout<<"RAM update success full !"<<endl;
    cout<< "RAM :"<<ram<<endl;
}
};
 int main() {
 Laptop laptop1;
 laptop1.brand = "Dell";
 laptop1.model = "XPS 13";
 laptop1.ram = 8;
 laptop1.storage = 256;
 laptop1.displayInfo();
 // ểKim tra xem laptop có đủ RAM đểạ chy ầphn ềmm yêu ầcu 16GB RAM
 laptop1.checkRAM(16);
 laptop1.upgradeRAM(90);
 return 0;
 }