#!/bin/sh
# usage: JAVA_HOME=... ./andcardinality.sh   (JDK 25 or later on macOS)
cd "$(dirname "$0")"
CP=$(ls lib/*.jar | tr '\n' ':')
mkdir -p build
"$JAVA_HOME/bin/javac" --release 25 -d build -cp "$CP" src/AndCardinality.java || exit 1
exec "$JAVA_HOME/bin/java" --enable-native-access=ALL-UNNAMED -javaagent:lib/agent.jar -cp "build:$CP" AndCardinality
