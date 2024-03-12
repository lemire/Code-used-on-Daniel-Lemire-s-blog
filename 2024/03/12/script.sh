echo "node"
node --version
echo 3 > /proc/sys/vm/drop_caches
node test1.js
echo 3 > /proc/sys/vm/drop_caches
node test2.js
echo 3 > /proc/sys/vm/drop_caches
node test3.js
echo 3 > /proc/sys/vm/drop_caches
node test4.js
echo 3 > /proc/sys/vm/drop_caches
node test5.js
echo 3 > /proc/sys/vm/drop_caches
echo "Bun"
echo 3 > /proc/sys/vm/drop_caches
bun run test1.js
echo 3 > /proc/sys/vm/drop_caches
bun run test2.js
echo 3 > /proc/sys/vm/drop_caches
bun run test3.js
echo 3 > /proc/sys/vm/drop_caches
bun run test4.js
echo 3 > /proc/sys/vm/drop_caches
bun run test5.js
