# c-23-prac

Minimal C++23 practice project using Xmake. Some C++26 features are also used.

## Skill Auto-Loading

Before starting work, proactively assess which skills are needed and load them via `skill(name=...)`:

| Condition | Skill to Load |
|-----------|---------------|
| Any `just` command or task runner usage | `justfile` |
| C++ build/run/check commands | `justfile` (this project uses just as task runner) |

Skills NOT relevant to this project (do NOT load): `typescript`, `python-uv-enforcer`, `rust-best-practices`, `moonbit-practice`.

## Key Commands

```bash
just list              # List targets
just build <name>      # Build a target
just run <name>        # Run a target
just check <name>      # clangd check on src/bin/<name>.cpp
just compdb            # Regenerate compile_commands.json
just clean             # Clean build artifacts
```

## 项目構造

- `xmake.lua` — ビルド定義 (C++26, macOS + Homebrew LLVM)
- `src/lib/` — 共有ライブラリ (static lib `practice_support`)
- `src/bin/` — 各ターゲットのエントリポイント (`src/bin/<name>.cpp` → target `<name>`)
- `justfile` — タスクランナー (thin wrapper around xmake + clangd)
- `.zed/settings.json` — Zed エディタ設定 (clangd path)

## Notes on Modules

`import std;` depends on local LLVM/Clang and libc++ setup.
If editor diagnostics for modules do not work on a fresh clone:
1. Install a recent LLVM toolchain
2. Point `LLVM_PREFIX` and `LLVM_BINDIR` at that toolchain if it isn't your default
3. Ensure your editor uses that `clangd`
4. Run `xmake build hello` once to generate module artifacts
5. Run `xmake project -k compile_commands`