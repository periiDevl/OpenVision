import subprocess
import sys
# Path to Visual Studio 2022 executable
vs_exe_path = r'E:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.exe'

# Path to solution file
sln_file_path = r'Vision_engine.sln'

# Version of MSBuild tools to use
msbuild_version = '16.0'
# Start Visual Studio 2022 with solution file, set build tools version, and run it in release mode
subprocess.run([vs_exe_path, sln_file_path, '/Command', 'Tools.Options.ProjectAndSolution.BuildAndRun.BuildToolsVersion=' + msbuild_version, '/Run', 'Release'], check=True)
