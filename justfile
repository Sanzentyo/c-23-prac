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
  clangd --check={{name}}.cpp

compdb:
  xmake project -k compile_commands

clean:
  xmake clean
