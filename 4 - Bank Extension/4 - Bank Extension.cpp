

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

const string ClientsFileName = "Clients.txt";

short ReadChooseNumber()
{
    short Number;

    cout << "Choose What do you want to do ? [1 To 7] : ";
    cin >> Number;

    return Number;
}

short ReadChooseNumberFromTransactions()
{
    short Number;

    cout << "Choose What do you want to do ? [1 To 4] : ";
    cin >> Number;

    return Number;
}
struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

sClient ConvertLineToRecord(string line, string Seperator = "#//#");

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{

    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLineToRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
            vClients.push_back(Client);
        }

        MyFile.close();

    }
    return false;
}



sClient ReadNewClint()
{
    sClient client;

    cout << "Please enter Account Number : ";
    getline(cin >> ws, client.AccountNumber);

    while (ClientExistsByAccountNumber(client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, client.AccountNumber);
    }

    cout << "Please enter Pincode : ";
    getline(cin, client.PinCode);

    cout << "Please enter Name : ";
    getline(cin, client.Name);

    cout << "Please enter Phone : ";
    getline(cin, client.Phone);

    cout << "Please enter Accounte Balance : ";
    cin >> client.AccountBalance;

    return client;

}


string ConvertRecordToLine(sClient Client, string Seperetor = "#//#")
{
    string ClientRecord = "";

    ClientRecord += Client.AccountNumber + Seperetor;
    ClientRecord += Client.PinCode + Seperetor;
    ClientRecord += Client.Name + Seperetor;
    ClientRecord += Client.Phone + Seperetor;
    ClientRecord += to_string(Client.AccountBalance);

    return ClientRecord;
}


void DataLineToFile(string FileName, string DataLine)
{

    fstream MyFile;

    MyFile.open(FileName, ios::out | ios::app);
    {
        if (MyFile.is_open())
        {
            MyFile << DataLine << endl;

            MyFile.close();
        }
    }
}


void AddNewClints()
{

    char AddMore = 'Y';

    do
    {
        system("cls");
        cout << "Adding New Client:\n\n";

        sClient Client;

        Client = ReadNewClint();

        DataLineToFile(ClientsFileName, ConvertRecordToLine(Client));

        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";

        cin >> AddMore;


    } while (toupper(AddMore) == 'Y');

}

void PrintAddNewClients()
{
    cout << "\n-----------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n_____________________________\n";

    AddNewClints();
}


string ReadAccountNumber()
{
    string AccountNumber;

    cout << "Please enter Account Number : ";
    cin >> AccountNumber;

    return AccountNumber;
}


void PrintClientCard(sClient Client)
{
    cout << "\nThe Follwing are The Client datails : " << endl;
    cout << "\nAccount Number   : " << Client.AccountNumber << endl;
    cout << "Pin Code         : " << Client.PinCode << endl;
    cout << "Name             : " << Client.Name << endl;
    cout << "Phone            : " << Client.Phone << endl;
    cout << "Number Balance   : " << Client.AccountBalance << endl;
}


vector<string> SplitString(string st, string Delim)
{
    vector<string> vClients;

    string sWord = "";
    size_t Pos;

    while ((Pos = st.find(Delim)) != std::string::npos)
    {
        sWord = st.substr(0, Pos);

        if (sWord != "")
        {
            vClients.push_back(sWord);
        }

        st.erase(0, Pos + Delim.length());

    }

    if (st != "")
    {
        vClients.push_back(st);

    }

    return vClients;
}


sClient ConvertLineToRecord(string line, string Seperator)
{
    sClient Client;

    vector <string> vClientData = SplitString(line, Seperator);


    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);

    return Client;

}


bool FindAccountByAccountNumber(string AccountNumber, vector<sClient>& vClients, sClient& Client)
{
    for (sClient& s : vClients)
    {
        if (s.AccountNumber == AccountNumber)
        {
            Client = s;
            return true;
        }
    }

    return false;
}


bool MarkClientForDelete(string AccountNumber, vector<sClient>& vClients)
{
    for (sClient& s : vClients)
    {
        if (s.AccountNumber == AccountNumber)
        {
            s.MarkForDelete = true;

            return true;
        }
    }

    return false;
}


void SaveDataLineToFile(string FileName, vector<sClient> vClients)
{
    fstream MyFile;

    MyFile.open(FileName, ios::out);  // Over Write

    string DataLine;

    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {
            if (C.MarkForDelete == false)
            {
                DataLine = ConvertRecordToLine(C);

                MyFile << DataLine << endl;

            }
        }
        MyFile.close();
    }

}


vector<sClient> LoadClientsDataFromFile(string FileName)
{
    vector<sClient> vClients;

    fstream MyFile;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLineToRecord(Line);

            vClients.push_back(Client);

        }
        MyFile.close();
    }

    return vClients;
}


bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindAccountByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);

        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDelete(AccountNumber, vClients);
            SaveDataLineToFile(ClientsFileName, vClients);

            vClients = LoadClientsDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully." << endl;

            return true;

        }


    }

    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
    }

    return false;
}


void PrintClientsAfterDelete()
{
    cout << "\n-----------------------------\n";
    cout << "\tDelete Clients Screen";
    cout << "\n_____________________________\n";

    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadAccountNumber();

    DeleteClientByAccountNumber(AccountNumber, vClients);
}


sClient changeClientRecord(string AccountNumber)
{
    sClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "Please enter Pin Code : ";
    getline(cin >> ws, Client.PinCode);

    cout << "Please enter Name : ";
    getline(cin, Client.Name);

    cout << "Please enter Phone : ";
    getline(cin, Client.Phone);

    cout << "Please enter Accounte Balance : ";
    cin >> Client.AccountBalance;

    return Client;

}


bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindAccountByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);

        cout << "\n\nAre you sure you want Update this client? y/n ? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            for (sClient& s : vClients)
            {
                if (s.AccountNumber == AccountNumber)
                {
                    s = changeClientRecord(AccountNumber);
                    break;
                }
            }
        }

        SaveDataLineToFile(ClientsFileName, vClients);

        cout << "\n\nClient Updated Successfully.";

        return true;


    }

    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
    }

    return false;

}


void PrintClientAfterUpdate()
{
    cout << "\n-----------------------------\n";
    cout << "\tUpdate Clients Screen";
    cout << "\n_____________________________\n";

    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadAccountNumber();

    UpdateClientByAccountNumber(AccountNumber, vClients);
}


bool FindClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{

    for (sClient& s : vClients)
    {
        if (s.AccountNumber == AccountNumber)
        {
            PrintClientCard(s);


            return true;
        }
    }

    cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!\n";
    return false;
}


void PrintFindClient()
{
    cout << "\n-----------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n_____________________________\n";

    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadAccountNumber();

    FindClientByAccountNumber(AccountNumber, vClients);
}


void PrintClientRecord(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;

}


void AllClientsData(vector<sClient> vClients)
{
    cout << "\n\t\t\t\t\tClinrt List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;


    for (sClient Client : vClients)
    {
        PrintClientRecord(Client);
        cout << endl;

    }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

}


void PrintAllClientsData()
{
    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    AllClientsData(vClients);
}


void ShowEndScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :-)";
    cout << "\n-----------------------------------\n";
}


double ReadDepositAmount()
{
    cout << "________________________________________________________" << endl;

    double DepositAmount;

    cout << "Please enter Deposit Amount : ";
     cin >> DepositAmount;

     return DepositAmount;
}


double ReadWithDrawAmount()
{
    cout << "________________________________________________________" << endl;

    double WithDrawAmount;

    cout << "Please enter withDraw Amount : ";
    cin >> WithDrawAmount;

    return WithDrawAmount;
}


bool DepositScreen(string AccountNumber, vector<sClient>& vClients)
{
    char Answer = 'n';
    sClient Client;

    if (FindAccountByAccountNumber(AccountNumber, vClients , Client))
    {
        PrintClientCard(Client);
       double DepositAmount = ReadDepositAmount();

        cout << "\n\nAre you sure you want Perform This Transaction ? y/n ? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            for (sClient& s : vClients)
            {
                if (s.AccountNumber == AccountNumber)
                {
                    s.AccountBalance += DepositAmount;
                    Client = s;

                    SaveDataLineToFile(ClientsFileName, vClients);
                    cout << "\nDone Successfully , New Balanc is " << s.AccountBalance << endl;
                    break;
                }
            }
        }
        return true;
    }

    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
    }

    return false;

}


void PrintDepostitScreen()
{
    cout << "________________________________________________________" << endl;
    cout << "\t Deposit Screen" << endl;
    cout << "________________________________________________________" << endl;

    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadAccountNumber();

    DepositScreen(AccountNumber, vClients);

}


bool WithDrawScreen(string AccountNumber, vector<sClient> &vClients)
{
    char Answer = 'n';
    sClient Client;

    if (FindAccountByAccountNumber(AccountNumber, vClients , Client))
    {
        PrintClientCard(Client);
        double WithDrawAmount = ReadWithDrawAmount();

        while (WithDrawAmount > Client.AccountBalance)
        {
            cout << "Amount Exceeds The Balance , You Can WithDraw Up To " << Client.AccountBalance << endl;
           
            WithDrawAmount = ReadWithDrawAmount();
        }

        cout << "\n\nAre you sure you want Perforf This Transaction ? y/n ? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            for (sClient& s : vClients)
            {
                if (s.AccountNumber == AccountNumber)
                {
                    s.AccountBalance -= WithDrawAmount;
                    Client = s;

                    SaveDataLineToFile(ClientsFileName, vClients);
                    cout << "\nDone Successfully , New Balanc is " << s.AccountBalance << endl;
                    break;
                }
            }
        }

        return true;
    }

    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
    }

    return false;

}


