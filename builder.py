import subprocess
import shutil
from pathlib import Path
import os
import time
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler
import tkinter as tk
import threading

class HotReloadHandler(FileSystemEventHandler):
    def __init__(self, msbuild_path, sln_file_path, dst_directory_path):
        super().__init__()
        self.msbuild_path = msbuild_path
        self.sln_file_path = sln_file_path
        self.dst_directory_path = dst_directory_path
        self.last_build_time = time.time()
        self.file_timestamps = {}
        self.build_lock = threading.Lock()
        self.build()

    def build(self):
        print("Building...")
        result = subprocess.run([self.msbuild_path, self.sln_file_path, '/p:Configuration=Release'])
        if result.returncode == 0:
            print("Build succeeded!")
            self.execute()
            # Update last build time after a successful build
            self.last_build_time = time.time()
        else:
            print("Build failed.")

    def execute(self):
        src_file_path = "x64/Release/Vision_engine.exe"

        if os.path.exists(os.path.join(self.dst_directory_path, "Vision_engine.exe")):
            os.remove(os.path.join(self.dst_directory_path, "Vision_engine.exe"))

        shutil.move(src_file_path, self.dst_directory_path)

        os.chdir(self.dst_directory_path)

        print("Executing...")
        os.system('Vision_engine.exe')

    def on_modified(self, event):
        if event.is_directory or not event.src_path.endswith('.cpp'):
            return

        # Check if the file has been modified since the last build
        if event.src_path in self.file_timestamps:
            last_modified_time = self.file_timestamps[event.src_path]
            current_time = time.time()
            if current_time - last_modified_time < 1.0:  # Within 1 second, batch changes
                return

        print(f"Detected change in {event.src_path}. Rebuilding...")
        self.file_timestamps[event.src_path] = time.time()  # Update the file's modified timestamp
        self.schedule_build()

    def schedule_build(self):
        # Delay the build for 1 second to batch multiple changes together
        time.sleep(0)
        # Ensure only one thread can build at a time
        with self.build_lock:
            self.build()

def get_data_from_file(file_path):
    data = None

    with open(file_path, "r") as f:
        data = f.read()

    return data

def write_to_file():
    text = entry_field.get()

    if os.path.exists("MsbuildPATH.ov"):
        with open("MsbuildPATH.txt", "w") as file:
            file.truncate(0)

    file = open("MsbuildPATH.ov", "w")

    file.write(text)

    file.close()

def check_if_file_exists(file_name):
    if os.path.isfile(file_name):
        return True
    else:
        return False

if __name__ == '__main__':
    if (not check_if_file_exists(get_data_from_file("MsbuildPATH.ov"))):
        window = tk.Tk()
        lb = tk.Label(text="Please specify the MSBuild.exe path.")
        lb.pack()
        entry_field = tk.Entry(window)

        write_button = tk.Button(window, text="Approve", command=lambda: write_to_file() and window.destroy())
        exit_button = tk.Button(window, text="Exit", command=window.destroy)

        entry_field.pack()
        write_button.pack()
        exit_button.pack()
        window.mainloop()

    print(get_data_from_file("MsbuildPATH.ov"))
    print(check_if_file_exists(get_data_from_file("MsbuildPATH.ov")))

    msbuild_path = get_data_from_file("MsbuildPATH.ov")
    sln_file_path = r'Vision_engine.sln'

    dst_directory_path = os.path.dirname(os.path.abspath(__file__))

    handler = HotReloadHandler(msbuild_path, sln_file_path, dst_directory_path)

    observer = Observer()
    observer.schedule(handler, '.', recursive=True)
    observer.start()

    try:
        while True:
            time.sleep(0)
    except KeyboardInterrupt:
        observer.stop()

    observer.join()
