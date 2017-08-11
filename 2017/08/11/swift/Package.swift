import PackageDescription

let package = Package(
    name: "hashbench"
,
    dependencies: [
   .Package(url: "https://github.com/lemire/Swimsuit.git",  majorVersion: 0)
    ]
)
