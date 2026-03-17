#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PROCESSES 10
#define TIME_QUANTUM 2

/**
 * main.c - Virtual OS Monitoring System (Ultra-Accuracy & Full Scheduling)
 */

typedef struct {
    int pid;
    char name[20];
    int at, bt, rt;
    int ct, tat, wt;
    int priority;
    float cpu_percent;
    float mem_percent;
    char state[15];
    char type[15]; 
} Process;

// --- Scheduling Algorithms ---

void fcfs(Process *p, int n) {
    int current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < p[i].at) current_time = p[i].at;
        p[i].wt = current_time - p[i].at;
        current_time += p[i].bt;
        p[i].ct = current_time;
        p[i].tat = p[i].ct - p[i].at;
    }
}

void sjf(Process *p, int n) {
    int completed = 0, current_time = 0;
    bool finished[MAX_PROCESSES] = {0};
    while (completed < n) {
        int idx = -1, min_bt = 1e9;
        for (int i = 0; i < n; i++) {
            if (p[i].at <= current_time && !finished[i] && p[i].bt < min_bt) {
                min_bt = p[i].bt;
                idx = i;
            }
        }
        if (idx != -1) {
            current_time += p[idx].bt;
            p[idx].ct = current_time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            finished[idx] = true;
            completed++;
        } else current_time++;
    }
}

void srtn(Process *p, int n) {
    int completed = 0, current_time = 0;
    int rem_bt[MAX_PROCESSES];
    for (int i = 0; i < n; i++) rem_bt[i] = p[i].bt;
    while (completed < n) {
        int idx = -1, min_rem = 1e9;
        for (int i = 0; i < n; i++) {
            if (p[i].at <= current_time && rem_bt[i] > 0 && rem_bt[i] < min_rem) {
                min_rem = rem_bt[i];
                idx = i;
            }
        }
        if (idx != -1) {
            rem_bt[idx]--;
            current_time++;
            if (rem_bt[idx] == 0) {
                p[idx].ct = current_time;
                p[idx].tat = p[idx].ct - p[idx].at;
                p[idx].wt = p[idx].tat - p[idx].bt;
                completed++;
            }
        } else current_time++;
    }
}

void priority_sched(Process *p, int n) {
    int completed = 0, current_time = 0;
    bool finished[MAX_PROCESSES] = {0};
    while (completed < n) {
        int idx = -1, highest_prio = 1e9;
        for (int i = 0; i < n; i++) {
            if (p[i].at <= current_time && !finished[i] && p[i].priority < highest_prio) {
                highest_prio = p[i].priority;
                idx = i;
            }
        }
        if (idx != -1) {
            current_time += p[idx].bt;
            p[idx].ct = current_time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            finished[idx] = true;
            completed++;
        } else current_time++;
    }
}

void rr(Process *p, int n) {
    int current_time = 0, completed = 0;
    int rem_bt[MAX_PROCESSES];
    for (int i = 0; i < n; i++) rem_bt[i] = p[i].bt;
    while (completed < n) {
        bool idle = true;
        for (int i = 0; i < n; i++) {
            if (p[i].at <= current_time && rem_bt[i] > 0) {
                idle = false;
                int exc = (rem_bt[i] > TIME_QUANTUM) ? TIME_QUANTUM : rem_bt[i];
                current_time += exc;
                rem_bt[i] -= exc;
                if (rem_bt[i] == 0) {
                    p[i].ct = current_time;
                    p[i].tat = p[i].ct - p[i].at;
                    p[i].wt = p[i].tat - p[i].bt;
                    completed++;
                }
            }
        }
        if (idle) current_time++;
    }
}

