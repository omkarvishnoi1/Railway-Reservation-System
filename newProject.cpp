#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <time.h>

using namespace std;

class Train
{
private:
    int trainno;
    char trainname[60];
    char startingpoint[60];
    char destination[60];
    int nofafseat, nofasseat, noffsseat, nofacseat, nofssseat;

public:
    Train()
    {
        trainno = 0;
        nofafseat = 0;
        nofasseat = 0;
        noffsseat = 0;
        nofacseat = 0;
        nofssseat = 0;
        strcpy(trainname, "");
        strcpy(startingpoint, "");
        strcpy(destination, "");
    }

    void input();
    void display();
    int returnTrainNumber() { return trainno; }
    int returnAF() { return nofafseat; }
    int returnAS() { return nofasseat; }
    int returnFS() { return noffsseat; }
    int returnAC() { return nofacseat; }
    int returnSS() { return nofssseat; }
};

void Train::input()
{
    cout << "ENTER THE TRAIN NUMBER: ";
    cin >> trainno;
    cout << "ENTER THE NUMBER OF A/C FIRST CLASS SEATS: ";
    cin >> nofafseat;
    cout << "ENTER THE NUMBER OF A/C SECOND CLASS SEATS: ";
    cin >> nofasseat;
    cout << "ENTER THE NUMBER OF FIRST CLASS SLEEPER SEATS: ";
    cin >> noffsseat;
    cout << "ENTER THE NUMBER OF A/C CHAIR CAR SEATS: ";
    cin >> nofacseat;
    cout << "ENTER THE NUMBER OF SECOND CLASS SLEEPER SEATS: ";
    cin >> nofssseat;
    cout << "ENTER THE TRAIN NAME: ";
    cin.ignore();
    cin.getline(trainname, 60);
    cout << "ENTER THE STARTING POINT: ";
    cin.getline(startingpoint, 60);
    cout << "ENTER THE DESTINATION: ";
    cin.getline(destination, 60);
}

void Train::display()
{
    cout << "TRAIN NUMBER: " << trainno << endl;
    cout << "TRAIN NAME: " << trainname << endl;
    cout << "NO OF A/C FIRST CLASS SEATS: " << nofafseat << endl;
    cout << "NO OF A/C SECOND CLASS SEATS: " << nofasseat << endl;
    cout << "NO OF FIRST CLASS SLEEPER SEATS: " << noffsseat << endl;
    cout << "NO OF A/C CHAIR CAR SEATS: " << nofacseat << endl;
    cout << "NO OF SECOND CLASS SLEEPER SEATS: " << nofssseat << endl;
    cout << "STARTING POINT: " << startingpoint << endl;
    cout << "DESTINATION: " << destination << endl;
}
class Ticket
{
private:
    char name[40];
    int age;
    int resno;
    char status[10];
    int tno, toaf, toas, tofs, toac, toss;

public:
    Ticket() : tno(0), toaf(0), toas(0), tofs(0), toac(0), toss(0) {}

    void reservation();
    void displayTicket();
};

void Ticket::reservation()
{
    srand(time(nullptr));
    cout << "RESERVATION" << endl;
    cout << "ENTER THE TRAIN NO: ";
    int tno;
    cin >> tno;

    ifstream fin("Train.dat", ios::binary);
    if (!fin)
    {
        cerr << "ERROR IN THE FILE" << endl;
        return;
    }

    Train tr;
    int found = 0;
    while (fin.read(reinterpret_cast<char *>(&tr), sizeof(tr)))
    {
        if (tno == tr.returnTrainNumber())
        {
            found = 1;
            toaf = tr.returnAF();
            toas = tr.returnAS();
            tofs = tr.returnFS();
            toac = tr.returnAC();
            toss = tr.returnSS();
            break;
        }
    }
    fin.close();

    if (!found)
    {
        cerr << "ERROR IN THE TRAIN NUMBER ENTERED!!!" << endl;
        return;
    }

    ofstream fout("Ticket.dat", ios::app | ios::binary);
    if (!fout)
    {
        cerr << "ERROR IN OPENING TICKET FILE" << endl;
        return;
    }

    cout << "NAME: ";
    cin.ignore();
    cin.getline(name, 40);
    cout << "AGE: ";
    cin >> age;

    cout << "SELECT THE CLASS WHICH YOU WISH TO TRAVEL" << endl;
    cout << "1. A/C FIRST CLASS" << endl;
    cout << "2. A/C SECOND CLASS" << endl;
    cout << "3. FIRST CLASS SLEEPER" << endl;
    cout << "4. A/C CHAIR CAR" << endl;
    cout << "5. SECOND CLASS SLEEPER" << endl;
    cout << "ENTER YOUR CHOICE: ";
    int c;
    cin >> c;

    switch (c)
    {
    case 1:
        toaf++;
        resno = rand();
        strcpy(status, (toaf < tr.returnAF()) ? "confirmed" : "pending");
        break;
    case 2:
        toas++;
        resno = rand();
        strcpy(status, (toas < tr.returnAS()) ? "confirmed" : "pending");
        break;
    case 3:
        tofs++;
        resno = rand();
        strcpy(status, (tofs < tr.returnFS()) ? "confirmed" : "pending");
        break;
    case 4:
        toac++;
        resno = rand();
        strcpy(status, (toac < tr.returnAC()) ? "confirmed" : "pending");
        break;
    case 5:
        toss++;
        resno = rand();
        strcpy(status, (toss < tr.returnSS()) ? "confirmed" : "pending");
        break;
    default:
        cerr << "Invalid choice!" << endl;
        return;
    }

    cout << "STATUS: " << status << endl;
    cout << "RESERVATION NO: " << resno << endl;
    fout.write(reinterpret_cast<char *>(this), sizeof(*this));
    fout.close();
}

void Ticket::displayTicket()
{
    cout << "Name: " << name << endl;
    cout << "Age: " << age << endl;
    cout << "Reservation No: " << resno << endl;
    cout << "Status: " << status << endl;
}
void addTrain()
{
    ofstream fout("Train.dat", ios::app | ios::binary);
    if (!fout)
    {
        cerr << "Error in opening file!" << endl;
        return;
    }

    Train tr;
    tr.input();
    fout.write(reinterpret_cast<char *>(&tr), sizeof(tr));
    fout.close();
    cout << "Train added successfully!" << endl;
}

void viewTrains()
{
    ifstream fin("Train.dat", ios::binary);
    if (!fin)
    {
        cerr << "Error in opening file!" << endl;
        return;
    }

    Train tr;
    while (fin.read(reinterpret_cast<char *>(&tr), sizeof(tr)))
    {
        tr.display();
        cout << "---------------------------------" << endl;
    }
    fin.close();
}

void viewTickets()
{
    ifstream fin("Ticket.dat", ios::binary);
    if (!fin)
    {
        cerr << "Error in opening file!" << endl;
        return;
    }

    Ticket tick;
    while (fin.read(reinterpret_cast<char *>(&tick), sizeof(tick)))
    {
        tick.displayTicket();
        cout << "---------------------------------" << endl;
    }
    fin.close();
}

int main()
{
    int choice;
    Ticket ticket;

    while (true)
    {
        cout << "1. Add Train" << endl;
        cout << "2. View Trains" << endl;
        cout << "3. Make Reservation" << endl;
        cout << "4. View Tickets" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            addTrain();
            break;
        case 2:
            viewTrains();
            break;
        case 3:
            ticket.reservation();
            break;
        case 4:
            viewTickets();
            break;
        case 5:
            exit(0);
        default:
            cout << "Invalid choice!" << endl;
        }
    }
    return 0;
}