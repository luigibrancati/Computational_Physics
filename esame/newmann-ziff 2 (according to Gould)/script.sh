#!/bin/bash
for i in {1..10}; do
cat > input <<EOF
30
EOF

./rand.out >> input
./a.out < input

done
