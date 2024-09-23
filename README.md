
# Crow App - README

This guide will walk you through the process of building and running a simple Crow-based web application. You'll need to install Boost and Crow as dependencies and use CMake to build the project.

## Prerequisites

Before you can build and run this application, ensure you have the following tools installed:

1. **C++ Compiler**: You’ll need a C++ compiler that supports C++11 or later (e.g., GCC, Clang, or Visual Studio).
2. **CMake**: A build system generator to help manage the build process.
3. **Boost**: A set of C++ libraries that Crow relies on for certain functionality.

### Required Libraries:
- **Boost** (for various utilities).
- **Crow** (header-only C++ web framework).

---

## Step-by-Step Instructions

### 1. Clone the Repository

First, clone the repository to your local machine:

```bash
git https://github.com/casrhub/Symmetric-Encryption-Algorithm.git
cd Symmetric-Encryption-Algorithm
```

---

### 2. Install Dependencies

#### Installing Boost

You’ll need Boost libraries for Crow to work. Follow the installation steps based on your operating system:

- **On Ubuntu**:
  ```bash
  sudo apt-get install libboost-all-dev
  ```

- **On MacOS (using Homebrew)**:
  ```bash
  brew install boost
  ```

- **On Windows (via vcpkg)**:
  Install vcpkg:
  ```bash
  git clone https://github.com/microsoft/vcpkg
  ./vcpkg/bootstrap-vcpkg.sh
  ```

  Install Boost:
  ```bash
  ./vcpkg/vcpkg install boost
  ```

#### Installing Crow

Since Crow is a header-only library, you just need to include it in your project. Follow these steps to download Crow:

1. Download Crow from its GitHub repository:
   ```bash
   git clone https://github.com/CrowCpp/Crow.git
   ```

2. Copy the Crow header files to your project folder, or set the path in your build configuration.

---

### 3. Build the Application

Now that you have Boost and Crow ready, follow these steps to build the application:

#### Using CMake:

1. **Configure the project with CMake**:

   First, make sure your `CMakeLists.txt` is set up properly. Here is an example:

   **`CMakeLists.txt`**:
   ```cmake
   cmake_minimum_required(VERSION 3.15)
   project(CrowSample)

   # Specify C++11 (or higher) standard
   set(CMAKE_CXX_STANDARD 11)
   set(CMAKE_CXX_STANDARD_REQUIRED ON)

   # Add the executable
   add_executable(CrowSample main.cpp)

   # Add the include directories for ASIO and Crow
   target_include_directories(CrowSample PUBLIC /usr/local/include /Users/casr/crowcpp/Crow-1.2.0/include)

   # Link pthread for multi-threading (required by Crow and ASIO)
   target_link_libraries(CrowSample PUBLIC pthread)
   ```

2. **Create the build directory**:

   In the root of your project folder, create a `build` directory and navigate to it:

   ```bash
   mkdir build
   cd build
   ```

3. **Run CMake to configure the build**:

   Run the following command in the `build` directory:

   ```bash
   cmake ..
   ```

   This will generate the necessary build files using your `CMakeLists.txt`.

4. **Build the project**:

   Now, run `make` (on Linux/macOS) or `cmake --build .` (on Windows) to build the application:

   ```bash
   make
   ```

   On Windows:
   ```bash
   cmake --build .
   ```

---

### 4. Run the Application

Once the build completes successfully, you’ll have an executable file (e.g., `crow_app`). You can now run your Crow-based web application:

```bash
./crow_app
```

After running, you can access your Crow app in the browser by visiting:

```
http://localhost:18080
```

The application will serve the page on the default port `18080`. You can modify the port in the `main.cpp` file if needed.

---



## Troubleshooting

### Boost Not Found
If Boost is not found, make sure it’s installed correctly and the path is configured properly in `CMakeLists.txt`. You can pass the Boost directory manually in the `cmake` command if needed:
```bash
cmake -DBOOST_ROOT=/path/to/boost ..
```

### Crow Not Found
Ensure that Crow's include directory is specified correctly in your `CMakeLists.txt`. If Crow is not in a system-wide include path, you will need to point to where you downloaded or placed the Crow headers.

---

## Conclusion

Now you know how to:
- Set up the dependencies (Boost and Crow).
- Build your Crow-based application using CMake.
- Run the application locally.

If you run into any issues, feel free to open an issue or reach out for help!
