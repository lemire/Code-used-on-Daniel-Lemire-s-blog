
### DOM parse, all stages (GB/s, best of 3 rounds)

| benchmark | 4.0.0 | 5.0.0 | speedup |
|---|--:|--:|--:|
| twitter | 4.78 | 4.82 | 1.01 |
| citm_catalog | 4.82 | 4.79 | 1.00 |
| canada | 1.10 | 1.21 | 1.10 |
| github_events | 5.33 | 5.30 | 0.99 |
| gsoc-2018 | 5.39 | 5.45 | 1.01 |
| marine_ik | 1.25 | 1.38 | 1.11 |
| mesh | 1.17 | 1.26 | 1.08 |
| numbers | 1.13 | 1.41 | 1.25 |
| random | 3.03 | 3.03 | 1.00 |
| twitterescaped | 1.59 | 2.85 | 1.79 |
| update-center | 3.96 | 3.84 | 0.97 |
| instruments | 4.15 | 4.11 | 0.99 |
| apache_builds | 4.95 | 4.78 | 0.97 |

### Stage 1 only (GB/s, best of 3 rounds)

| benchmark | 4.0.0 | 5.0.0 | speedup |
|---|--:|--:|--:|
| twitter | 12.75 | 12.88 | 1.01 |
| citm_catalog | 14.19 | 14.21 | 1.00 |
| canada | 14.36 | 14.35 | 1.00 |
| github_events | 14.93 | 14.94 | 1.00 |
| gsoc-2018 | 11.60 | 11.60 | 1.00 |
| marine_ik | 13.16 | 13.12 | 1.00 |
| mesh | 13.48 | 13.49 | 1.00 |
| numbers | 14.21 | 14.21 | 1.00 |
| random | 10.43 | 10.43 | 1.00 |
| twitterescaped | 13.48 | 13.48 | 1.00 |
| update-center | 13.84 | 13.90 | 1.00 |
| instruments | 14.75 | 14.76 | 1.00 |
| apache_builds | 14.75 | 14.63 | 0.99 |

### Stage 2 only (GB/s, best of 3 rounds)

| benchmark | 4.0.0 | 5.0.0 | speedup |
|---|--:|--:|--:|
| twitter | 7.68 | 7.72 | 1.01 |
| citm_catalog | 7.31 | 7.27 | 0.99 |
| canada | 1.19 | 1.32 | 1.11 |
| github_events | 8.32 | 8.23 | 0.99 |
| gsoc-2018 | 10.18 | 10.34 | 1.02 |
| marine_ik | 1.38 | 1.55 | 1.12 |
| mesh | 1.28 | 1.39 | 1.09 |
| numbers | 1.23 | 1.57 | 1.28 |
| random | 4.28 | 4.28 | 1.00 |
| twitterescaped | 1.80 | 3.62 | 2.01 |
| update-center | 5.57 | 5.32 | 0.95 |
| instruments | 5.79 | 5.71 | 0.99 |
| apache_builds | 7.48 | 7.13 | 0.95 |

### DOM serialization (minify) (GB/s, best of 3 rounds)

| benchmark | 4.0.0 | 5.0.0 | speedup |
|---|--:|--:|--:|
| twitter | 0.94 | 0.96 | 1.02 |
| citm_catalog | 1.07 | 1.08 | 1.01 |
| canada | 0.31 | 0.52 | 1.69 |
| github_events | 1.74 | 1.86 | 1.07 |
| gsoc-2018 | 1.10 | 1.25 | 1.14 |
| marine_ik | 0.28 | 0.37 | 1.31 |
| mesh | 0.34 | 0.47 | 1.41 |
| numbers | 0.32 | 0.49 | 1.57 |
| random | 1.14 | 1.18 | 1.04 |
| twitterescaped | 1.59 | 1.68 | 1.06 |
| update-center | 1.26 | 1.29 | 1.02 |
| instruments | 1.03 | 1.08 | 1.05 |
| apache_builds | 1.55 | 1.45 | 0.94 |

### bench_ondemand (GB/s, best of 3 rounds)

| benchmark | 4.0.0 | 5.0.0 | speedup |
|---|--:|--:|--:|
| json2msgpack<simdjson_ondemand> | 3.39 | 3.37 | 0.99 |
| json2msgpack<simdjson_dom> | 2.18 | 2.19 | 1.00 |
| partial_tweets<simdjson_ondemand> | 6.28 | 6.59 | 1.05 |
| partial_tweets<simdjson_dom> | 4.36 | 4.46 | 1.02 |
| distinct_user_id<simdjson_ondemand> | 6.76 | 6.88 | 1.02 |
| distinct_user_id<simdjson_ondemand_json_pointer> | 5.95 | 5.81 | 0.98 |
| distinct_user_id<simdjson_dom> | 4.50 | 4.47 | 1.00 |
| distinct_user_id<simdjson_dom_json_pointer> | 4.30 | 4.31 | 1.00 |
| find_tweet<simdjson_ondemand> | 11.43 | 11.32 | 0.99 |
| find_tweet<simdjson_dom> | 4.81 | 4.72 | 0.98 |
| top_tweet<simdjson_ondemand> | 6.79 | 6.79 | 1.00 |
| top_tweet<simdjson_ondemand_forward_only> | 6.41 | 6.61 | 1.03 |
| top_tweet<simdjson_dom> | 4.63 | 4.58 | 0.99 |
| kostya<simdjson_ondemand> | 3.04 | 3.11 | 1.02 |
| kostya<simdjson_dom> | 2.00 | 2.02 | 1.01 |
| large_random<simdjson_ondemand> | 1.23 | 1.27 | 1.03 |
| large_random<simdjson_dom> | 0.70 | 0.72 | 1.02 |
| amazon_cellphones<simdjson_dom<UNTHREADED>> | 2.65 | 2.60 | 0.98 |
| amazon_cellphones<simdjson_ondemand<UNTHREADED>> | 3.87 | 3.91 | 1.01 |
| large_amazon_cellphones<simdjson_dom<UNTHREADED>> | 2.44 | 2.42 | 0.99 |
| large_amazon_cellphones<simdjson_ondemand<UNTHREADED>> | 3.51 | 3.32 | 0.94 |
