# Advent Of Code 2023

This repo holds my answers to the Advent Of Code 2023 challenges.
Each challenge is contained in its folder.
The first challenge is in [challenge-1](./challenge-1/), and so on...

To execute the code of a challenge, use the [Makefile](./Makefile):

```bash
make challenge target=$X
# replace $X with the number of the challenge to execute.
```

In case there are several C++ files within the selected challenge folder, each with a `main` function, then chose the one to complile:

```bash
make challenge target=$X src=$Y
# replace $Y with the name of the C++ file,
# which is by default "main.cpp"
```

You can also:

```bash
make execute
# to execute the file named "executable" in "./build"
```

```bash
make clear
# deletes the executable in `./build`
```

All of these challenges are done in C++ 20. **You need to have g++ installed**.