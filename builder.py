import shutil
import os
import subprocess
import time
# Path to the .exe file you want to run
exe_path = "F:\Development\C++\OpenVision\OpenVision(Engine)\Vision_engine.exe"


    # Start the .exe file
process = subprocess.Popen(exe_path)


    # Terminate the .exe process
process.terminate()  # or process.kill() for forceful termination

# Define the source and destination directories and file names
source_directory = 'F:/Development/C++/OpenVision/OpenVision(Engine)/DynaLL/x64/Debug/'
destination_directory = 'F:/Development/C++/OpenVision/OpenVision(Engine)/'
file_name = 'DynaLL.dll'  # Replace with the actual file name

# Construct the full paths to the source and destination files
source_path = os.path.join(source_directory, file_name)
destination_path = os.path.join(destination_directory, file_name)

# Use shutil.copy2() to copy and replace the file
shutil.copy2(source_path, destination_path)

# Optionally, you can print a message to confirm the operation
print(f"File '{file_name}' copied and replaced successfully.")

# Specify the path to the executable file you want to run

try:
    # Use the subprocess.run() function to start the .exe file
    subprocess.run(exe_path)
    process = subprocess.Popen([exe_path], shell=True, close_fds=True)
except subprocess.CalledProcessError as e:
    print(f"Error: {e}")
except FileNotFoundError:
    print(f"Error: The file '{exe_path}' does not exist.")
