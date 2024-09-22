import os

def compile_to_dll(source_folder, output_dll):
  """
  This function attempts to compile a basic C++ project in a folder to a DLL using gcc,
  placing the output files in a new "build" folder and handling missing object files.

  Args:
      source_folder: Path to the folder containing C++ source files (defaults to current directory).
      output_dll: Desired name for the output DLL file.
  """
  # Handle optional source folder argument (defaults to current directory)
  if not source_folder:
    source_folder = os.getcwd()

  # Create the build folder if it doesn't exist
  build_folder = os.path.join(source_folder, "build")
  if not os.path.exists(build_folder):
    os.makedirs(build_folder)

  # Identify .cpp files 
  cpp_files = [f for f in os.listdir(source_folder) if f.endswith(".cpp")]

  # Object file tracking and compilation commands
  object_files = []
  compile_commands = []

  for cpp_file in cpp_files:
    # Object file path
    object_file = os.path.join(build_folder, os.path.splitext(cpp_file)[0] + ".o")
    object_files.append(object_file)

    # Compile command with proper path for object file
    compile_command = f"g++ -c -o {object_file} {os.path.join(source_folder, cpp_file)}"
    compile_commands.append(compile_command)

  # Compile all source files first
  for command in compile_commands:
    os.system(command)

  # Link object files into the DLL (assuming compilation was successful)
  if all(os.path.exists(f) for f in object_files):  # Check if all object files exist
    command = f"g++ {' '.join(object_files)} -o {os.path.join(build_folder, output_dll)} -shared"
    os.system(command)
  else:
    print("Error: Some object files were not created during compilation. Check source files and flags.")

# Example usage (optional argument for source folder)
source_folder = ""  # Leave empty to use current directory
output_dll = "my_cpp_dll.dll"
compile_to_dll(source_folder, output_dll)

print("Compilation complete (with limitations. See script comments).")
