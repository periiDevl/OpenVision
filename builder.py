import subprocess
import shutil

sln_file_path = r'C:\Users\peric\OneDrive\Documents\dev\OpenVision\Vision_engine.sln'

# Set the path to MSBuild.exe and the solution file
msbuild_path = r"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"

# Build the solution in release mode
result = subprocess.run([msbuild_path, sln_file_path, '/p:Configuration=Release'])
if result.returncode == 0:
    print("Build succeeded!")
else:
    print("Build failed.")


# source file path
src_file_path = "C:/Users/peric/OneDrive/Documents/dev/OpenVision/x64/Release/Vision_engine.exe"

# destination directory path
dest_dir_path = "C:/Users/peric/OneDrive/Documents/dev/OpenVision/"

# move file to destination directory
shutil.move(src_file_path, dest_dir_path)