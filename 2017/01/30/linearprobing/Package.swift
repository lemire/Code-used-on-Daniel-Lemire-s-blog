import PackageDescription

let package = Package(
    name: "linearprobing"
,
    dependencies: [
   .Package(url: "https://github.com/lemire/Swimsuit.git",  majorVersion: 0)
    ]
)
