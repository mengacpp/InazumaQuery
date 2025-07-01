# inazuma-api

A C++ API for querying and comparing Inazuma Eleven player stats.

## Dependencies

The library is not depending on any other external library. However, to install it on your device you'll need installed on your device.

- Git
- CMake
- Python (Required only if you want a 'fast' installation)

## Installation

> [!WARNING]
> The API it's still in its beta, there might be bugs during the installation process. As of now, i recommend not to install it in system paths if you don't know what you're doing.

You can install Inazuma-API on your device using two methods. The installation process is the same on every system.

### Install using python

Follow this step-by-step guide to install Inazuma-API on your device in 2 simple steps. Make sure you have [required dependencies](#dependencies) installed on your device.

1. **Clone the repostory on your device**

    You can clone this repostory on your device using git:

    ```terminal
    git clone https://github.com/mengacpp/inazuma-api.git
    ```

2. **Install the library**

    Install the library on your device by running the `scripts/install.py` script. You can specify the install directory by passing it as an argument, or use the default one by passing nothing.

    ```terminal
    python scripts/install.py your/installation/path
    ```

    In the [Scripts](https://github.com/mengacpp/inazuma-api/tree/main/scripts) folder you'll find more useful scripts like `test.py` and `build_release.py`.

### Install using CMake

If you don't have python installed on your device, or you don't want to use it, you can follow these steps:

1. **Clone the repostory on your device**

    You can clone this repostory on your device using git:

    ```terminal
    git clone https://github.com/mengacpp/inazuma-api.git
    ```

2. **Generate and build the project**

    Use CMake to build the project:

    ```terminal
    cmake -B build -S . 
    cmake --build build
    ```

3. **Install the library on your device**

    You can use cmake to install the API after you've built it. Specify the installation path using the `--prefix` option, or you can use the default one.

    ```terminal
    cmake --install build/inazuma --prefix your/installation/path
    ```

## Credits

Player stats and data used in this project were sourced from [SwareJonge](https://github.com/SwareJonge)'s comprehensive [Inazuma Eleven databases](https://docs.google.com/spreadsheets/d/1qfanvDyPubSLyfcOMuXN9IbGtr7U1jr-5FRCf2R7FQA/edit?gid=469737450#gid=469737450). Huge thanks to their work!
