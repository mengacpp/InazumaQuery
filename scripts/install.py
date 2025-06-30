#!/usr/bin/env python3
import sys
import cmake_utils as inazuma
from utils import exit_if_one_fails as check

if __name__ == "__main__":

    inazuma.specifications = inazuma.CmakeSpecifications(
        fresh_generation=True,
        build_tests=False
    )

    if len(sys.argv) >= 2:
        inazuma.specifications.install_prefix = sys.argv[1]

    check([
        inazuma.generate(),
        inazuma.compile_and_link(),
        inazuma.install()
        ])
