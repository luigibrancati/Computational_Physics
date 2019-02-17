#!/bin/bash
for sigma in 0.62 0.625 0.63 0.635 0.64 0.645 0.65; do
cat > input << EOF
$sigma
EOF

./a.out < input >> dati

done
