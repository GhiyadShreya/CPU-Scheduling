#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <bits/stdc++.h>

using namespace std;

/** This file handles parsing the data we are going to work with **/
/** It also holds all the global variables we parse into         **/

// Global variables
string operation;                 // Operation mode (e.g., TRACE or SHOW_STATISTICS)
int last_instant;                // The time unit until which processes are executed
int process_count;               // Number of processes to be scheduled
vector<pair<char, int>> algorithms;  // List of algorithms with their IDs and quantum values
vector<tuple<string,int,int>> processes; // List of processes with name, arrival time, and service time
vector<vector<char>> timeline;       // Timeline representation of process scheduling
unordered_map<string,int> processToIndex; // Mapping from process names to their indices

// Results
vector<int> finishTime;             // Vector to store finish times for each process
vector<int> turnAroundTime;         // Vector to store turnaround times for each process
vector<float> normTurn;             // Vector to store normalized turnaround times for each process

/**
 * Parses the algorithm chunk to extract scheduling algorithms and their parameters.
 * @param algorithm_chunk: A string containing algorithms and their quantum values.
 */
void parse_algorithms(string algorithm_chunk)
{
    stringstream stream(algorithm_chunk);
    while (stream.good())
    {
        string temp_str;
        getline(stream, temp_str, ',');   // Split by comma
        stringstream ss(temp_str);
        getline(ss, temp_str, '-');        // Extract algorithm ID
        char algorithm_id = temp_str[0];
        getline(ss, temp_str, '-');        // Extract quantum value
        int quantum = temp_str.size() >= 1 ? stoi(temp_str) : -1; // Convert to integer or set to -1 if empty
        algorithms.push_back(make_pair(algorithm_id, quantum));   // Store the algorithm and its quantum
    }
}

/**
 * Parses the process data from the input.
 */
void parse_processes()
{
    string process_chunk, process_name;
    int process_arrival_time, process_service_time;
    for(int i = 0; i < process_count; i++)
    {
        cin >> process_chunk; // Read process data from standard input

        stringstream stream(process_chunk);
        string temp_str;
        getline(stream, temp_str, ',');    // Extract process name
        process_name = temp_str;
        getline(stream, temp_str, ',');    // Extract arrival time
        process_arrival_time = stoi(temp_str);
        getline(stream, temp_str, ',');    // Extract service time
        process_service_time = stoi(temp_str);

        processes.push_back(make_tuple(process_name, process_arrival_time, process_service_time)); // Store process info
        processToIndex[process_name] = i;  // Map process name to its index
    }
}

/**
 * Parses the input data to initialize the algorithms, processes, and other variables.
 */
void parse()
{
    string algorithm_chunk;
    cin >> operation >> algorithm_chunk >> last_instant >> process_count; // Read operation mode, algorithm chunk, and counts
    parse_algorithms(algorithm_chunk);  // Parse algorithm data
    parse_processes();                 // Parse process data
    finishTime.resize(process_count);   // Resize finish time vector
    turnAroundTime.resize(process_count); // Resize turnaround time vector
    normTurn.resize(process_count);     // Resize normalized turnaround time vector
    timeline.resize(last_instant);      // Resize timeline vector
    for(int i = 0; i < last_instant; i++)
        for(int j = 0; j < process_count; j++)
            timeline[i].push_back(' '); // Initialize timeline with empty spaces
}

#endif // PARSER_H_INCLUDED
