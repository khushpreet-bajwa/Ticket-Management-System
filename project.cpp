#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include "httplib.h"

using namespace std;
using namespace httplib;

const int TOTAL_SEATS = 10;
const int MAX_WAITING = 100;
const int MAX_CANCEL = 100;

// -------------------- LINKED LIST --------------------
class TicketNode {
public:
    int ticketID;
    string passengerName;
    string source;
    string destination;
    string journeyDate;
    int seatNo;
    TicketNode* next;

    TicketNode(int id, string name, string src, string dest, string date, int seat) {
        ticketID = id;
        passengerName = name;
        source = src;
        destination = dest;
        journeyDate = date;
        seatNo = seat;
        next = NULL;
    }
};

TicketNode* head = NULL;

// -------------------- QUEUE --------------------
class WaitingPassenger {
public:
    int requestID;
    string passengerName;
    string source;
    string destination;
    string journeyDate;
};

WaitingPassenger waitingQueue[MAX_WAITING];
int frontQ = -1, rearQ = -1;

// -------------------- STACK --------------------
class CancelledTicket {
public:
    int ticketID;
    string passengerName;
    string source;
    string destination;
    string journeyDate;
    int seatNo;
};

CancelledTicket cancelStack[MAX_CANCEL];
int top = -1;

// -------------------- GLOBALS --------------------
bool seatOccupied[TOTAL_SEATS + 1] = {false};
int nextTicketID = 1001;

// -------------------- HELPERS --------------------
int getAvailableSeat() {
    for (int i = 1; i <= TOTAL_SEATS; i++) {
        if (!seatOccupied[i]) {
            return i;
        }
    }
    return -1;
}

bool hasAvailableSeat() {
    return getAvailableSeat() != -1;
}

