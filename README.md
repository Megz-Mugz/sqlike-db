# sqllike

## Requirements

- A C++ compiler with C++23 support
- CMake 3.20 or newer
- GoogleTest (required when building the tests)

On macOS with Homebrew, install the build dependencies with:

```sh
brew install cmake googletest
```

## Develop

Use the project helper for the common workflows:

```sh
./dev run
./dev test
./dev debug
```

Each command configures and builds the Debug preset automatically. The
`debug` command opens the program in LLDB; enter `run` at the LLDB prompt to
start it.

## Build manually

Configure and build the debug preset:

```sh
cmake --preset debug
cmake --build --preset debug
```

The executable is written to `build/debug/sqllike`.

For an optimized build, replace `debug` with `release`.

## Run manually

```sh
./build/debug/sqllike
```

## Test manually

```sh
ctest --preset debug
```
