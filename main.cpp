#include <bits/stdc++.h>
#include "parser.h"

#define all(v) v.begin(), v.end()

using namespace std;
/** Global Constants **/
const string TRACE = "trace";
const string SHOW_STATISTICS = "stats";
const string ALGORITHMS[6] = {"", "FCFS", "RR-", "SPN", "SRT", "HRRN"};

//Tuple = {Name,Arrival time, Service time}
// Comparator to sort processes by service time
bool sortByServiceTime(const tuple<string, int, int> &a, const tuple<string, int, int> &b)
{
    return (get<2>(a) < get<2>(b));
}

// Comparator to sort processes by arrival time
bool sortByArrivalTime(const tuple<string, int, int> &a, const tuple<string, int, int> &b)
{
    return (get<1>(a) < get<1>(b));
}

// Comparator to sort by highest response ratio (descending)
bool descendingly_by_response_ratio(tuple<string, double, int> a, tuple<string, double, int> b)
{
    return get<1>(a) > get<1>(b);
}

// Comparator to sort by priority level, breaking ties by service time
bool byPriorityLevel(const tuple<int, int, int> &a, const tuple<int, int, int> &b)
{
    if(get<0>(a) == get<0>(b))
        return get<2>(a) > get<2>(b);
    return get<0>(a) > get<0>(b);
}

// Function to clear the timeline matrix
void clear_timeline()
{
    for(int i = 0; i < last_instant; i++)
        for(int j = 0; j < process_count; j++)
            timeline[i][j] = ' ';
}

// Getter function to extract process name
string getProcessName(tuple<string, int, int> &a)
{
    return get<0>(a);
}

// Getter function to extract arrival time
int getArrivalTime(tuple<string, int, int> &a)
{
    return get<1>(a);
}

// Getter function to extract service time
int getServiceTime(tuple<string, int, int> &a)
{
    return get<2>(a);
}

// Getter function to extract priority level
int getPriorityLevel(tuple<string, int, int> &a)
{
    return get<2>(a);
}

// Function to calculate the response ratio
double calculate_response_ratio(int wait_time, int service_time)
{
    return (wait_time + service_time) * 1.0 / service_time;
}

//TIMELINE MATRIX STRUCTURE
// * = running 
// . = waiting
//Time →     0  1  2  3  4  5
//Process 0   *  *  *  .  .  *
//Process 1   .  .  *  *  *  .

// Function to fill in the wait time for each process on the timeline
void fillInWaitTime() {
    // Loop through all processes
    for (int i = 0; i < process_count; i++) {
        int arrivalTime = getArrivalTime(processes[i]);  // Get the arrival time of the process
        // Mark wait time on the timeline from arrival to finish time
        for (int k = arrivalTime; k < finishTime[i]; k++) {
            if (timeline[k][i] != '*')  // Check if the process is not running at this time
                timeline[k][i] = '.';   // Mark wait time
        }
    }
}

// Function to schedule processes using First-Come, First-Serve (FCFS) algorithm
void firstComeFirstServe() {
    int time = getArrivalTime(processes[0]);  // Initialize time with the arrival time of the first process

    // Loop through all processes
    for (int i = 0; i < process_count; i++) {
        int processIndex = i;
        int arrivalTime = getArrivalTime(processes[i]);  // Get the arrival time of the process
        int serviceTime = getServiceTime(processes[i]);  // Get the service time (CPU burst time) of the process

        finishTime[processIndex] = (time + serviceTime);  // Calculate finish time for the process
        turnAroundTime[processIndex] = (finishTime[processIndex] - arrivalTime);  // Calculate turnaround time
        // Mark the waiting period on the timeline before the process starts
        for (int j = arrivalTime; j < time; j++)
            timeline[j][processIndex] = '.';
        // Mark the process execution on the timeline
        for (int j = time; j < finishTime[processIndex]; j++)
            timeline[j][processIndex] = '*';
        
        time += serviceTime;  // Update the current time
    }
}

