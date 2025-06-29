import utils
import sys
import os

def install(install_prefix: str = "\\usr\\local") -> int:
    commands = [
        ["cmake", "-B", "build", "-S", ".", "--fresh"],
        ["cmake", "--build", "build"],
        ["cmake", "--install", "build", "--prefix", install_prefix]
    ]

    result = utils.run_processes(commands)

    if result != 0:
        return result

    # bin_path = os.path.join(install_prefix, "bin")

    # print("{:8}adding {} to user path".format("[INFO]", bin_path))
    # utils.add_to_user_path(bin_path)
    return 0

if __name__ == "__main__":
    install()