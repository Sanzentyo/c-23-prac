add_rules("mode.debug", "mode.release")
set_languages("c++23")
set_warnings("all")
set_policy("build.c++.modules", true)

local llvm_bindir = os.getenv("LLVM_BINDIR")
if llvm_bindir and #llvm_bindir > 0 then
    set_toolset("cxx", path.join(llvm_bindir, "clang++"))
end

local llvm_sdk = os.getenv("LLVM_PREFIX")
if llvm_sdk and #llvm_sdk > 0 then
    set_values("sdk", llvm_sdk)
end

local module_sources = os.files("*.cppm")

for _, source in ipairs(os.files("*.cpp")) do
    local name = path.basename(source)

    target(name)
        set_kind("binary")
        add_files(source)
        if #module_sources > 0 then
            add_files(table.unpack(module_sources))
        end
end