// Function to schedule processes using Round Robin algorithm
void roundRobin(int originalQuantum) {
    queue<pair<int, int>> q;  // Queue to manage processes (process index, remaining service time)
    int j = 0;

    // If the first process arrives at time 0, add it to the queue
    if (getArrivalTime(processes[j]) == 0) {
        q.push(make_pair(j, getServiceTime(processes[j])));
        j++;
    }

    int currentQuantum = originalQuantum;  // Set the quantum time for Round Robin

    // Loop through each time unit until the last instant
    // last_instant = global variable in parser.h
    for (int time = 0; time < last_instant; time++) {
        if (!q.empty()) {  // If there are processes in the queue
            int processIndex = q.front().first;
            q.front().second = q.front().second - 1;  // Decrease remaining service time
            int remainingServiceTime = q.front().second;
            int arrivalTime = getArrivalTime(processes[processIndex]);
            int serviceTime = getServiceTime(processes[processIndex]);
            currentQuantum--;

            timeline[time][processIndex] = '*';  // Mark the process execution on the timeline

            // Check for any new processes arriving at the current time
            while (j < process_count && getArrivalTime(processes[j]) == time + 1) {
                q.push(make_pair(j, getServiceTime(processes[j])));
                j++;
            }

            // Handle process completion or quantum expiration
            if (currentQuantum == 0 && remainingServiceTime == 0) {
                finishTime[processIndex] = time + 1;  // Process completed
                turnAroundTime[processIndex] = (finishTime[processIndex] - arrivalTime);  // Calculate turnaround time
                currentQuantum = originalQuantum;  // Reset quantum
                q.pop();  // Remove the process from the queue
            } else if (currentQuantum == 0 && remainingServiceTime != 0) {
                q.pop();  // Move the process to the end of the queue
                q.push(make_pair(processIndex, remainingServiceTime));
                currentQuantum = originalQuantum;  // Reset quantum
            } else if (currentQuantum != 0 && remainingServiceTime == 0) {
                finishTime[processIndex] = time + 1;  // Process completed
                turnAroundTime[processIndex] = (finishTime[processIndex] - arrivalTime);  // Calculate turnaround time
                q.pop();  // Remove the process from the queue
                currentQuantum = originalQuantum;  // Reset quantum
            }
        }

        // Check for any new processes arriving at the current time
        while (j < process_count && getArrivalTime(processes[j]) == time + 1) {
            q.push(make_pair(j, getServiceTime(processes[j])));
            j++;
        }
    }

    fillInWaitTime();  // Fill in wait times on the timeline
}

// Function to schedule processes using Shortest Process Next (SPN) algorithm
void shortestProcessNext() {
    // Min-heap priority queue to select the process with the shortest service time
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; 
    int j = 0;

    // Loop through each time unit until the last instant
    for (int i = 0; i < last_instant; i++) {
        // Add all processes that have arrived by the current time to the priority queue
        while (j < process_count && getArrivalTime(processes[j]) <= i) {
            pq.push(make_pair(getServiceTime(processes[j]), j));
            j++;
        }

        if (!pq.empty()) {  // If there are processes in the queue
            int processIndex = pq.top().second;
            int arrivalTime = getArrivalTime(processes[processIndex]);
            int serviceTime = getServiceTime(processes[processIndex]);
            pq.pop();  // Remove the selected process from the queue

            int temp = arrivalTime;
            // Mark the waiting period on the timeline before the process starts
            for (; temp < i; temp++)
                timeline[temp][processIndex] = '.';

            temp = i;
            // Mark the process execution on the timeline
            for (; temp < i + serviceTime; temp++)
                timeline[temp][processIndex] = '*';

            finishTime[processIndex] = (i + serviceTime);  // Calculate finish time
            turnAroundTime[processIndex] = (finishTime[processIndex] - arrivalTime);  // Calculate turnaround time

            i = temp - 1;  // Move time forward to the end of the process execution
        }
    }
}

void shortestRemainingTime()
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    int j = 0;
    for (int i = 0; i < last_instant; i++)
    {
        while(j<process_count &&getArrivalTime(processes[j]) == i){
            pq.push(make_pair(getServiceTime(processes[j]), j));
            j++;
        }
        if (!pq.empty())
        {
            int processIndex = pq.top().second;
            int remainingTime = pq.top().first;
            pq.pop();
            int serviceTime = getServiceTime(processes[processIndex]);
            int arrivalTime = getArrivalTime(processes[processIndex]);
            timeline[i][processIndex] = '*';

            if (remainingTime == 1) // process finished
            {
                finishTime[processIndex] = i + 1;
                turnAroundTime[processIndex] = (finishTime[processIndex] - arrivalTime);
            }
            else
            {
                pq.push(make_pair(remainingTime - 1, processIndex));
            }
        }
    }
    fillInWaitTime();
}

void highestResponseRatioNext()
{

    // Vector of tuple <process_name, process_response_ratio, time_in_service> for processes that are in the ready queue
    vector<tuple<string, double, int>> present_processes;
    int j=0;
    for (int current_instant = 0; current_instant < last_instant; current_instant++)
    {
        while(j<process_count && getArrivalTime(processes[j])<=current_instant){
            present_processes.push_back(make_tuple(getProcessName(processes[j]), 1.0, 0));
            j++;
        }
        // Calculate response ratio for every process
        for (auto &proc : present_processes)
        {
            string process_name = get<0>(proc);
            int process_index = processToIndex[process_name];
            int wait_time = current_instant - getArrivalTime(processes[process_index]);
            int service_time = getServiceTime(processes[process_index]);
            get<1>(proc) = calculate_response_ratio(wait_time, service_time);
        }

        // Sort present processes by highest to lowest response ratio
        sort(all(present_processes), descendingly_by_response_ratio);

        if (!present_processes.empty())
        {
            int process_index = processToIndex[get<0>(present_processes[0])];
            while(current_instant<last_instant && get<2>(present_processes[0]) != getServiceTime(processes[process_index])){
                timeline[current_instant][process_index]='*';
                current_instant++;
                get<2>(present_processes[0])++;
            }
            current_instant--;
            present_processes.erase(present_processes.begin());
            finishTime[process_index] = current_instant + 1;
            turnAroundTime[process_index] = finishTime[process_index] - getArrivalTime(processes[process_index]);
        }
    }
    fillInWaitTime();
}




