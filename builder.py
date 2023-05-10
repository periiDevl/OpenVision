import subprocess
import shutil
from pathlib import Path
import os
import time
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler


class HotReloadHandler(FileSystemEventHandler):
    def __init__(self, msbuild_path, sln_file_path, dst_directory_path):
        super().__init__()
        self.msbuild_path = msbuild_path
        self.sln_file_path = sln_file_path
        self.dst_directory_path = dst_directory_path
        self.build()

    def build(self):
        print("Building...")
        result = subprocess.run([self.msbuild_path, self.sln_file_path, '/p:Configuration=Release'])
        if result.returncode == 0:
            print("Build succeeded!")
            self.execute()
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
        print(f"Detected change in {event.src_path}. Rebuilding...")
        self.build()


if __name__ == '__main__':
    msbuild_path = r"Build\MSBuild\Current\Bin\MSBuild.exe"
    sln_file_path = r'Vision_engine.sln'

    dst_directory_path = os.path.dirname(os.path.abspath(__file__))

    handler = HotReloadHandler(msbuild_path, sln_file_path, dst_directory_path)

    observer = Observer()
    observer.schedule(handler, '.', recursive=True)
    observer.start()

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        observer.stop()

    observer.join()
