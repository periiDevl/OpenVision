import subprocess
import shutil
import os
import time
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler
import tkinter as tk
import threading

def get_data_from_file(file_path):
    data = None
    with open(file_path, "r") as f:
        data = f.read()
    return data

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
        # Set maximum CPU count for parallel builds
        result = subprocess.run([self.msbuild_path, self.sln_file_path, '/p:Configuration=Release', '/maxcpucount'])
        if result.returncode == 0:
            print("Build succeeded!")
            self.execute()
            # Update last build time after a successful build
            self.last_build_time = time.time()
        else:
            print("Build failed.")

    def execute(self):
        src_file_path = "x64/Release/Vision_engine.exe"
        dst_file_path = os.path.join(self.dst_directory_path, "Vision_engine.exe")

        # Remove (delete) the existing "Vision_engine.exe" if it exists
        if os.path.exists(dst_file_path):
            try:
                os.remove(dst_file_path)
            except PermissionError:
                # If the file is still in use, you can try terminating the process
                # using a platform-specific method. For Windows, you can use taskkill.
                # For Unix-based systems, you can use pkill or terminate the process using its PID.
                pass

        # Replace the original "Vision_engine.exe" with the new version
        shutil.move(src_file_path, dst_file_path)

        os.chdir(self.dst_directory_path)

        print("Executing...")
        subprocess.Popen(['Vision_engine.exe'])

    def on_modified(self, event):
        if event.is_directory or not event.src_path.endswith('.cpp'):
            return

        print(f"Detected change in {event.src_path}. Rebuilding...")
        self.file_timestamps[event.src_path] = time.time()  # Update the file's modified timestamp
        self.schedule_build()

    def schedule_build(self):
        # Delay the build for 1 second to avoid redundant builds for multiple changes
        def delayed_build():
            time.sleep(1)
            self.build()

        with self.build_lock:
            threading.Thread(target=delayed_build).start()

if __name__ == '__main__':
    # Your existing code for the Tkinter GUI and initialization remains unchanged.

    msbuild_path = get_data_from_file("MsbuildPATH.ov")
    sln_file_path = r'Vision_engine.sln'
    dst_directory_path = os.path.dirname(os.path.abspath(__file__))

    handler = HotReloadHandler(msbuild_path, sln_file_path, dst_directory_path)

    # Create the Watchdog observer
    observer = Observer()
    observer.schedule(handler, '.', recursive=True)

    # Use a separate thread for the Watchdog observer
    observer_thread = threading.Thread(target=observer.start)
    observer_thread.daemon = True
    observer_thread.start()

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        observer.stop()

    observer.join()
