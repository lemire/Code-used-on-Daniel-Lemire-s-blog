// swift-tools-version:4.0
import PackageDescription

let package = Package(
    name: "arrayinit",
    products: [
        .executable(name: "arrayinit", targets: ["arrayinit"]),
    ],
    dependencies: [
   .package(url: "https://github.com/lemire/Swimsuit.git", from: "0.1.2")
    ],
    targets: [
        .target(
            name: "arrayinit",
            dependencies: ["Swimsuit"]),
    ]
)
