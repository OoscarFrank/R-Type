<div align="center">
    <h1 align="center">
        Installation
    </h1>
</div>

## Description

Building a game like R-Type from its source code is a complex process that requires specific configuration and steps. Here's a brief description of the general steps to build the R-Type game from its source code:

### Step 1: Prerequisites

Before you begin, ensure that you have all the necessary prerequisites installed on your system. This may include:

- A C/C++ development environment (such as g++ for Linux or Visual Studio for Windows).
- A dependency manager such as [CMake](https://cmake.org/).
- The necessary low-level and platform-specific graphical or sound system libraries such as [OpenGL](https://www.opengl.org/) and X11.

### Step 2: Download the Source Code

Make sure you have access to the R-Type game's source code. You can typically download it from a Git repository using the command:

```bash
git clone git@github.com:EpitechPromo2026/B-CPP-500-MLH-5-1-rtype-martin.d-herouville.git
cd B-CPP-500-MLH-5-1-rtype-martin.d-herouville.git
git submodule init
git submodule update
```

These commands will get all the source code from the game and init all the necessary dependencies for the good of the project.

### Step 3: Build Configuration

In our R-Type you have several ways to build the different build tools.

#### CMake

To build the project with CMake, first of all, you need to create a folder 'build' in the server and in the client folders.

```bash
mkdir client/build server/build
```

After that, you will run the next command in each folder (client, server) to build all the necessary files:

```bash
cd build/
cmake ..
make
```

To run the server and the client on Linux, I will redirect you to the [Usage](./README.md#usage) section.
