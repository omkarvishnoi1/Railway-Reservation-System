#include <bits/stdc++.h>
using namespace std;

/*
  Railway Reservation Demo:
  - Trains stored in Train.dat (binary)
  - Tickets stored in Ticket.dat (binary)
  - Reservation counter in ResCounter.dat (binary)
*/

struct TicketRecord {
    char name[40];
    int age;
    int resno;
    char status[16];   // confirmed / waiting / cancelled
    int tno;           // train number
    int travelClass;   // 1..5
};

class Train {
private:
    int trainno;
    char trainname[60];
    char startingpoint[60];
    char destination[60];
    int nofafseat, nofasseat, noffsseat, nofacseat, nofssseat;

public:
    Train() {
        trainno = 0;
        nofafseat = nofasseat = noffsseat = nofacseat = nofssseat = 0;
        trainname[0] = startingpoint[0] = destination[0] = '\0';
    }

    void input();
    void display() const;
    int returnTrainNumber() const { return trainno; }

    // Try to book a seat in given class
    bool bookSeat(int cls) {
        switch (cls) {
            case 1: if (nofafseat > 0) { --nofafseat; return true; } else return false;
            case 2: if (nofasseat > 0) { --nofasseat; return true; } else return false;
            case 3: if (noffsseat > 0) { --noffsseat; return true; } else return false;
            case 4: if (nofacseat > 0) { --nofacseat; return true; } else return false;
            case 5: if (nofssseat > 0) { --nofssseat; return true; } else return false;
            default: return false;
        }
    }

    // Restore a seat when cancelled
    void restoreSeat(int cls) {
        switch (cls) {
            case 1: ++nofafseat; break;
            case 2: ++nofasseat; break;
            case 3: ++noffsseat; break;
            case 4: ++nofacseat; break;
            case 5: ++nofssseat; break;
        }
    }
};

void Train::input() {
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
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.getline(trainname, sizeof(trainname));
    cout << "ENTER THE STARTING POINT: ";
    cin.getline(startingpoint, sizeof(startingpoint));
    cout << "ENTER THE DESTINATION: ";
    cin.getline(destination, sizeof(destination));
}

void Train::display() const {
    cout << "---------------------------------\n";
    cout << "TRAIN NUMBER: " << trainno << "\n";
    cout << "TRAIN NAME: " << trainname << "\n";
    cout << "NO OF A/C FIRST CLASS SEATS: " << nofafseat << "\n";
    cout << "NO OF A/C SECOND CLASS SEATS: " << nofasseat << "\n";
    cout << "NO OF FIRST CLASS SLEEPER SEATS: " << noffsseat << "\n";
    cout << "NO OF A/C CHAIR CAR SEATS: " << nofacseat << "\n";
    cout << "NO OF SECOND CLASS SLEEPER SEATS: " << nofssseat << "\n";
    cout << "STARTING POINT: " << startingpoint << "\n";
    cout << "DESTINATION: " << destination << "\n";
}

// Generate unique sequential reservation number
int generateReservationNo() {
    fstream counter("ResCounter.dat", ios::in | ios::out | ios::binary);
    int last = 10000;  // starting number
    if (counter) {
        counter.read(reinterpret_cast<char*>(&last), sizeof(last));
    }
    last++;
    counter.close();

    counter.open("ResCounter.dat", ios::out | ios::binary | ios::trunc);
    counter.write(reinterpret_cast<char*>(&last), sizeof(last));
    counter.close();

    return last;
}

// Add a train
void addTrain() {
    ofstream fout("Train.dat", ios::app | ios::binary);
    if (!fout) {
        cerr << "Error in opening Train.dat\n";
        return;
    }
    Train tr;
    tr.input();
    fout.write(reinterpret_cast<char*>(&tr), sizeof(tr));
    fout.close();
    cout << "Train added successfully!\n";
}

// View trains
void viewTrains() {
    ifstream fin("Train.dat", ios::binary);
    if (!fin) {
        cout << "No trains found.\n";
        return;
    }
    Train tr;
    bool any = false;
    while (fin.read(reinterpret_cast<char*>(&tr), sizeof(tr))) {
        tr.display();
        any = true;
    }
    if (!any) cout << "No trains stored.\n";
    fin.close();
}

// Make reservation
void makeReservation() {
    int tno;
    cout << "RESERVATION\nENTER THE TRAIN NO: ";
    cin >> tno;

    fstream file("Train.dat", ios::in | ios::out | ios::binary);
    if (!file) {
        cerr << "Train file not found.\n";
        return;
    }

    Train tr;
    bool found = false;
    streampos recordStartPos;

    while (true) {
        recordStartPos = file.tellg();
        if (!file.read(reinterpret_cast<char*>(&tr), sizeof(tr))) break;
        if (tr.returnTrainNumber() == tno) {
            found = true;
            break;
        }
    }

    if (!found) {
        cerr << "Train not found.\n";
        file.close();
        return;
    }

    TicketRecord rec;
    memset(&rec, 0, sizeof(rec));
    cout << "NAME: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.getline(rec.name, sizeof(rec.name));
    cout << "AGE: ";
    cin >> rec.age;
    cout << "SELECT CLASS\n";
    cout << "1. A/C FIRST CLASS\n2. A/C SECOND CLASS\n3. FIRST CLASS SLEEPER\n4. A/C CHAIR CAR\n5. SECOND CLASS SLEEPER\n";
    cout << "ENTER CHOICE: ";
    int cls;
    cin >> cls;
    rec.travelClass = cls;
    rec.tno = tno;
    rec.resno = generateReservationNo();

    bool booked = tr.bookSeat(cls);
    if (booked) {
        strncpy(rec.status, "confirmed", sizeof(rec.status)-1);
        file.seekp(recordStartPos);
        file.write(reinterpret_cast<char*>(&tr), sizeof(tr));
        file.flush();
    } else {
        strncpy(rec.status, "waiting", sizeof(rec.status)-1);
    }

    file.close();

    ofstream fout("Ticket.dat", ios::app | ios::binary);
    fout.write(reinterpret_cast<char*>(&rec), sizeof(rec));
    fout.close();

    cout << "STATUS: " << rec.status << "\n";
    cout << "RESERVATION NO: " << rec.resno << "\n";
}

