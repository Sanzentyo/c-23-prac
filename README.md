# c-23-prac

Minimal C++23 practice project using Xmake as the build system.(一部C++26の機能も使用)

## Requirements

- `xmake`
- `clangd`
- A recent LLVM/Clang toolchain if you want to use C++ modules such as `import std;`

On macOS, the system `clangd` may be too old for modules. A Homebrew LLVM install is recommended.

If LLVM isn't your default toolchain, point Xmake and your editor at it:

```sh
export LLVM_PREFIX=/opt/homebrew/opt/llvm
export LLVM_BINDIR=/opt/homebrew/opt/llvm/bin
```

## Usage

List targets:

```sh
xmake show -l targets
```

Build a target:

```sh
xmake build hello
```

Run a target:

```sh
xmake run hello
```

Generate `compile_commands.json` for `clangd`:

```sh
xmake project -k compile_commands
```

## just

This repository also includes a `justfile` as a thin task runner.

```sh
just list
just build hello
just run hello
just compdb
just check hello
```

## Notes on Modules

`import std;` depends on your local LLVM/Clang and libc++ setup.

If editor diagnostics for modules do not work on a fresh clone:

1. Install a recent LLVM toolchain.
2. Point `LLVM_PREFIX` and `LLVM_BINDIR` at that toolchain if it isn't your default.
3. Ensure your editor uses that `clangd`.
4. Run `xmake build hello` once to generate module artifacts.
5. Run `xmake project -k compile_commands`.
