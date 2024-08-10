# CPU-Scheduling

An implementation of various CPU scheduling algorithms in C++. The algorithms included are First Come First Serve (FCFS), Round Robin (RR), Shortest Process Next (SPN), Shortest Remaining Time (SRT), Highest Response Ratio Next (HRRN).

## Table of Contents
- [Algorithms](#algorithms)
  - [First Come First Serve (FCFS)](#first-come-first-serve-fcfs)
  - [Round Robin with varying time quantum (RR)](#round-robin-with-varying-time-quantum-rr)
  - [Shortest Process Next (SPN)](#shortest-process-next-spn)
  - [Shortest Remaining Time (SRT)](#shortest-remaining-time-srt)
  - [Highest Response Ratio Next (HRRN)](#highest-response-ratio-next-hrrn)
- [Input Format](#input-format)

## Algorithms

### First Come First Serve (FCFS)
First Come First Served (FCFS) is a non-preemptive scheduling algorithm where the process that arrives first is executed first. It does not prioritize processes based on burst time or priority, which can lead to inefficiencies if a long process arrives before shorter ones.

### Round Robin with varying time quantum (RR)
Round Robin (RR) scheduling is a time-sharing algorithm that assigns a fixed time slice or quantum to each process. This version allows the quantum to vary, which can optimize CPU utilization by adjusting the time slice based on process requirements.

### Shortest Process Next (SPN)
Shortest Process Next (SPN) is a non-preemptive algorithm that selects the process with the shortest burst time for execution. This can minimize average waiting time but may lead to longer processes being starved.

### Shortest Remaining Time (SRT)
Shortest Remaining Time (SRT) is a preemptive version of SPN. It selects processes based on the shortest remaining time, allowing it to interrupt currently running processes if a new process with a shorter remaining time arrives.

### Highest Response Ratio Next (HRRN)
Highest Response Ratio Next (HRRN) is a non-preemptive algorithm that prioritizes processes based on their response ratio, which considers both waiting time and burst time. It aims to balance waiting times and is beneficial when burst times are not known in advance.

## Input Format
1. **Operation Mode:** Line 1 should be `"trace"` or `"stats"`.
2. **Scheduling Policies:** Line 2 should be a comma-separated list specifying the scheduling policies to analyze or visualize, along with their parameters if applicable. Policies are represented as follows:
   - 1: FCFS (First Come First Serve)
   - 2: RR (Round Robin)
   - 3: SPN (Shortest Process Next)
   - 4: SRT (Shortest Remaining Time)
   - 5: HRRN (Highest Response Ratio Next)
   - For Round Robin and Aging, a parameter specifying the quantum `q` is required (e.g., `2-4` for RR with `q=4`).
3. **Last Instant:** Line 3 should specify the integer indicating the last instant to be used in the simulation.
4. **Number of Processes:** Line 4 should specify the number of processes.
5. **Process Descriptions:** Lines 5 and onwards should describe each process using a comma-separated list specifying:
   - Process Name
   - Arrival Time
   - Service Time
