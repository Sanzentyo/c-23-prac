set shell := ["zsh", "-cu"]

default:
  @just --list

list:
  xmake show -l targets

build name:
  xmake build {{name}}

run name:
  xmake run {{name}}

check name:
  xmake project -k compile_commands
  if [[ -x /opt/homebrew/opt/llvm/bin/clangd ]]; then /opt/homebrew/opt/llvm/bin/clangd --check=src/bin/{{name}}.cpp; else clangd --check=src/bin/{{name}}.cpp; fi

compdb:
  xmake project -k compile_commands

clean:
  xmake clean
