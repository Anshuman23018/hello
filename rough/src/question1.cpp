#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class Investment {
private:
    int index;
    int year;
    float interestRate;
    float inflationRate;
    string government;
    string continent;

public:
Investment() {
        index = 0;
        year = 0;
        interestRate=0.0f;
        inflationRate=0.0f;
        government="";
        continent="";
    }
    Investment(int index, int year, float interestRate, float inflationRate, string government, string continent) {
        this->index = index;
        this->year = year;
        this->interestRate = interestRate;
        this->inflationRate = inflationRate;
        this->government = government;
        this->continent = continent;
    }

    int getyear() {
      return year;
    }
     float getIntrestRate() {
      return interestRate;
    }
     float getInflationRate() {
      return inflationRate;
    }

     float roundToNearestPaisa(float value) {
        int roundedValue = static_cast<int>(value * 100 + 0.5);
        return static_cast<float>(roundedValue) / 100;
    }
    void MoneyRecived(Investment investments[],int count,float principalAmount,int investYear){
       
        float updatedPrincipal=principalAmount;
        for (int i=0;i<count;i++) {
        if (investments[i].getyear()>=investYear && investments[i].getyear()<=2022) {
            float netRate=investments[i].getIntrestRate()-investments[i].getInflationRate();
            
             updatedPrincipal*= (1+(netRate/100));
        }
}
updatedPrincipal=roundToNearestPaisa(updatedPrincipal);
cout<<"The Actual money recived after "<<2022-investYear<<" years is :"<<updatedPrincipal<<endl;

    }
void ActualValue(Investment investments[],int count,float principalAmount,int investYear){
    float updatedValue=principalAmount;
for (int i=0;i<count;i++) {
        if (investments[i].getyear()>=investYear && investments[i].getyear()<=2022) {
            
             updatedValue*= (1+(investments[i].getIntrestRate()/100));
        }    
}
 updatedValue=roundToNearestPaisa(updatedValue); 
cout<<"The invested money after year "<<investYear<<" is :"<<updatedValue<<endl;
}
};

int main() {
    ifstream ifs("india_data.csv");

    if (!ifs.is_open()) {
        cout << "Error opening file" << endl;
        return 1;
    }
    // Skips the first line 
    string headerLine;
    getline(ifs, headerLine);

    const int MaxInvestments = 63; 
    Investment investments[MaxInvestments];
    int count = 0;

    string line;

    while (getline(ifs, line) && count<MaxInvestments) {
        stringstream ss(line);
        string tempString;
        getline(ss, tempString,',');
        int index = stoi(tempString);
        getline(ss, tempString,',');
        int year = stoi(tempString);
        getline(ss, tempString,',');
        float interestRate = stof(tempString);
        getline(ss, tempString,',');
        float inflationRate = stof(tempString);
        getline(ss, tempString, ','); // Consume the government field
        getline(ss, tempString); // Consume the continent field

        Investment investment(index, year, interestRate, inflationRate, "", "");// government and continent filed are not stored.
         investments[count]=investment;
         count++;
    }

    ifs.close();

    float principalAmount;
    int investYear;

    cout << "Enter the principle investment amount: ";
    cin >> principalAmount;
    while(principalAmount<1){
        cout<<"\nEnter vaild amount";
        cin>>principalAmount;
    }

    cout << "Enter the year of investment: ";
    cin >> investYear;
    while(investYear>2022 || investYear<1960){
        cout<<"\nEnter vaild year of investment(1960-2022)";
        cin>>investYear;
    }
   
    Investment investmentObj(0,0,0.0f,0.0f, "", "");// object for investment class

    // Calling the MoneyRecived function
    investmentObj.ActualValue(investments,count,principalAmount, investYear); 
    investmentObj.MoneyRecived(investments,count,principalAmount, investYear);
            
    return 0;
}
