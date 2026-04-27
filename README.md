# flash

A POSIX shell written in C23. Imitates traditional Bash behaviour, but provides command suggestions to user based on levenshtein distances.

## Features

- Pipelines (`|`), conditional execution (`&&`, `||`), and sequencing (`;`)
- I/O redirection (`>`, `>>`, `2>`, `2>>`)
- Tab completion
- "Did you mean" suggestion line. Shows the closest matching command as you type.
- Configurable prompt via `~/.flashrc`
- Builtins: `cd`, `echo`, `exit`, `pwd`, `type`

## Build

Note: The project can only be built within a linux environment.

Since the readline library is not currently being installed correctly with CMake, you should install it locally using 

```sh
 sudo apt update
 sudo apt install -y libreadline-dev
```

Requires `cmake` and `vcpkg` (for readline).

```sh
cmake -B build -S .
cmake --build ./build
./build/shell
```

Or use the convenience script:

```sh
./run.sh
```

## Configuration

Create `~/.flashrc` to customize the prompt:

```
PS1_TEXT = "$ "       =>    Sets prompt text
PS1_COLOR = green     =>    Sets prompt color
```

`PS1_COLOR` accepts `green`, `red`, `blue`, or `default`.

## Contributing

Format all C source files before submitting a pull request:


```sh
find src -name '*.c' -o -name '*.h' | xargs clang-format -i
```

Formatting is enforced automatically in CI.
