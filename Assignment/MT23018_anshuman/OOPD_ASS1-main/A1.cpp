
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

const int MAX_ROWS = 1000;  // Define the maximum number of rows
const int NUM_COLS = 4;     // Define the number of columns

struct Bank {
    int Serial;
    int Year;
    double Rate;
    double Inflation;
};

void moneyReceived(Bank investments[], int numRows, float principal, int Year) {
    float newPrincipal=principal;
    float netRate;
    for (int i = 0; i < numRows;i++)
        {
        if (investments[i].Year>=Year && investments[i].Year <= 2022)
         {
                 netRate = investments[i].Rate - investments[i].Inflation;
                newPrincipal = newPrincipal * (1 + (netRate / 100));
                if(investments[i].Year == 2001)
        {
                newPrincipal = newPrincipal / 166;  // Set newPrincipal for the currency change
        }
    }
}

    // Round the final result to two decimal places
    cout << fixed << setprecision(2);  // Set output to fixed and two decimal places
    cout << "The Actual money received after " <<  Year << " years is: " << newPrincipal << " euros" << endl;
}




void moneyInvested(Bank investments[], int numRows, float principal, int Year) {
    float newPrincipal=principal;
    for (int i = 0; i < numRows;i++)
        {
        if (investments[i].Year>=Year && investments[i].Year <= 2022)
         {
                newPrincipal = newPrincipal * (1 + (investments[i].Rate/ 100));
                if(investments[i].Year == 2001)
        {
                newPrincipal = newPrincipal / 166;  // Set newPrincipal for the currency change
        }
    }
}
    cout<<"The investment money for year " <<Year<<" is: "<<newPrincipal<<endl;

}



int main() {
    ifstream inputFile;
    inputFile.open("spain_data.csv");

    Bank investments[MAX_ROWS];
    int numRows=0;

    string line="";
    getline(inputFile,line);  // Read the header line and discard it

    while(getline(inputFile,line)&&numRows<MAX_ROWS){
        stringstream inputString(line);
        string tempString;

        getline(inputString,tempString,',');
        investments[numRows].Serial = atoi(tempString.c_str());

        getline(inputString,tempString,',');
        investments[numRows].Year = atoi(tempString.c_str());

        getline(inputString, tempString,',');
        investments[numRows].Rate = atof(tempString.c_str());

        getline(inputString,tempString,',');
        investments[numRows].Inflation = atof(tempString.c_str());

        ++numRows;

        line = "";
    }

    inputFile.close();  // Closing the File.

    int year;
    double amount;

    cout<<"Enter the investment year: "<<endl;
    cin>>year;
    while(year>2022 || year<1960)
        {
        cout<<"Enter a valid year of investment from (1960-2022): "<<endl;
        cin>>year;
        }

    cout<<"Enter the investment amount: "<<endl;
    cin>>amount;
    while(amount<1)
        {
        cout<<"Enter a valid amount: "<<endl;
        cin>>amount;
        }

    moneyInvested(investments,numRows,amount,year);
    moneyReceived(investments,numRows,amount,year);
    return 0;
}

