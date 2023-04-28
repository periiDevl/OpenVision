import subprocess
import shutil
from pathlib import Path
import os


sln_file_path = r'Vision_engine.sln'

# Set the path to MSBuild.exe and the solution file
msbuild_path = r"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"

# Build the solution in release mode
result = subprocess.run([msbuild_path, sln_file_path, '/p:Configuration=Release'])
if result.returncode == 0:
    print("Build succeeded!")
else:
    print("Build failed.")


# Source file path
src_file_path = "x64/Release/Vision_engine.exe"

# Destination directory path
dst_directory_path = os.path.dirname(os.path.abspath(__file__))

# Remove the old file if it exists
if os.path.exists(os.path.join(dst_directory_path, "Vision_engine.exe")):
    os.remove(os.path.join(dst_directory_path, "Vision_engine.exe"))

# Move file to destination directory
shutil.move(src_file_path, dst_directory_path)

# Change the working directory to the destination directory
os.chdir(dst_directory_path)

# Execute the file
while True:
    os.system('Vision_engine.exe')