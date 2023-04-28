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


# source file path
src_file_path = "x64/Release/Vision_engine.exe"
# destination directory path

# move file to destination directory
os.remove("Vision_engine.exe")
shutil.move(src_file_path, os.path.dirname(os.path.abspath(__file__)))

os.system('Vision_engine.exe')