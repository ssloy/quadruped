set terminal pngcairo size 1024,1024 enhanced font 'Verdana,10'
set output 'readings.png'
set grid
set timestamp
plot 'readings.txt' using 1:2

