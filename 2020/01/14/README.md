```
  65536 pages   256 MB   calloc                            65.477 ms     3.82 GB/s 
  65536 pages   256 MB   new_and_touch                     65.245 ms     3.83 GB/s 
  65536 pages   256 MB   new_and_value_init               211.980 ms     1.18 GB/s 
  65536 pages   256 MB   new_and_value_init_nothrow       156.723 ms     1.60 GB/s 
  65536 pages   256 MB   memset_existing_allocation         8.325 ms    30.03 GB/s 
  65536 pages   256 MB   mempy_into_existing_allocation    20.202 ms    12.38 GB/s 

 131072 pages   512 MB   calloc                           130.321 ms     3.84 GB/s 
 131072 pages   512 MB   new_and_touch                    129.980 ms     3.85 GB/s 
 131072 pages   512 MB   new_and_value_init               422.791 ms     1.18 GB/s 
 131072 pages   512 MB   new_and_value_init_nothrow       313.152 ms     1.60 GB/s 
 131072 pages   512 MB   memset_existing_allocation        16.601 ms    30.12 GB/s 
 131072 pages   512 MB   mempy_into_existing_allocation    40.136 ms    12.46 GB/s 

 262144 pages  1024 MB   calloc                           259.954 ms     3.85 GB/s 
 262144 pages  1024 MB   new_and_touch                    259.416 ms     3.85 GB/s 
 262144 pages  1024 MB   new_and_value_init               845.110 ms     1.18 GB/s 
 262144 pages  1024 MB   new_and_value_init_nothrow       626.049 ms     1.60 GB/s 
 262144 pages  1024 MB   memset_existing_allocation        33.185 ms    30.13 GB/s 
 262144 pages  1024 MB   mempy_into_existing_allocation    79.908 ms    12.51 GB/s 
```