void printProcesses()
{
    // Print the header for processes
    cout << "Process    ";
    // Print each process name
    for (int i = 0; i < process_count; i++)
        cout << "|  " << getProcessName(processes[i]) << "  ";
    cout << "|\n";
}

void printArrivalTime()
{
    // Print the header for arrival times
    cout << "Arrival    ";
    // Print the arrival time for each process
    for (int i = 0; i < process_count; i++)
        printf("|%3d  ", getArrivalTime(processes[i]));
    cout << "|\n";
}

void printServiceTime()
{
    // Print the header for service times
    cout << "Service    |";
    // Print the service time for each process
    for (int i = 0; i < process_count; i++)
        printf("%3d  |", getServiceTime(processes[i]));
    cout << " Mean|\n";
}

void printFinishTime()
{
    // Print the header for finish times
    cout << "Finish     ";
    // Print the finish time for each process
    for (int i = 0; i < process_count; i++)
        printf("|%3d  ", finishTime[i]);
    cout << "|-----|\n";
}

void printTurnAroundTime()
{
    // Print the header for turnaround times
    cout << "Turnaround |";
    int sum = 0;
    // Print the turnaround time for each process and calculate the sum
    for (int i = 0; i < process_count; i++)
    {
        printf("%3d  |", turnAroundTime[i]);
        sum += turnAroundTime[i];
    }
    // Print the mean turnaround time, formatted with two decimal places
    if ((1.0 * sum / turnAroundTime.size()) >= 10)
        printf("%2.2f|\n", (1.0 * sum / turnAroundTime.size()));
    else
        printf(" %2.2f|\n", (1.0 * sum / turnAroundTime.size()));
}


void printStats(int algorithm_index)
{
    // Print all relevant statistics for the given scheduling algorithm
    printProcesses();
    printArrivalTime();
    printServiceTime();
    printFinishTime();
    printTurnAroundTime();
}

void printTimeline(int algorithm_index)
{
    // Print the timeline header
    for (int i = 0; i <= last_instant; i++)
        cout << i % 10 << " ";
    cout << "\n";
    cout << "------------------------------------------------\n";
    
    // Print each process's timeline
    for (int i = 0; i < process_count; i++)
    {
        cout << getProcessName(processes[i]) << "     |";
        for (int j = 0; j < last_instant; j++)
        {
            cout << timeline[j][i] << "|";
        }
        cout << " \n";
    }
    cout << "------------------------------------------------\n";
}

void execute_algorithm(char algorithm_id, int quantum, string operation)
{
    // Select the scheduling algorithm based on the algorithm_id and execute it
    switch (algorithm_id)
    {
    case '1':
        // First-Come, First-Served (FCFS)
        if (operation == TRACE) cout << "FCFS  ";
        firstComeFirstServe();
        break;
    case '2':
        // Round Robin (RR) with specified quantum
        if (operation == TRACE) cout << "RR-" << quantum << "  ";
        roundRobin(quantum);
        break;
    case '3':
        // Shortest Process Next (SPN)
        if (operation == TRACE) cout << "SPN   ";
        shortestProcessNext();
        break;
    case '4':
        // Shortest Remaining Time (SRT)
        if (operation == TRACE) cout << "SRT   ";
        shortestRemainingTime();
        break;
    case '5':
        // Highest Response Ratio Next (HRRN)
        if (operation == TRACE) cout << "HRRN  ";
        highestResponseRatioNext();
        break;
    case '6':
        // Feedback Queue 1 (FB-1)
        if (operation == TRACE) cout << "FB-1  ";
        feedbackQ1();
        break;
    case '7':
        // Feedback Queue 2i (FB-2i)
        if (operation == TRACE) cout << "FB-2i ";
        feedbackQ2i();
        break;
    case '8':
        // Aging with specified quantum
        if (operation == TRACE) cout << "Aging ";
        aging(quantum);
        break;
    default:
        // Handle invalid algorithm_id if needed
        break;
    }
}

int main()
{
    // Parse input data and initialize variables
    parse();
    
    // Loop through each algorithm and execute it
    for (int idx = 0; idx < (int)algorithms.size(); idx++)
    {
        // Clear the timeline for each new algorithm execution
        clear_timeline();
        
        // Execute the selected algorithm
        execute_algorithm(algorithms[idx].first, algorithms[idx].second, operation);
        
        // Print the timeline or statistics based on the operation mode
        if (operation == TRACE)
            printTimeline(idx);
        else if (operation == SHOW_STATISTICS)
            printStats(idx);
        
        // Print a newline for better readability
        cout << "\n";
    }
    return 0;
}
