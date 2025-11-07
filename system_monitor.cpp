#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <algorithm>
#include <cctype>
#include <iomanip>

using namespace std;

// ANSI color codes
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define CYAN "\033[36m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RED "\033[31m"

struct ProcessInfo {
    int pid;
    string command;
    double cpuUsage;
    double memUsage;
};

// Get total memory info
void getMemoryInfo(long long &totalMem, long long &freeMem, long long &availMem) {
    ifstream file("/proc/meminfo");
    string key;
    long long value;
    string unit;
    totalMem = freeMem = availMem = 0;
    while (file >> key >> value >> unit) {
        if (key == "MemTotal:") totalMem = value;
        else if (key == "MemFree:") freeMem = value;
        else if (key == "MemAvailable:") availMem = value;
    }
    file.close();
}

// Get list of processes
vector<ProcessInfo> getProcesses(long long totalMem) {
    vector<ProcessInfo> processes;
    DIR *dir = opendir("/proc");
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (isdigit(entry->d_name[0])) {
            int pid = stoi(entry->d_name);
            string cmdPath = "/proc/" + to_string(pid) + "/cmdline";
            ifstream cmdFile(cmdPath);
            string cmd;
            getline(cmdFile, cmd);
            if (cmd.empty()) cmd = "[kernel process]";

            string statmPath = "/proc/" + to_string(pid) + "/statm";
            ifstream statm(statmPath);
            long long size = 0, resident = 0;
            statm >> size >> resident;
            statm.close();

            double memUsageKB = (double)resident * sysconf(_SC_PAGE_SIZE) / 1024.0;
            double memPercent = (memUsageKB / (double)totalMem) * 100.0;

            ProcessInfo p = {pid, cmd, 0.0, memPercent};
            processes.push_back(p);
        }
    }
    closedir(dir);
    return processes;
}

int main() {
    while (true) {
        system("clear");
        cout << CYAN << BOLD << "==================== SYSTEM MONITOR TOOL ====================" << RESET << endl;

        // Memory info
        long long totalMem, freeMem, availMem;
        getMemoryInfo(totalMem, freeMem, availMem);
        double memUsedPercent = (1.0 - (double)availMem / totalMem) * 100.0;

        cout << "\n" << GREEN << BOLD << "Memory Info:" << RESET << endl;
        cout << "Total: " << totalMem / 1024 << " MB  |  Free: " << freeMem / 1024
             << " MB  |  Used: " << fixed << setprecision(2) << memUsedPercent << "%" << endl;

        // Process info
        vector<ProcessInfo> processes = getProcesses(totalMem);

        // Sort by memory usage
        sort(processes.begin(), processes.end(), [](const ProcessInfo &a, const ProcessInfo &b) {
            return a.memUsage > b.memUsage;
        });

        cout << "\n" << YELLOW << BOLD << "Top 10 Processes (by Memory Usage):" << RESET << endl;
        cout << BOLD << "PID\tMEM%\tCOMMAND" << RESET << endl;
        cout << "---------------------------------------------------------------" << endl;

        int count = 0;
        for (auto &p : processes) {
            string color = (p.memUsage > 10.0) ? RED : GREEN;
            cout << color << p.pid << "\t" << fixed << setprecision(2) << p.memUsage << "\t"
                 << p.command.substr(0, 40) << RESET << endl;
            if (++count >= 10) break;
        }

        // Kill process option
        cout << "\n" << YELLOW << BOLD << "Enter PID to kill (or 0 to skip): " << RESET;
        int pidToKill;
        cin >> pidToKill;
        if (pidToKill > 0) {
            if (kill(pidToKill, SIGTERM) == 0)
                cout << GREEN << "Process " << pidToKill << " terminated successfully." << RESET << endl;
            else
                perror(RED "Failed to kill process" RESET);
        }

        cout << "\n" << CYAN << "(Refreshing every 5 seconds... Press Ctrl+C to stop)" << RESET << endl;
        sleep(5);
    }

    return 0;
}

