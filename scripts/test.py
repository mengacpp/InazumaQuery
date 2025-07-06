import subprocess
import os
import shutil
import sys
from typing import List, Union, Optional

def execute_command(
    command: Union[str, List[str]],
    timeout: Optional[float] = None
) -> int:
    """
    Execute the given command, letting stdout/stderr go to the console.

    Args:
        command: Command to execute; a string (runs in shell) or a list of args.
        timeout: Timeout in seconds.

    Returns:
        The process return code, or –1 on error.
        Any errors (timeout, file not found, unexpected) are printed to stderr.
    """
    try:
        completed = subprocess.run(
            command,
            shell=isinstance(command, str),
            timeout=timeout
        )
        return completed.returncode

    except subprocess.TimeoutExpired as e:
        print(f"Error: command '{e.cmd}' timed out after {e.timeout} sec", file=sys.stderr)
        return -1

    except FileNotFoundError as e:
        print(f"Error: {e}", file=sys.stderr)
        return -1

    except Exception as e:
        print(f"Unexpected error: {e}", file=sys.stderr)
        return -1


if __name__ == "__main__":
    if(execute_command("cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DINA_BUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug --fresh") != 0):
        sys.exit(1)


    if(execute_command("cmake --build build") != 0):
        sys.exit(1)

    ctest_cmd = "ctest --test-dir build"
    if sys.argv[1:]:
        ctest_cmd += " " + " ".join(sys.argv[1:])

    if(execute_command(ctest_cmd) != 0):
        if(execute_command("ctest --test-dir build --rerun-failed --output-on-failure") != 0):
            sys.exit(1)