// swift-tools-version:4.0
import PackageDescription

let package = Package(
    name: "arrayinit",
    products: [
        .executable(name: "precomphash", targets: ["precomphash"]),
    ],
    dependencies: [
   .package(url: "https://github.com/lemire/Swimsuit.git", from: "0.1.2")
    ],
    targets: [
        .target(
            name: "precomphash",
            dependencies: ["Swimsuit"]),
    ]
)