// -------------------- LINKED LIST --------------------
void insertBookedTicket(int id, string name, string src, string dest, string date, int seatNo) {
    TicketNode* newNode = new TicketNode(id, name, src, dest, date, seatNo);

    if (head == NULL) {
        head = newNode;
    } else {
        TicketNode* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    seatOccupied[seatNo] = true;
}

TicketNode* searchBookedTicket(int ticketID) {
    TicketNode* temp = head;
    while (temp != NULL) {
        if (temp->ticketID == ticketID) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

string getAllBookedTicketsText() {
    ostringstream out;

    if (head == NULL) {
        out << "No booked tickets found.";
        return out.str();
    }

    TicketNode* temp = head;
    out << "========== CONFIRMED TICKETS ==========\n";
    while (temp != NULL) {
        out << "Ticket ID     : " << temp->ticketID << "\n";
        out << "Passenger Name: " << temp->passengerName << "\n";
        out << "Source        : " << temp->source << "\n";
        out << "Destination   : " << temp->destination << "\n";
        out << "Journey Date  : " << temp->journeyDate << "\n";
        out << "Seat No       : " << temp->seatNo << "\n";
        out << "Status        : Confirmed\n";
        out << "--------------------------------------\n";
        temp = temp->next;
    }
    return out.str();
}

// -------------------- QUEUE --------------------
bool isWaitingQueueEmpty() {
    return (frontQ == -1 || frontQ > rearQ);
}

bool isWaitingQueueFull() {
    return rearQ == MAX_WAITING - 1;
}

void enqueueWaitingPassenger(string name, string src, string dest, string date) {
    if (isWaitingQueueFull()) {
        return;
    }

    if (frontQ == -1) {
        frontQ = 0;
    }

    rearQ++;
    waitingQueue[rearQ].requestID = nextTicketID++;
    waitingQueue[rearQ].passengerName = name;
    waitingQueue[rearQ].source = src;
    waitingQueue[rearQ].destination = dest;
    waitingQueue[rearQ].journeyDate = date;
}

WaitingPassenger dequeueWaitingPassenger() {
    WaitingPassenger temp;
    temp.requestID = -1;

    if (isWaitingQueueEmpty()) {
        return temp;
    }

    temp = waitingQueue[frontQ];
    frontQ++;
    return temp;
}

string getWaitingListText() {
    ostringstream out;

    if (isWaitingQueueEmpty()) {
        out << "Waiting list is empty.";
        return out.str();
    }

    out << "========== WAITING LIST ==========\n";
    for (int i = frontQ; i <= rearQ; i++) {
        out << "Request ID    : " << waitingQueue[i].requestID << "\n";
        out << "Passenger Name: " << waitingQueue[i].passengerName << "\n";
        out << "Source        : " << waitingQueue[i].source << "\n";
        out << "Destination   : " << waitingQueue[i].destination << "\n";
        out << "Journey Date  : " << waitingQueue[i].journeyDate << "\n";
        out << "Status        : Waiting\n";
        out << "----------------------------------\n";
    }
    return out.str();
}

// -------------------- STACK --------------------
bool isCancelStackEmpty() {
    return top == -1;
}

bool isCancelStackFull() {
    return top == MAX_CANCEL - 1;
}

void pushCancelledTicket(int id, string name, string src, string dest, string date, int seatNo) {
    if (isCancelStackFull()) {
        return;
    }

    top++;
    cancelStack[top].ticketID = id;
    cancelStack[top].passengerName = name;
    cancelStack[top].source = src;
    cancelStack[top].destination = dest;
    cancelStack[top].journeyDate = date;
    cancelStack[top].seatNo = seatNo;
}

CancelledTicket popCancelledTicket() {
    CancelledTicket temp;
    temp.ticketID = -1;

    if (isCancelStackEmpty()) {
        return temp;
    }

    temp = cancelStack[top];
    top--;
    return temp;
}

string getCancelHistoryText() {
    ostringstream out;

    if (isCancelStackEmpty()) {
        out << "No cancel history found.";
        return out.str();
    }

    out << "========== CANCEL HISTORY ==========\n";
    for (int i = top; i >= 0; i--) {
        out << "Ticket ID     : " << cancelStack[i].ticketID << "\n";
        out << "Passenger Name: " << cancelStack[i].passengerName << "\n";
        out << "Source        : " << cancelStack[i].source << "\n";
        out << "Destination   : " << cancelStack[i].destination << "\n";
        out << "Journey Date  : " << cancelStack[i].journeyDate << "\n";
        out << "Seat No       : " << cancelStack[i].seatNo << "\n";
        out << "Status        : Cancelled\n";
        out << "-----------------------------------\n";
    }
    return out.str();
}

// -------------------- MAIN LOGIC --------------------
string assignSeatToWaitingPassenger() {
    if (!hasAvailableSeat() || isWaitingQueueEmpty()) {
        return "";
    }

    WaitingPassenger p = dequeueWaitingPassenger();
    if (p.requestID == -1) {
        return "";
    }

    int seat = getAvailableSeat();
    insertBookedTicket(
        p.requestID,
        p.passengerName,
        p.source,
        p.destination,
        p.journeyDate,
        seat
    );

    ostringstream out;
    out << "\nSeat automatically assigned to waiting passenger.\n";
    out << "Ticket ID : " << p.requestID << "\n";
    out << "Passenger : " << p.passengerName << "\n";
    out << "Seat No   : " << seat << "\n";
    return out.str();
}

string bookTicket(string name, string src, string dest, string date) {
    if (name.empty() || src.empty() || dest.empty() || date.empty()) {
        return "Please fill all fields.";
    }

    if (src == dest) {
        return "Source and destination cannot be same.";
    }

    ostringstream out;

    if (hasAvailableSeat()) {
        int seat = getAvailableSeat();
        int id = nextTicketID++;

        insertBookedTicket(id, name, src, dest, date, seat);

        out << "Ticket booked successfully.\n";
        out << "Ticket ID : " << id << "\n";
        out << "Passenger : " << name << "\n";
        out << "Source    : " << src << "\n";
        out << "Destination: " << dest << "\n";
        out << "Journey Date: " << date << "\n";
        out << "Seat No   : " << seat << "\n";
        out << "Ticket Confirmed";
    } else {
        enqueueWaitingPassenger(name, src, dest, date);
        out << "No seat available.\n";
        out << "Passenger added to waiting list successfully.";
    }

    return out.str();
}

string cancelTicket(int id) {
    if (head == NULL) {
        return "No booked ticket available to cancel.";
    }

    TicketNode* temp = head;
    TicketNode* prev = NULL;

    while (temp != NULL && temp->ticketID != id) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        return "Ticket not found.";
    }

    pushCancelledTicket(
        temp->ticketID,
        temp->passengerName,
        temp->source,
        temp->destination,
        temp->journeyDate,
        temp->seatNo
    );

    seatOccupied[temp->seatNo] = false;

    if (prev == NULL) {
        head = temp->next;
    } else {
        prev->next = temp->next;
    }

    delete temp;

    ostringstream out;
    out << "Ticket cancelled successfully.\n";

    string autoAssignMsg = assignSeatToWaitingPassenger();
    if (!autoAssignMsg.empty()) {
        out << autoAssignMsg;
    }

    return out.str();
}

string undoLastCancellation() {
    if (isCancelStackEmpty()) {
        return "No cancellation available to undo.";
    }

    CancelledTicket last = popCancelledTicket();

    int seatToAssign = -1;
    if (!seatOccupied[last.seatNo]) {
        seatToAssign = last.seatNo;
    } else {
        seatToAssign = getAvailableSeat();
    }

    if (seatToAssign == -1) {
        pushCancelledTicket(
            last.ticketID,
            last.passengerName,
            last.source,
            last.destination,
            last.journeyDate,
            last.seatNo
        );
        return "Undo not possible because all seats are occupied.";
    }

    insertBookedTicket(
        last.ticketID,
        last.passengerName,
        last.source,
        last.destination,
        last.journeyDate,
        seatToAssign
    );

    ostringstream out;
    out << "Last cancelled ticket restored successfully.\n";
    out << "Ticket ID : " << last.ticketID << "\n";
    out << "Passenger : " << last.passengerName << "\n";
    out << "Seat No   : " << seatToAssign << "\n";

    return out.str();
}

string searchTicketByIDText(int id) {
    TicketNode* result = searchBookedTicket(id);

    if (result == NULL) {
        return "Ticket not found.";
    }

    ostringstream out;
    out << "========== TICKET FOUND ==========\n";
    out << "Ticket ID     : " << result->ticketID << "\n";
    out << "Passenger Name: " << result->passengerName << "\n";
    out << "Source        : " << result->source << "\n";
    out << "Destination   : " << result->destination << "\n";
    out << "Journey Date  : " << result->journeyDate << "\n";
    out << "Seat No       : " << result->seatNo << "\n";
    out << "Status        : Confirmed\n";
    return out.str();
}

string getAvailableSeatsText() {
    ostringstream out;
    out << "Available Seats: ";
    bool found = false;

    for (int i = 1; i <= TOTAL_SEATS; i++) {
        if (!seatOccupied[i]) {
            out << i << " ";
            found = true;
        }
    }

    if (!found) {
        out << "No seats available";
    }

    return out.str();
}

// -------------------- SERVER --------------------
int main() {
    Server svr;

    svr.set_mount_point("/", "./frontend");

    svr.Get("/", [](const Request&, Response& res) {
        res.set_redirect("/index.html");
    });

    svr.Get("/book", [](const Request& req, Response& res) {
        string name = req.has_param("name") ? req.get_param_value("name") : "";
        string source = req.has_param("source") ? req.get_param_value("source") : "";
        string destination = req.has_param("destination") ? req.get_param_value("destination") : "";
        string date = req.has_param("date") ? req.get_param_value("date") : "";

        string result = bookTicket(name, source, destination, date);
        res.set_content(result, "text/plain");
    });

    svr.Get("/cancel", [](const Request& req, Response& res) {
        if (!req.has_param("id")) {
            res.set_content("Ticket ID is required.", "text/plain");
            return;
        }

        int id = stoi(req.get_param_value("id"));
        string result = cancelTicket(id);
        res.set_content(result, "text/plain");
    });

    svr.Get("/undo", [](const Request&, Response& res) {
        string result = undoLastCancellation();
        res.set_content(result, "text/plain");
    });

    svr.Get("/tickets", [](const Request&, Response& res) {
        res.set_content(getAllBookedTicketsText(), "text/plain");
    });

    svr.Get("/waiting", [](const Request&, Response& res) {
        res.set_content(getWaitingListText(), "text/plain");
    });

    svr.Get("/history", [](const Request&, Response& res) {
        res.set_content(getCancelHistoryText(), "text/plain");
    });

    svr.Get("/search", [](const Request& req, Response& res) {
        if (!req.has_param("id")) {
            res.set_content("Ticket ID is required.", "text/plain");
            return;
        }

        int id = stoi(req.get_param_value("id"));
        res.set_content(searchTicketByIDText(id), "text/plain");
    });

    svr.Get("/seats", [](const Request&, Response& res) {
        res.set_content(getAvailableSeatsText(), "text/plain");
    });

    int port = 10000;
    if (const char* p = getenv("PORT")) port = stoi(p);

    cout << "Server running on port " << port << endl;
    svr.listen("0.0.0.0", port);

    return 0;
}
