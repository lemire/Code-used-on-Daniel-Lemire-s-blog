To run:

```
cd StreamVSByte
mvn clean install
java -jar target/benchmarks.jar
```


To setup project:
```
mvn archetype:generate -DinteractiveMode=false -DarchetypeGroupId=org.openjdk.jmh -DarchetypeArtifactId=jmh-java-benchmark-archetype -DgroupId=me.lemire.microbenchmarks -DartifactId=StreamVSByte -Dversion=1.0
```