void PrintWithDrawScreen()
{
    cout << "________________________________________________________" << endl;
    cout << "\t WithDraw Screen" << endl;
    cout << "________________________________________________________" << endl;

    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadAccountNumber();

    WithDrawScreen(AccountNumber, vClients);

}


void PrintClientTotalBalances(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}


void TotalBalance(vector<sClient> vClients)
{
    cout << "\n\t\t\t\t\tBalances Lists (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;


    for (sClient Client : vClients)
    {
        PrintClientTotalBalances(Client);
        cout << endl;
    }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;


    double Add = 0;

    for (sClient& s : vClients)
    {
        Add += s.AccountBalance;
    }

    cout << "\t\t\t\t\tTotal Balances = " << Add << endl;

}


void PrintTotalBalances()
{
    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    TotalBalance(vClients);
}


enum enMainMenueOptions
{
    eShowClientList = 1,
    eAddNewClient = 2,
    eDeleteClient = 3,
    eUpdateClint = 4,
    eFindClient = 5,
    eTransactions = 6,
    eExit = 7,
};


enum  enTransactions
{
    enDeposit = 1,
    enWithDraw = 2,
    enTotalBalances = 3,
    enMainMenue = 4,
};


void PrintTransactiondMenue();
void PrintMainMenueScreen();
void GoBackToMainMenueFromTrasactions();

void PerformTransactionsMenue(enTransactions Transactions)
{

    switch (Transactions)
    {
    case enTransactions::enDeposit:
    {
        system("cls");
        PrintDepostitScreen();
        GoBackToMainMenueFromTrasactions();
        break;
    }

    case enTransactions::enWithDraw:
    {
        system("cls");
        PrintWithDrawScreen();
        GoBackToMainMenueFromTrasactions();
        break;
    }

    case enTransactions::enTotalBalances:
    {
        system("cls");
        PrintTotalBalances();
        GoBackToMainMenueFromTrasactions();
        break;
    }

    case enTransactions::enMainMenue:
    {
        system("cls");
        PrintMainMenueScreen();
        break;
    }

    }
}



void PrintTransactiondMenue()
{
    system("cls");
    cout << "========================================================" << endl;
    cout << "\t\t Transactions Menue Screen " << endl;
    cout << "========================================================" << endl;


    cout << "\t[1] Deposit" << endl;
    cout << "\t[2] WithDraw" << endl;
    cout << "\t[3] Total Balances" << endl;
    cout << "\t[4] Main Menue" << endl;

    cout << "========================================================" << endl;

    PerformTransactionsMenue((enTransactions)ReadChooseNumberFromTransactions());

}


void PrintMainMenueScreen();

void GoBackToMainMenueFromTrasactions()
{
    cout << "\nPress any Key Go To Back Transactions Menue ...";
    system("pause>0");

    system("cls");
    PrintTransactiondMenue();
}


void GoBackToMainMenue()
{
    cout << "\n\nPress Any Key To Go Back To Main Menue ....";
    system("pause>0");

    PrintMainMenueScreen();
}


void PerformMainMenueOption(enMainMenueOptions MainMenueOption)
{
    switch (MainMenueOption)
    {
    case enMainMenueOptions::eShowClientList:
    {
        system("cls");
        PrintAllClientsData();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOptions::eAddNewClient:
    {
        system("cls");
        PrintAddNewClients();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOptions::eDeleteClient:
    {
        system("cls");
        PrintClientsAfterDelete();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOptions::eUpdateClint:
    {
        system("cls");
        PrintClientAfterUpdate();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOptions::eFindClient:
    {
        system("cls");
        PrintFindClient();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOptions::eTransactions:
    {
        system("cls");
        PrintTransactiondMenue();
        break;
    }
    case enMainMenueOptions::eExit:
    {
        system("cls");
        ShowEndScreen();
        break;
    }

    }
}


void PrintMainMenueScreen()
{
    system("cls");
    cout << "========================================================" << endl;
    cout << "\t\t Main Menue Screen " << endl;
    cout << "========================================================" << endl;

    cout << "\t [1] Show Client List" << endl;
    cout << "\t [2] Add New Client" << endl;
    cout << "\t [3] Delete Client" << endl;
    cout << "\t [4] Update Client Info" << endl;
    cout << "\t [5] Find Client" << endl;
    cout << "\t [6] Transactions" << endl;
    cout << "\t [7] Exit" << endl;

    cout << "========================================================" << endl;

    PerformMainMenueOption((enMainMenueOptions)ReadChooseNumber());
}



int main()
{
    PrintMainMenueScreen();
    system("pause>0");
}