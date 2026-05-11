add_rules("mode.debug", "mode.release")
set_languages("c++26")
set_warnings("all")
set_policy("build.c++.modules", true)

-- This project currently assumes a macOS + Homebrew LLVM layout for C++ modules.
-- TODO: Linux support
--   - Detect an LLVM/libc++ install path such as /usr/lib/llvm-*/ or /usr/local.
--   - Point xmake at clang++ and the matching libc++ modules metadata/std.cppm.
-- TODO: Windows support
--   - Decide whether to support LLVM/clang-cl + libc++ modules or MSVC's modules flow.
--   - Add platform-specific compiler, SDK, and module metadata discovery.
if not is_host("macosx") then
    raise("this xmake.lua currently supports macOS only")
end

local llvm_prefix = os.getenv("LLVM_PREFIX")

if not llvm_prefix or #llvm_prefix == 0 then
    if os.isdir("/opt/homebrew/opt/llvm") then
        llvm_prefix = "/opt/homebrew/opt/llvm"
    else
        raise("LLVM_PREFIX is not set and /opt/homebrew/opt/llvm was not found")
    end
end

set_toolchains("llvm", {sdk = llvm_prefix})

for _, source in ipairs(os.files("src/bin/*.cpp")) do
    local name = path.basename(source)

    target(name)
        set_kind("binary")
        add_files(source)
        add_files("src/lib/*.cppm")
end
