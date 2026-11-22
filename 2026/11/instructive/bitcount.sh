#!/bin/sh
# usage: JAVA_HOME=... ./bitcount.sh
cd "$(dirname "$0")"
mkdir -p build
"$JAVA_HOME/bin/javac" --release 25 --add-modules jdk.incubator.vector -d build src/BitCount.java 2>&1 | grep -v warning
exec "$JAVA_HOME/bin/java" --add-modules jdk.incubator.vector -cp build BitCount 2>&1 | grep -v WARNING