// View tickets
void viewTickets() {
    ifstream fin("Ticket.dat", ios::binary);
    if (!fin) {
        cout << "No tickets found.\n";
        return;
    }
    TicketRecord rec;
    bool any = false;
    while (fin.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        if (rec.resno == 0) continue;
        any = true;
        cout << "---------------------------------\n";
        cout << "Name: " << rec.name << "\n";
        cout << "Age: " << rec.age << "\n";
        cout << "Train No: " << rec.tno << "\n";
        cout << "Class: " << rec.travelClass << "\n";
        cout << "Reservation No: " << rec.resno << "\n";
        cout << "Status: " << rec.status << "\n";
    }
    if (!any) cout << "No tickets stored.\n";
    fin.close();
}

// Cancel ticket with auto-upgrade waiting
void cancelTicket() {
    int resno;
    cout << "Enter Reservation Number to cancel: ";
    cin >> resno;

    fstream ticketFile("Ticket.dat", ios::in | ios::out | ios::binary);
    if (!ticketFile) {
        cerr << "Ticket.dat not found.\n";
        return;
    }

    TicketRecord rec;
    bool found = false;
    streampos pos;

    while (ticketFile.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        pos = ticketFile.tellg();
        if (rec.resno == resno) {
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Reservation not found.\n";
        ticketFile.close();
        return;
    }

    if (strcmp(rec.status, "cancelled") == 0) {
        cout << "Ticket already cancelled.\n";
        ticketFile.close();
        return;
    }

    int trainNo = rec.tno;
    int cls = rec.travelClass;

    if (strcmp(rec.status, "confirmed") == 0) {
        fstream trainFile("Train.dat", ios::in | ios::out | ios::binary);
        Train tr;
        streampos trainPos;
        bool trainFound = false;

        while (true) {
            trainPos = trainFile.tellg();
            if (!trainFile.read(reinterpret_cast<char*>(&tr), sizeof(tr))) break;
            if (tr.returnTrainNumber() == trainNo) {
                trainFound = true;
                break;
            }
        }

        if (trainFound) {
            tr.restoreSeat(cls);

            // Look for oldest waiting ticket
            fstream waitFile("Ticket.dat", ios::in | ios::out | ios::binary);
            TicketRecord waitRec;
            streampos waitPos;
            bool waitingFound = false;

            while (waitFile.read(reinterpret_cast<char*>(&waitRec), sizeof(waitRec))) {
                waitPos = waitFile.tellg();
                if (waitRec.tno == trainNo &&
                    waitRec.travelClass == cls &&
                    strcmp(waitRec.status, "waiting") == 0) {
                    waitingFound = true;
                    break;
                }
            }

            if (waitingFound) {
                strncpy(waitRec.status, "confirmed", sizeof(waitRec.status)-1);
                waitFile.seekp(waitPos - static_cast<streamoff>(sizeof(waitRec)));
                waitFile.write(reinterpret_cast<char*>(&waitRec), sizeof(waitRec));
                waitFile.flush();

                tr.bookSeat(cls);

                cout << "Waiting ticket with Reservation No " << waitRec.resno
                     << " upgraded to CONFIRMED.\n";
            }
            waitFile.close();

            trainFile.seekp(trainPos);
            trainFile.write(reinterpret_cast<char*>(&tr), sizeof(tr));
            trainFile.flush();
        }
        trainFile.close();
    }

    strncpy(rec.status, "cancelled", sizeof(rec.status)-1);
    ticketFile.seekp(pos - static_cast<streamoff>(sizeof(rec)));
    ticketFile.write(reinterpret_cast<char*>(&rec), sizeof(rec));
    ticketFile.flush();
    ticketFile.close();

    cout << "Ticket cancelled successfully.\n";
}

int main() {
    while (true) {
        cout << "\n===== RAILWAY RESERVATION SYSTEM =====\n";
        cout << "1. Add Train\n2. View Trains\n3. Make Reservation\n4. View Tickets\n5. Cancel Ticket\n6. Exit\n";
        cout << "Enter your choice: ";
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.\n";
            continue;
        }
        switch (choice) {
            case 1: addTrain(); break;
            case 2: viewTrains(); break;
            case 3: makeReservation(); break;
            case 4: viewTickets(); break;
            case 5: cancelTicket(); break;
            case 6: cout << "Goodbye.\n"; return 0;
            default: cout << "Invalid choice!\n";
        }
    }
    return 0;
}
