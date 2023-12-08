echo "conventional"
node --expose-gc  conventionalmem.mjs
echo "switch"
node --expose-gc  switchmem.mjs
echo "ternary"
node --expose-gc  ternarytreemem.mjs
echo "tree"
node --expose-gc  treemem.mjs
