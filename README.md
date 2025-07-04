# InazumaQuery

> [!CAUTION]
> This library it's still in early development phase (pre-alpha). Bugs have to be expected even during the installation phase.

A C API for querying and comparing Inazuma Eleven player stats.

## Table of contents

- [InazumaQuery](#inazumaquery)
  - [Table of contents](#table-of-contents)
  - [Features](#features)
  - [Getting started](#getting-started)
  - [Supported platform](#supported-platform)
  - [Dependencies](#dependencies)
  - [Installation](#installation)
    - [Install using Python scripts](#install-using-python-scripts)
    - [Install using plain CMake](#install-using-plain-cmake)
  - [Credits](#credits)

## Features

**Currently available features:**

- Open the **Inazuma Eleven 1, 2, and 3 player databases** by default, or import your own
- **Filter and sort** players to get the ones you are looking for
- **Look for players** using their full name

**Features I’m currently working on and planned features:**

- Create built in compare functions to sort players by 'skills' rather than stats. (e.g. sorting by general ability to shoot rather than by kick statistic).  
- Adding secondary informations about players (team, recruitment method, recruitment location ..., move ids)

**Known bugs:**

- Some nicknames in the IE3 database and stats in the IE1 database (less than 5% I'd say) are subject to small decryption bugs that might influece accuracy
- Some players are duplicates, this is probably caused by the different versions of the same players depending on the game version

## Getting started

Using the API is straight forward: make sure you have all the required [dependencies](#dependencies), and then follow the [installation instructions](#installation).



I've provided some [examples](https://github.com/mengacpp/InazumaQuery/tree/main/examples) on how to use the library and i'll create a small wiki as soon as the API is in a more stable status and the first official version is released.

## Supported platform

Every platform is theroetically supported, as there are no platform specific dependencies, but only MacOS has been tested right now.

> [!IMPORTANT]
> I will create issue templates in the near future so that you can point out bugs you've found.

## Dependencies

The library does not depend on any external libraries. However, to install it on your device you will need the following tools installed on your device:

- Git
- CMake
- Python (required only for a fast installation)

## Installation

> [!WARNING]
> The API is still in beta; there may be bugs during installation. As of now, I recommend not to install it in system paths if you don't know what you're doing.

You can install InazumaQuery on your device using CMake. To make the process easier, I've also created some python scripts you can use. However, if you want you can still go with the plain CMake installation. **The process is the same on every platform**.

> [!NOTE]
> In most cases, you can avoid installing the library in your system and just include it in your project folder

The API gets built as a shared library to allow compatibility with Python projects even though this hasn't been tested.

### Install using Python scripts

Follow this step-by-step guide to install InazumaQuery on your device in 2 simple steps. Make sure you have [required dependencies](#dependencies) installed on your device.

1. **Clone the repostory on your device**

    You can clone this repository on your device using git:

    ```terminal
    git clone https://github.com/mengacpp/InazumaQuery.git
    ```

2. **Install the library**

    Install the library on your device by running the `scripts/install.py` script. You can specify the install directory by passing it as an argument, or use the default one by passing nothing.

    ```terminal
    python scripts/install.py your/installation/path
    ```

    In the [Scripts](https://github.com/mengacpp/InazumaQuery/tree/main/scripts) folder you'll find more useful scripts like `test.py` and `build_release.py`.

### Install using plain CMake

If you don't have python installed on your device, or you don't want to use it, you can follow these steps:

1. **Clone the repostory on your device**

    You can clone this repostory on your device using git:

    ```terminal
    git clone https://github.com/mengacpp/InazumaQuery.git
    ```

2. **Generate and build the project**

    Use CMake to build the project:

    ```terminal
    cmake -B build -S . 
    cmake --build build
    ```

3. **Install the library on your device**

    You can use CMake to install the API after you've built it. Specify the installation path using the `--prefix` option, or you can use the default one.

    ```terminal
    cmake --install build/inazuma --prefix your/installation/path
    ```

## Credits

- Player stats and data used in this project were sourced from [SwareJonge](https://github.com/SwareJonge)'s comprehensive [Inazuma Eleven databases](https://docs.google.com/spreadsheets/d/1qfanvDyPubSLyfcOMuXN9IbGtr7U1jr-5FRCf2R7FQA/edit?gid=469737450#gid=469737450). Huge thanks to their work!
