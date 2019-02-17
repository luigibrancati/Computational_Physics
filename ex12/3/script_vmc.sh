#!/bin/bash
for sigma in 0.99 1. 1.01; do
cat > input << EOF
$sigma
EOF

./a.out < input >> dati

done
