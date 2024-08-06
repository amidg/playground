#include <iostream>
using namespace std;
#include <vector>
#include <algorithm>

//Define the structs Workshops and Available_Workshops.
//Implement the functions initialize and CalculateMaxWorkshops
typedef struct Workshop {
    int start_time; // cannot be more than 12
    int duration;
    int end_time; // technically cannot be more than 12
} Workshop;

typedef struct Available_Workshop {
    int n; // number of workshops
    Workshop *workshops; // technically a pointer
} Available_Workshops;

inline bool operator<(const Workshop& lhs, const Workshop& rhs) {
    return (lhs.end_time <= rhs.start_time);
}

inline bool operator>(const Workshop& lhs, const Workshop& rhs) {
    return (lhs.start_time >= rhs.end_time);
}

Available_Workshops* initialize(int start_times[], int duration[], int n) {
    Available_Workshops* workshops = new Available_Workshops; // static so that actual memory space does not go out of scope
    workshops->n = n;
    workshops->workshops = new Workshop[n];
    for (int i = 0; i < n; i++) {
        workshops->workshops[i].start_time = start_times[i];
        workshops->workshops[i].duration = duration[i];
        workshops->workshops[i].end_time = start_times[i] + duration[i];
    }
    return workshops;
}

int CalculateMaxWorkshops(Available_Workshops* ptr) {
    std::vector<Workshop> good = {ptr->workshops[0]};
    bool is_good = true;
    Workshop temp;
    for (int i = 1; i < ptr->n; i++) {
        is_good = true;
        temp = ptr->workshops[i];
        for (auto j = good.begin(); j != good.end(); j++) { // gonna be O(N^2) anyway?
            if ( (temp.start_time >= (*j).start_time && temp.start_time <= (*j).end_time) ||
                 (temp.end_time >= (*j).start_time && temp.end_time <= (*j).end_time) ||
                 (temp.start_time == (*j).start_time || temp.end_time == (*j).end_time) ){
                is_good = false;
                break;
            } 
        }
        if (is_good)
            good.push_back(temp);
    }
    //for (auto& i : good)
    //    std::cout << i.start_time << " " << i.duration << std::endl;
    return good.size();
}

int main(int argc, char *argv[]) {
    int n; // number of workshops
    cin >> n;
    // create arrays of unknown size n
    int* start_time = new int[n];
    int* duration = new int[n];

    for(int i=0; i < n; i++){
        cin >> start_time[i];
    }
    for(int i = 0; i < n; i++){
        cin >> duration[i];
    }

    Available_Workshops * ptr;
    ptr = initialize(start_time,duration, n);
    cout << CalculateMaxWorkshops(ptr) << endl;
    return 0;
}

