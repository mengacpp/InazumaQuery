import utils
from typing import Optional

class CmakeSpecifications:
    def __init__(self, 
                 build_dir = "build", 
                 source_dir = ".", 
                 build_type = "Debug", 
                 install_prefix = "/usr/local", 
                 build_tests = True, 
                 fresh_generation = False):
        self.build_dir = build_dir
        self.source_dir = source_dir
        self.build_type = build_type
        self.build_tests = build_tests
        self.fresh_generation = fresh_generation
        self.install_prefix = install_prefix

specifications = CmakeSpecifications()

def generate() -> int:
    if specifications.build_type != "Debug" and specifications.build_type != "Release":
       print(("{:8}Invalid build type '" + (specifications.build_type or "") + "'").format("[ERROR]"))
       return 1

    command = ["cmake", "-B", specifications.build_dir, 
                               "-S", specifications.source_dir, 
                               "-DBUILD_TESTS=" + ("ON" if specifications.build_tests else "OFF"), 
                               "-DCMAKE_BUILD_TYPE=" + specifications.build_type]
    
    if specifications.fresh_generation:
        command.append("--fresh")
    
    return  utils.run_process(command)

def compile_and_link():
    return  utils.run_process(["cmake", "--build", specifications.build_dir])

def install() -> int:

    prefix = specifications.install_prefix.replace("\\", "/")
    return utils.run_process(["cmake", "--install", specifications.build_dir + "/src", "--prefix", prefix])

def test() -> int:

    command = ["ctest", "--test-dir", specifications.build_dir, "--output-on-failure"]

    return utils.run_process(command)
