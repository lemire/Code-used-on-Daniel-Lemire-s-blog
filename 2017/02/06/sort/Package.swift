import PackageDescription

let package = Package(
    name: "sort",
    dependencies: [
   .Package(url: "https://github.com/lemire/Swimsuit.git",  majorVersion: 0)
    ]
)
