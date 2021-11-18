for i in {1..20}; do ./verbose 10000 | ./pipespeed && ./verbose 10000| ./cpipespeed; done
