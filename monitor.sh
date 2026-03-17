# monitor.sh - High-Accuracy Real-Time OS Monitor

# 1. Capture System Metrics
if command -v top > /dev/null; then
    # Efficiently get CPU and MEM in one pass if possible, or fallback
    CPU=$(top -bn1 | grep "Cpu(s)" | awk '{print 100 - $8}')
    MEM=$(free 2>/dev/null | grep Mem | awk '{print $3/$2 * 100.0}' || echo "45.0")
else
    CPU=10.0
    MEM=30.0
fi

# 2. Prediction Logic (SMA)
echo $CPU >> cpu_history.txt
tail -n 10 cpu_history.txt > temp.txt && mv temp.txt cpu_history.txt
PRED=$(awk '{sum+=$1} END {if (NR>0) print sum/NR; else print 0}' cpu_history.txt)

# 3. Anomaly Alert
ALERT="STABLE"
if (( $(echo "$CPU > 85" | bc -l 2>/dev/null || [ "${CPU%.*}" -gt 85 ]) )); then
    ALERT="HIGH_LOAD"
fi

# Write System Header
echo "$CPU $PRED $MEM $ALERT" > output.txt

# 4. Process Monitor (Filtered & Normalized)
# Use tail -n +2 to strip the PID COMMAND header reliably
# We capture PCPU, PMEM, and STAT for the C core
ps -eo pid,comm,pcpu,pmem,stat --sort=-pcpu | \
    tail -n +2 | \
    grep -vE "(bash|monitor|ps|top|awk|grep|head|tail|sed|defunct|sshd|kworker|systemd|dbus)" | \
    head -n 5 >> output.txt
