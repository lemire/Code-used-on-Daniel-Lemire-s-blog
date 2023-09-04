#ifndef NAME_TO_DNSWIRE_H
#define NAME_TO_DNSWIRE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Identifiers can start with an ascii letter or _, and they can contain letters, _ or digits. */
size_t count_identifiers(const char *source, size_t size);
size_t count_identifiers_neon(const char *source, size_t size);
size_t count_identifiers_neon_strager(const char *source, size_t size);
size_t count_identifiers_neon_strager_ranges(const char *source, size_t size);

#endif // NAME_TO_DNSWIRE_H
