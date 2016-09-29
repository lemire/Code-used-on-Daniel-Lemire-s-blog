import PackageDescription

let package = Package(
    name: "SwiftCallOverhead",
    targets: [
        Target(name: "SwiftCallOverhead", dependencies: ["CSwiftCallOverhead"]),
    ],
    exclude: ["code"]
)
