import PackageDescription

let package = Package(
    name: "arrays",
    dependencies: [
   .Package(url: "https://github.com/lemire/Swimsuit.git", majorVersion: 0)
    ]
)