int main() {
    float cpu, pred, mem;
    char alert[50];
    Process procs[MAX_PROCESSES];

    while(1) {
        system("bash monitor.sh");
        FILE *fp = fopen("output.txt", "r");
        if(fp == NULL) { usleep(100000); continue; }

        if (fscanf(fp, "%f %f %f %[^\n]", &cpu, &pred, &mem, alert) != 4) {
            fclose(fp); continue;
        }

        int n = 0;
        char stat_str[15];
        while (n < 5 && !feof(fp)) {
            if (fscanf(fp, "%d %s %f %f %s", &procs[n].pid, procs[n].name, &procs[n].cpu_percent, &procs[n].mem_percent, stat_str) == 5) {
                if (procs[n].cpu_percent > 100.0) procs[n].cpu_percent = 100.0;
                procs[n].at = n; 
                procs[n].bt = (int)(procs[n].cpu_percent / 4) + 1;
                procs[n].priority = (int)(10 - procs[n].cpu_percent/10); 
                if (procs[n].priority < 1) procs[n].priority = 1;

                bool is_foreground = (strchr(stat_str, '+') != NULL);
                bool is_running = (stat_str[0] == 'R' || is_foreground || procs[n].cpu_percent > 0.5);

                if (is_running) strcpy(procs[n].state, "Active");
                else strcpy(procs[n].state, "Idle");

                // ACTIVITY-BASED INTELLIGENCE: 
                // A process is "Interactive" only if it's Active AND (Foreground OR High CPU)
                bool is_high_cpu = (procs[n].cpu_percent > 3.0); 
                bool is_known_app = (strstr(procs[n].name, "firefox") || 
                                   strstr(procs[n].name, "whatsapp") || 
                                   strstr(procs[n].name, "chrome") ||
                                   strstr(procs[n].name, "code"));

                if (is_running && (is_foreground || is_high_cpu)) {
                    strcpy(procs[n].type, "Interactive");
                } else {
                    strcpy(procs[n].type, "Batch");
                }

                n++;
            } else fgetc(fp); 
        }
        fclose(fp);

        system("clear");
        printf("\033[1;36m┌────────────────────────────────────────────────────────────────┐\033[0m\n");
        printf("\033[1;36m│   Virtual OS Monitoring System (Activity-Based Intelligence)    │\033[0m\n");
        printf("\033[1;36m└────────────────────────────────────────────────────────────────┘\033[0m\n\n");

        printf("\033[1;97m[System State]\033[0m\n");
        printf("CPU Load  : \033[1;%dm%.1f%%\033[0m (Pred: %.1f%%)\n", (cpu > 80 ? 31 : 32), cpu, pred);
        printf("Memory    : \033[1;34m%.1f%%\033[0m   Status: %s\n\n", mem, (cpu > 80 ? "\033[1;31mSTRESS\033[0m" : "\033[1;32mOPTIMAL\033[0m"));

        printf("\033[1;97m[Active User Processes]\033[0m\n");
        printf("%-8s %-16s %-7s %-7s %-10s %-12s\n", "PID", "NAME", "CPU%", "MEM%", "STATE", "TYPE");
        printf("──────────────────────────────────────────────────────────────────\n");
        for(int i=0; i<n; i++) {
            printf("%-8d %-16s %-7.1f %-7.1f %-10s %-12s\n", 
                   procs[i].pid, procs[i].name, procs[i].cpu_percent, procs[i].mem_percent, procs[i].state, procs[i].type);
        }

        printf("\n\033[1;97m[Scheduling & Resource Analysis]\033[0m\n");
        printf("%-8s %-4s %-4s %-4s %-4s %-4s %-4s\n", "PID", "AT", "BT", "CT", "TAT", "WT", "PRIO");

        char *sched_mode = "FCFS";
        if (cpu > 75) { rr(procs, n); sched_mode = "Round Robin"; }
        else if (n > 0 && procs[0].cpu_percent > 50) { srtn(procs, n); sched_mode = "SRTN"; }
        else if (n > 0 && procs[0].priority < 4) { priority_sched(procs, n); sched_mode = "Priority"; }
        else if (n > 0 && procs[0].bt < 8) { sjf(procs, n); sched_mode = "SJF"; }
        else { fcfs(procs, n); }

        float total_tat = 0, total_wt = 0;
        for(int i=0; i<n; i++) {
            printf("%-8d %-4d %-4d %-4d %-4d %-4d %-4d\n", 
                   procs[i].pid, procs[i].at, procs[i].bt, procs[i].ct, procs[i].tat, procs[i].wt, procs[i].priority);
            total_tat += procs[i].tat;
            total_wt += procs[i].wt;
        }
        
        if (n > 0) {
            printf("──────────────────────────────────────────────────────────────\n");
            printf("AVERAGES:               TAT: \033[1;33m%.2f\033[0m  WT: \033[1;33m%.2f\033[0m\n", 
                   total_tat/n, total_wt/n);
        }

        printf("\n\033[1;33m[AI Scheduler Insights]\033[0m\n");
        printf("Current Algorithm : \033[1;37m%s\033[0m\n", sched_mode);
        printf("Tactical Decision : %s\n", (cpu > 75) ? "Preemptive load balancing active." : "Optimizing for task throughput.");

        printf("\n\033[0;90mRefreshing (5s) • Accurate Metric Integration ON\033[0m\n");
        sleep(5);
    }
    return 0;
}
