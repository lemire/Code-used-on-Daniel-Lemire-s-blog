#include <inttypes.h>
#include <stdlib.h>
#include <iostream>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

/* max MLP value tested with the naked strategy */
constexpr int NAKED_MAX = 30;

/* if true, try the pure pointer chasing strategy */
constexpr int DO_NAKED = true;

void naked_measure_body(float (&time_measure)[NAKED_MAX], uint64_t *bigarray, size_t howmanyhits, size_t repeat);
