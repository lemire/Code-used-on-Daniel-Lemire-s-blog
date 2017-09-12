import PackageDescription

let package = Package(
    name: "opqueue",
    dependencies: [
   .Package(url: "https://github.com/lemire/Swimsuit.git", majorVersion: 0)
    ]
)
