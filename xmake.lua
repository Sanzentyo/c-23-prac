set_languages("c++23")
set_warnings("all")

for _, source in ipairs(os.files("*.cpp")) do
    local name = path.basename(source)

    target(name)
        set_kind("binary")
        add_files(source)
end
