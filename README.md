# System Monitor Tool 

A Linux-based C++ System Monitor Tool that displays real-time information about CPU usage, memory utilization, and active processes, similar to the 'top' command.  
It also allows users to terminate processes safely and updates dynamically every few seconds.

---

## Objective
To create a console-based system monitor in C++ using Linux System Programming (LSP) concepts such as:
- Reading data from the /proc filesystem  
- Using system calls for process management  
- Displaying real-time system metrics

---

## Features
- Displays total, free, and used memory in real time  
- Lists top 10 processes by memory usage  
- Sorts processes dynamically based on memory consumption  
- Allows user to kill processes by PID  
- Color-coded output for better readability  
- Automatically refreshes every few seconds  
- Fully implemented using Linux System Programming

---

## Technologies Used
  Language : C++ 
  Platform : Ubuntu (via WSL on Windows) 
  
---

## How to Run

1. Open your Ubuntu (WSL) terminal.
2. Clone this repository:
    ```bash
   git clone https://github.com/Rushali222/System-Monitor-Tool.git
   cd System-Monitor-Tool
   ```    
3. Compile the code:
   g++ system_monitor.cpp -o monitor
4. Run the tool:
   ./monitor
5. The tool will refresh automatically every 5 seconds.
   Press Ctrl + C to stop.

   
## Author
Rushali Gupta
B.Tech, Computer Science and Engineering
ITER, SOA University (Batch 2026)
   
