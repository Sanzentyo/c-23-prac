set shell := ["zsh", "-cu"]

default:
  @just --list

list:
  @zig build --list-steps

build name:
  zig build -Dname={{name}}

run name:
  zig build run -Dname={{name}}

check name:
  clangd --check={{name}}.cpp
