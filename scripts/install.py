import utils
import os

def install(install_prefix: str = None) -> int:
    if install_prefix is None:
        install_prefix = r"\usr\local" if os.name == "nt" else "/usr/local"

    prefix = install_prefix.replace("\\", "/")

    commands = [
        ["cmake", "-B", "build", "-S", ".", "--fresh"],
        ["cmake", "--build", "build"],
        ["cmake", "--install", "build", "--prefix", prefix],
    ]

    return utils.run_processes(commands)

if __name__ == "__main__":
    install()