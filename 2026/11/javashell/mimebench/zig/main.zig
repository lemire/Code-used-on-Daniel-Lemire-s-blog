// MIME base64 decoding (76-character lines, CRLF) with std.base64's
// decoder that ignores chosen characters. Zig 0.17.
const std = @import("std");
const builtin = @import("builtin");
const Io = std.Io;

fn decode(d: anytype, a: std.mem.Allocator, in: []const u8) ![]u8 {
    const buf = try a.alloc(u8, d.calcSizeUpperBound(in.len));
    const n = try d.decode(buf, in);
    return buf[0..n];
}

pub fn main(init: std.process.Init) !void {
    const io = init.io;
    const gpa = std.heap.c_allocator;
    const cwd = Io.Dir.cwd();
    const exp_txt = try cwd.readFileAlloc(io, "expected.txt", gpa, .unlimited);
    var it = std.mem.tokenizeAny(u8, exp_txt, " \n");
    const exp_len = try std.fmt.parseInt(usize, it.next().?, 10);
    const exp_sum = try std.fmt.parseInt(u64, it.next().?, 10);
    const input = try cwd.readFileAlloc(io, "mime.txt", gpa, .unlimited);

    const decoder = std.base64.standard.decoderWithIgnore("\r\n");
    const cap = decoder.calcSizeUpperBound(input.len);

    const out = try decode(&decoder, gpa, input);
    var s: u64 = 0;
    for (out) |b| s += b;
    if (out.len != exp_len or s != exp_sum) return error.BadDecode;
    gpa.free(out.ptr[0..cap]);

    const iters = 200;
    for (0..3 * iters) |_| {
        const o = try decode(&decoder, gpa, input);
        std.mem.doNotOptimizeAway(o.ptr);
        gpa.free(o.ptr[0..cap]);
    }
    var best: f64 = std.math.floatMax(f64);
    for (0..10) |_| {
        const st = Io.Timestamp.now(io, .awake);
        for (0..iters) |_| {
            const o = try decode(&decoder, gpa, input);
            std.mem.doNotOptimizeAway(o.ptr);
            gpa.free(o.ptr[0..cap]);
        }
        const ns: f64 = @floatFromInt(st.durationTo(Io.Timestamp.now(io, .awake)).nanoseconds);
        best = @min(best, ns / iters);
    }
    std.debug.print("zig {s} mime std.base64 decoderWithIgnore(\"\\r\\n\"): {d:.2} GB/s\n", .{ builtin.zig_version_string, @as(f64, @floatFromInt(input.len)) / best });
}
