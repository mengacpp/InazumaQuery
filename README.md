# inazuma-API

A C API for querying and comparing Inazuma Eleven player stats.

## Features

Currently available features:

- Open the Inazuma Eleven 1, 2, and 3 player databases, or import your own
- Filter and sort players to get the one you are looking for.
- Get player information using their full name.

Features I’m working on, planned features, and known bugs:

- Fix nickname encoding in IE3 and IE1 stats to improve accuracy. As of now, some players (affecting fewer than 5% of players) are subject to small decryption bugs.

## Example

Here is an example that shows how powerful this API is.

```C

#include "inazuma/inazuma.h"

// function to compare two players: returns > 0 if p > p2, == 0 if p == p2 and <
// 0 if p < p2. Iin this case we are comparing them in a way that p > p2 
// if p->lvl99_kick > p2->lvl99_kick
int compare_kick(ina_player_t const *p, ina_player_t const *p2)
{
    return p2->lvl99_kick - p->lvl99_kick;
}

// filter function to check if a player has a specific requirement
// in this case we are checking that the player is of type air.
bool filter_air_element(ina_player_t const *p)
{
    return p->element == ELEMENT_AIR;
}

int main()
{
    // open the Inazuma eleven 3 player database
    ina_db_t *ie3_db = ina_db_create_from_csv(INA_IE3_CSV);

    // get the top 100 players of element air sorted 
    // by their kick stat at lvl99.
    ina_list_t *players =
        ina_db_query(ie3_db, 100, filter_air_element, compare_kick);


    // print all players found
    ina_list_fprint(players, stdout, ina_player_list_printer, "\n");

    // cleanup
    ina_list_destroy(&players);
    ina_db_close(&ie3_db);

    return 0;
}
```

For more examples you can take a look at the [examples](https://github.com/mengacpp/inazuma-api/tree/main/examples) folder.

## Dependencies

The library does not depend on any external libraries. However, to install it on your device you will need the following tools installed on your device:

- Git
- CMake
- Python (required only for a fast installation)

## Installation

> [!WARNING]
> The API is still in beta; there may be bugs during installation. As of now, I recommend not to install it in system paths if you don't know what you're doing.

You can install Inazuma-API on your device using two methods. The installation process is the same on every system.

### Install using Python

Follow this step-by-step guide to install Inazuma-API on your device in 2 simple steps. Make sure you have [required dependencies](#dependencies) installed on your device.

1. **Clone the repostory on your device**

    You can clone this repository on your device using git:

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

    You can use CMake to install the API after you've built it. Specify the installation path using the `--prefix` option, or you can use the default one.

    ```terminal
    cmake --install build/inazuma --prefix your/installation/path
    ```

## Credits

Player stats and data used in this project were sourced from [SwareJonge](https://github.com/SwareJonge)'s comprehensive [Inazuma Eleven databases](https://docs.google.com/spreadsheets/d/1qfanvDyPubSLyfcOMuXN9IbGtr7U1jr-5FRCf2R7FQA/edit?gid=469737450#gid=469737450). Huge thanks to their work!
