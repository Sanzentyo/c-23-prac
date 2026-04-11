const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});
    const requested_name = b.option([]const u8, "name", "Base name of the .cpp file to build");

    var dir = std.fs.cwd().openDir(".", .{ .iterate = true }) catch |err| {
        std.debug.panic("failed to open project directory: {s}", .{@errorName(err)});
    };
    defer dir.close();

    var it = dir.iterate();
    var found_requested = requested_name == null;
    var run_step: ?*std.Build.Step = null;

    while (it.next() catch |err| {
        std.debug.panic("failed to iterate project directory: {s}", .{@errorName(err)});
    }) |entry| {
        if (entry.kind != .file) continue;
        if (!std.mem.endsWith(u8, entry.name, ".cpp")) continue;

        const exe_name = std.fs.path.stem(entry.name);
        if (requested_name) |name| {
            if (!std.mem.eql(u8, exe_name, name)) continue;
            found_requested = true;
        }

        const root_module = b.createModule(.{
            .target = target,
            .optimize = optimize,
            .link_libcpp = true,
        });

        const exe = b.addExecutable(.{
            .name = exe_name,
            .root_module = root_module,
        });

        root_module.addCSourceFile(.{
            .file = b.path(entry.name),
            .flags = &.{"-std=c++23"},
        });

        b.installArtifact(exe);

        if (requested_name != null) {
            const cmd = b.addRunArtifact(exe);
            run_step = &cmd.step;
        } else {
            const step_name = b.dupe(exe_name);
            const step_desc = b.fmt("Build the {s} executable", .{exe_name});
            const exe_step = b.step(step_name, step_desc);
            exe_step.dependOn(&exe.step);
        }
    }

    if (!found_requested) {
        std.debug.panic("no .cpp file found for -Dname={s}", .{requested_name.?});
    }

    if (requested_name) |name| {
        const selected_step = b.step(name, b.fmt("Build the {s} executable", .{name}));
        selected_step.dependOn(b.getInstallStep());

        const run = b.step("run", b.fmt("Build and run the {s} executable", .{name}));
        run.dependOn(run_step.?);
    }
}
