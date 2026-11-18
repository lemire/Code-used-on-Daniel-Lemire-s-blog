<?php
[$expLen, $expSum] = array_map('intval', explode(' ', trim(file_get_contents('expected.txt'))));
foreach (['mime'] as $name) {
    $s = file_get_contents("$name.txt");
    $out = base64_decode($s);
    if (strlen($out) !== $expLen || array_sum(unpack('C*', $out)) !== $expSum) die("bad decode $name\n");
    $iters = 200;
    for ($i = 0; $i < 3 * $iters; $i++) $out = base64_decode($s);
    $best = PHP_FLOAT_MAX;
    for ($t = 0; $t < 10; $t++) {
        $st = hrtime(true);
        for ($i = 0; $i < $iters; $i++) $out = base64_decode($s);
        $best = min($best, (hrtime(true) - $st) / $iters);
    }
    printf("php %s%s %s base64_decode(str): %.2f GB/s\n", PHP_VERSION, getenv("PHPLABEL") ?: "", $name, strlen($s) / $best);
}
