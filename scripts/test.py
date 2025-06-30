import cmake_utils as inazuma
import sys
from utils import exit_if_one_fails as check

if __name__ == "__main__":
    inazuma.specifications = inazuma.CmakeSpecifications(
        fresh_generation=True
        )
    
    args = []

    if len(sys.argv) > 1:
        args = sys.argv[1:]

    check([
        inazuma.generate(),
        inazuma.compile_and_link(),
        inazuma.test(args)
        ])