import subprocess

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