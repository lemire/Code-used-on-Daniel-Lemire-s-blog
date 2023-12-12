echo "conventional"
node --expose-gc  conventionalmem.mjs > /dev/null
for i in {1..5}; do
  node --expose-gc  conventionalmem.mjs
done
node --expose-gc  switchmem.mjs > /dev/null
echo "switch"
for i in {1..5}; do
  node --expose-gc  switchmem.mjs
done
echo "ternary"
node --expose-gc  ternarytreemem.mjs > /dev/null
for i in {1..5}; do
  node --expose-gc  ternarytreemem.mjs
done
echo "tree"
node --expose-gc  treemem.mjs > /dev/null
for i in {1..5}; do
  node --expose-gc  treemem.mjs
done
