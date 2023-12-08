echo "conventional"
for i in {1..3}; do
  node --expose-gc  conventionalmem.mjs
done
echo "switch"
for i in {1..3}; do
  node --expose-gc  switchmem.mjs
done
echo "ternary"
for i in {1..3}; do
  node --expose-gc  ternarytreemem.mjs
done
echo "tree"
for i in {1..3}; do
  node --expose-gc  treemem.mjs
done