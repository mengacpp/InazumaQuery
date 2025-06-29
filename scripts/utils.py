import subprocess
import os
import winreg
import ctypes


def print_list(items: list):
    for item in items:
        print(item, end=" ")

def run_process(command: list) -> int:
    print("{:8}running".format("[INFO]"), end=" ")
    print_list(command)
    print()

    result = subprocess.run(command, capture_output=True, text=True)
    print(result.stdout)

    if result.returncode != 0:
        print("{:8}execution failed".format("[ERROR]"))
        print(result.stderr)
        return result.returncode

    return result.returncode

def run_processes(commands: list) -> int:
    for cmd in commands:
        result = run_process(cmd)
        if(result != 0):
            return result
    return 0
        

def add_to_user_path(new_path):
    # Normalize path for comparison
    new_path = os.path.normpath(new_path)
    
    # Open the Environment key in the current user registry
    env_key_path = r"Environment"
    try:
        with winreg.OpenKey(winreg.HKEY_CURRENT_USER, env_key_path, 0, winreg.KEY_READ) as key:
            try:
                current_path, _ = winreg.QueryValueEx(key, "PATH")
            except FileNotFoundError:
                current_path = ""
    except Exception as e:
        print(f"Failed to read user PATH from registry: {e}")
        return

    # Split PATH into individual directories, normalized
    path_dirs = [os.path.normpath(p) for p in current_path.split(";") if p.strip()]

    if new_path in path_dirs:
        print(f"The path '{new_path}' is already in your user PATH.")
        return

    # Add new_path at the end (or front, if needed)
    path_dirs.append(new_path)  # or path_dirs.insert(0, new_path) to add at the front
    new_path_value = ";".join(path_dirs)

    try:
        with winreg.OpenKey(winreg.HKEY_CURRENT_USER, env_key_path, 0, winreg.KEY_WRITE) as key:
            winreg.SetValueEx(key, "PATH", 0, winreg.REG_EXPAND_SZ, new_path_value)

        # Broadcast environment change
        HWND_BROADCAST = 0xFFFF
        WM_SETTINGCHANGE = 0x001A
        ctypes.windll.user32.SendMessageW(HWND_BROADCAST, WM_SETTINGCHANGE, 0, "Environment")

        print(f"Successfully added '{new_path}' to user PATH.")
        print("You may need to restart applications to see the effect.")
    except Exception as e:
        print(f"Failed to write user PATH to registry: {e}")