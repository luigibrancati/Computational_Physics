#!/bin/bash
for p in 0.50 0.52 0.54 0.56 0.58 0.6;do
cat > input <<EOF
10
$p
EOF

./a.out < input >> lattice.txt

done
