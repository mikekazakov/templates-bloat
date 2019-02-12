#!/bin/sh

# bootstrap the source generator 2
clang++ -std=c++17 -O2 pattern_2.cpp -o pattern_2

/bin/echo -n > timings.csv
/bin/echo -n > sizes.csv

for(( METHODS=1; METHODS <= 20; METHODS++ ))
do

    for(( TYPES=1; TYPES <= 20; TYPES++ ))
    do

        # generate a source file to measure
        ./pattern_2 $METHODS $TYPES > tmp.cpp

        # run compilation
        /usr/bin/time clang++ -std=c++17 -O2 -c tmp.cpp 2> timing

        # grab time and size stats
        /bin/echo -n $(awk '{ print $1 }' timing) "" >> timings.csv
        /bin/echo -n $(stat -f "%z" tmp.o) "" >> sizes.csv
    done

    /bin/echo >> timings.csv
    /bin/echo >> sizes.csv
done
