echo "conventional"
bun run  conventionalmem.mjs > /dev/null
for i in {1..3}; do
  bun run --smol  conventionalmem.mjs
done
bun run --smol  switchmem.mjs > /dev/null
echo "switch"
for i in {1..3}; do
  bun run --smol  switchmem.mjs
done
echo "ternary"
bun run --smol  ternarytreemem.mjs > /dev/null
for i in {1..3}; do
  bun run --smol  ternarytreemem.mjs
done
echo "tree"
bun run --smol  treemem.mjs > /dev/null
for i in {1..3}; do
  bun run --smol  treemem.mjs
done
