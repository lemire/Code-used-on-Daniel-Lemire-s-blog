#!/usr/bin/env bash

for i in {1..10}; do
echo "trial ${i}"
echo "node"
node --version
node test1.js > /dev/null
node test1.js
node test2.js > /dev/null
node test2.js
node test3.js > /dev/null
node test3.js
node test4.js > /dev/null
node test4.js
node test5.js > /dev/null
node test5.js
echo "Bun"
bun run test1.js > /dev/null
bun run test1.js
bun run test2.js > /dev/null
bun run test2.js
bun run test3.js > /dev/null
bun run test3.js
bun run test4.js > /dev/null
bun run test4.js
bun run test5.js > /dev/null
bun run test5.js
done
