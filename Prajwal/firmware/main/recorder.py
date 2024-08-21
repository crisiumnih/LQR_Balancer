import matplotlib.pyplot as plt
import subprocess
import time
from collections import deque
import os

# Initialize data structures
time_data = deque(maxlen=10000)  # Adjust maxlen as needed
angle_data = deque(maxlen=10000)  # Adjust maxlen as needed

# Set up the plot
plt.ion()  # Turn on interactive mode
fig, ax = plt.subplots()
line, = ax.plot([], [])
ax.set_xlabel('Time')
ax.set_ylabel('Angle')
ax.set_title('Real-time Angle Data')

# Function to parse incoming data
def parse_data(line, keyword="start"):
    try:
        parts = line.strip().split()
        if len(parts) >= 3 and parts[0] == keyword:
            return float(parts[1]), float(parts[2])
    except ValueError:
        pass
    return None, None

# Ensure data.txt exists
if not os.path.exists('data.txt'):
    open('data.txt', 'a').close()
    print("Created data.txt file")

# Main loop
process = subprocess.Popen(['tail', '-f', 'data.txt'],
                           stdout=subprocess.PIPE,
                           universal_newlines=True)

# Set your desired keyword here
KEYWORD = "recording"  # Change this to whatever keyword you want

try:
    while True:
        line = process.stdout.readline()
        if not line:
            break
        
        time_value, angle_value = parse_data(line, keyword=KEYWORD)
        if time_value is not None and angle_value is not None:
            time_data.append(time_value)
            angle_data.append(angle_value)
            
            # Update the plot
            line.set_data(time_data, angle_data)
            ax.relim()
            ax.autoscale_view()
            plt.draw()
            plt.pause(0.01)  # Small pause to allow plot to update

except KeyboardInterrupt:
    print("Plotting stopped.")

finally:
    process.terminate()
    plt.ioff()
    plt.show()