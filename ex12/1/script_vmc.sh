#!/bin/bash
for sigma in 1. 1.2 1.4 1.6 1.8 2.0453 2.2 2.4 2.6 2.8 3.; do
cat > input << EOF
$sigma
EOF

./a.out < input >> dati

done
