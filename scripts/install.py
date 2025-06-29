#!/usr/bin/env python3
import sys
import cmake_utils as inazuma
from utils import exit_if_one_fails as check

if __name__ == "__main__":
    specs = inazuma.CmakeSpecifications(
        fresh_generation=True
    )

    if len(sys.argv) >= 2:
        specs.install_prefix = sys.argv[1]

    inazuma.specifications = specs

    check([
        inazuma.generate(),
        inazuma.compile_and_link(),
        inazuma.install()
        ])
