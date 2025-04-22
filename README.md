# Coll MPA

A custom Muliple Precision Arithmetic library (MPA) written in C++ for me to
explore the concepts of MPA and different algorithms used in it. As I am simply
doing this to explore and learn, there is no guarantee that any of this actually
makes sense or is correct. This library should also not be used in any
production code as it probably will be quite buggy, inefficient and I do not
intend to maintain it.

## Build

_This project was developed on GNU/Linux and for GNU/Linux there is no guarantee
that it will work on other platforms._

To build the project, follow these steps:

1. Ensure you have CMake (version 3.28 or higher) and a C++ compiler that
   supports C++23 installed.
2. Clone the repository:

   ```bash
   git clone <repository-url>
   cd <repository-name>
   ```

3. Create a build directory and navigate into it:

   ```bash
   mkdir build && cd build
   ```

4. Run CMake to configure the project:

   ```bash
   cmake ..
   ```

5. Build the project:

   ```bash
   make
   ```

6. The executable will be available in the `build` directory.
