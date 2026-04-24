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
local llvm_bindir = os.getenv("LLVM_BINDIR")

if not llvm_prefix or #llvm_prefix == 0 then
    if os.isdir("/opt/homebrew/opt/llvm") then
        llvm_prefix = "/opt/homebrew/opt/llvm"
    else
        raise("LLVM_PREFIX is not set and /opt/homebrew/opt/llvm was not found")
    end
end

if not llvm_bindir then
    llvm_bindir = path.join(llvm_prefix, "bin")
elseif #llvm_bindir == 0 then
    raise("LLVM_BINDIR is empty")
end
llvm_bindir = assert(llvm_bindir)

local clangxx = path.join(llvm_bindir, "clang++")
if not os.isfile(clangxx) then
    raise("clang++ not found: " .. clangxx)
end

set_toolset("cxx", clangxx)
set_values("sdk", llvm_prefix)

target("practice_support")
    set_kind("static")
    add_files("src/lib/*.cpp")
    add_headerfiles("src/lib/*.hpp")
    add_includedirs("src/lib", { public = true })

for _, source in ipairs(os.files("src/bin/*.cpp")) do
    local name = path.basename(source)

    target(name)
        set_kind("binary")
        add_files(source)
        add_deps("practice_support")
        add_includedirs("src/lib")
end
