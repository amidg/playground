#include<bits/stdc++.h>

using namespace std;

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

Available_Workshops* initialize(int start_times[], int duration[], int n) {
    Available_Workshops* workshops = new Available_Workshop; // static so that actual memory space does not go out of scope
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
    int max_shops = 1;
    int last = 0; // index
    for (int i = 1; i < ptr->n; i++) {
        if (ptr->workshops[i].start_time >= ptr->workshops[last].end_time || ptr->workshops[i].end_time <= ptr->workshops[last].start_time) {
            max_shops++;
            last = i;
        }
    }
    
    return max_shops;
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

