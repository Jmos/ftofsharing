#!/bin/sh

./create_docs.sh

rm -fdr ../lhmail_dist/*

mkdir ../lhmail_dist/doc
mkdir ../lhmail_dist/doc/html
mkdir ../lhmail_dist/examples
#mkdir ../lhmail_dist/examples/smtp
#mkdir ../lhmail_dist/examples/browser
#mkdir ../lhmail_dist/examples/pop3

for i in examples/* ; do
    mkdir ../lhmail_dist/"$i"
done;

cp *.cpp *.c *.h *.pro *.sh ../lhmail_dist
cp doc/html/* ../lhmail_dist/doc/html

for i in examples/*; do
    cp $i/*.cpp $i/*.c $i/*.h $i/*.pro $i/*.sh ../lhmail_dist/$i/
done;


