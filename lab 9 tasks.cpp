#include <iostream>
#include <deque>
#include <vector>
using namespace std;

struct Applicant {
    int id;
    double height;
    double weight;
    double eyesight;
    bool tested;
    Applicant(int _id=0,double h=0,double w=0,double e=0,bool t=false)
      : id(_id), height(h), weight(w), eyesight(e), tested(t) {}
};

struct ANode {
    Applicant a;
    ANode *prev, *next;
    ANode(const Applicant& x): a(x), prev(nullptr), next(nullptr) {}
};

class ApplicantLine {
    ANode *head = nullptr, *tail = nullptr;
    int sz = 0;
public:
    ~ApplicantLine() { clear(); }

    void clear() {
        while (head) {
            ANode* t = head;
            head = head->next;
            delete t;
        }
        tail = nullptr; sz = 0;
    }

    bool empty() const { return sz == 0; }
    int size() const { return sz; }

    // add at back (enqueue)
    void push_back(const Applicant& x) {
        ANode* n = new ANode(x);
        if (!tail) head = tail = n;
        else {
            tail->next = n;
            n->prev = tail;
            tail = n;
        }
        ++sz;
    }

    bool pop_front(Applicant &out) {
        if (!head) return false;
        ANode* t = head;
        out = t->a;
        head = head->next;
        if (head) head->prev = nullptr;
        else tail = nullptr;
        delete t;
        --sz;
        return true;
    }

    bool remove_second(Applicant &out) {
        if (sz < 2) return false;
        ANode* second = head->next;
        out = second->a;
        ANode* third = second->next;
        head->next = third;
        if (third) third->prev = head;
        else tail = head;
        delete second;
        --sz;
        return true;
    }

    void show() const {
        if (!head) { cout << "Line is empty.\n"; return; }
        cout << "Line (front -> back): ";
        for (ANode* cur = head; cur; cur = cur->next) {
            cout << cur->a.id;
            if (cur->next) cout << " -> ";
        }
        cout << "\n";
    }
};

class GarageSystem {
    deque<int> road;     
    vector<int> garage;   
public:
    void on_road(int id) {
        road.push_back(id);
        cout << "On_road(" << id << ")\n";
    }

    void enter_garage_from_road() {
        if (road.empty()) { cout << "No truck on road.\n"; return; }
        int id = road.front(); road.pop_front();
        garage.push_back(id);
        cout << "Enter_garage(" << id << ") from road\n";
    }

    void enter_garage_direct(int id) {
        deque<int> tmp;
        while (!road.empty()) {
            int x = road.front(); road.pop_front();
            if (x != id) tmp.push_back(x);
        }
        road.swap(tmp);
        garage.push_back(id);
        cout << "Enter_garage(" << id << ") direct\n";
    }

    void exit_garage(int id) {
        if (garage.empty()) { cout << "Garage is empty.\n"; return; }
        int top = garage.back();
        if (top != id) {
            cout << "Truck is not near garage door.\n";
            return;
        }
        garage.pop_back();
        cout << "Exit_garage(" << id << ")\n";
    }

    void show_trucks(const string &which) const {
        if (which == "garage") {
            if (garage.empty()) { cout << "Garage is empty.\n"; return; }
            cout << "Garage (near door -> deeper): ";
            for (auto it = garage.rbegin(); it != garage.rend(); ++it) {
                cout << *it;
                if (it+1 != garage.rend()) cout << " -> ";
            }
            cout << "\n";
        } else if (which == "road") {
            if (road.empty()) { cout << "Road is empty.\n"; return; }
            cout << "Road (front -> back): ";
            for (size_t i=0;i<road.size();++i) {
                cout << road[i];
                if (i+1<road.size()) cout << " -> ";
            }
            cout << "\n";
        } else cout << "Unknown container. Use 'garage' or 'road'.\n";
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ApplicantLine line;
    for (int i = 1; i <= 7; ++i)
        line.push_back(Applicant(i, 160+i, 50+i, 1.0, false));

    GarageSystem gs;

    while (true) {
        cout << "\nMenu:\n"
             << "1) Show applicants\n"
             << "2) Add applicant at end\n"
             << "3) Front gives test (remove front)\n"
             << "4) Second leaves (remove second)\n"
             << "5) On_road(truck_id)\n"
             << "6) Enter_garage() [from road front]\n"
             << "7) Enter_garage(truck_id) [direct]\n"
             << "8) Exit_garage(truck_id)\n"
             << "9) Show_trucks(garage)\n"
             << "10) Show_trucks(road)\n"
             << "0) Exit\nChoose: ";
        int choice; if (!(cin >> choice)) { cin.clear(); cin.ignore(); continue; }

        if (choice == 0) break;
        if (choice == 1) line.show();
        else if (choice == 2) {
            int id; double h,w,e; int t;
            cout << "Enter id height weight eyesight tested(0/1): ";
            cin >> id >> h >> w >> e >> t;
            line.push_back(Applicant(id,h,w,e,t!=0));
            cout << "Added applicant " << id << "\n";
        } else if (choice == 3) {
            Applicant out;
            if (line.pop_front(out)) cout << "Applicant " << out.id << " removed (gave test).\n";
            else cout << "Line empty.\n";
        } else if (choice == 4) {
            Applicant out;
            if (line.remove_second(out)) cout << "Applicant " << out.id << " (second) left.\n";
            else cout << "Less than 2 applicants.\n";
        } else if (choice == 5) {
            int id; cout << "truck_id: "; cin >> id; gs.on_road(id);
        } else if (choice == 6) {
            gs.enter_garage_from_road();
        } else if (choice == 7) {
            int id; cout << "truck_id: "; cin >> id; gs.enter_garage_direct(id);
        } else if (choice == 8) {
            int id; cout << "truck_id: "; cin >> id; gs.exit_garage(id);
        } else if (choice == 9) {
            gs.show_trucks("garage");
        } else if (choice == 10) {
            gs.show_trucks("road");
        } else cout << "Invalid option.\n";
    }

    cout << "Goodbye.\n";
    return 0;
}

