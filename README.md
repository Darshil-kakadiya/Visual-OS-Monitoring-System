# Virtual OS Monitoring System 🚀

A high-performance, hybrid monitoring tool built with **C** and **Shell Scripting**. It features real-time system metrics, activity-based intelligence, and an AI-driven process scheduler.

---

## ✨ Key Features

- **Real-Time Monitoring**: Accurate tracking of CPU load, memory usage, and process states.
- **Activity-Based Intelligence**: Automatically categorizes processes as "Interactive" or "Batch" based on UI interaction and resource consumption.
- **AI Scheduler Insights**: Dynamically selects the best scheduling algorithm (FCFS, SJF, SRTN, Priority, or Round Robin) based on system load.
- **Predictive Analysis**: Uses Simple Moving Average (SMA) to predict future CPU load.
- **Anomaly Detection**: Visual alerts for high-load "STRESS" conditions.

## 🏗️ System Architecture

The project employs a hybrid architecture for maximum efficiency:
1.  **Shell Engine (`monitor.sh`)**: Captures low-level system metrics using Linux utilities (`top`, `ps`, `free`) and performs lightweight predictive analysis.
2.  **C Core (`main.c`)**: The central processing unit that parses metrics, runs the AI-based scheduling logic, and renders a premium terminal UI.
3.  **Communication**: Data is exchanged via a lightweight `output.txt` file for minimal overhead.

## 🚀 Getting Started

### Prerequisites

- **OS**: Linux (Ubuntu/Debian recommended) or WSL (Windows Subsystem for Linux).
- **Tools**: `gcc`, `make`, `bash`, `bc`.

### Installation

1.  **Clone the repository**:
    ```bash
    git clone https://github.com/your-username/virtual-os-monitor.git
    cd virtual-os-monitor
    ```

2.  **Build the project**:
    ```bash
    make
    ```

3.  **Run the monitor**:
    ```bash
    make run
    ```

## 📁 Project Structure

- `main.c`: The core C engine for UI and scheduling logic.
- `monitor.sh`: Shell script for real-time metric collection.
- `Makefile`: Build system for easy compilation and execution.
- `output.txt`: Intermediate data exchange file.
- `cpu_history.txt`: History file for CPU load prediction.

## 🛠️ Build Commands

- `make`: Compiles the C program and sets permissions for the script.
- `make run`: Builds and executes the monitor immediately.
- `make clean`: Removes binary files and temporary logs.

---

*Built with ❤️ for better system observability.*
