import proc

commands = [["cmake", "-B", "build", "-S", "."],
            ["cmake", "--build", "build"]]

exit(proc.run_processes(commands))
