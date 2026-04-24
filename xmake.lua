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

local function nonempty(value)
    return value and #value > 0
end

local llvm_prefix = os.getenv("LLVM_PREFIX")
local llvm_bindir = os.getenv("LLVM_BINDIR")

if not nonempty(llvm_prefix) and os.isdir("/opt/homebrew/opt/llvm") then
    llvm_prefix = "/opt/homebrew/opt/llvm"
end

if not nonempty(llvm_bindir) and nonempty(llvm_prefix) then
    llvm_bindir = path.join(llvm_prefix, "bin")
end

if nonempty(llvm_bindir) then
    local clangxx = path.join(llvm_bindir, "clang++")
    if os.isfile(clangxx) then
        set_toolset("cxx", clangxx)
    end
end

if nonempty(llvm_prefix) then
    set_values("sdk", llvm_prefix)
end

target("practice_support")
    set_kind("static")
    add_files("src/lib/*.cpp")
    add_headerfiles("src/lib/*.hpp")
    add_includedirs("src/lib", {public = true})

for _, source in ipairs(os.files("src/bin/*.cpp")) do
    local name = path.basename(source)

    target(name)
        set_kind("binary")
        add_files(source)
        add_deps("practice_support")
        add_includedirs("src/lib")
end
