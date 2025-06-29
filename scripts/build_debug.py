import cmake_utils as inazuma
from utils import exit_if_one_fails as check

if __name__ == "__main__":

    specifications = inazuma.CmakeSpecifications(fresh_generation=True, build_type="Debug", build_tests=False)

    inazuma.specifications = specifications

    check([
        inazuma.generate(),
        inazuma.compile_and_link()    
    ])
    