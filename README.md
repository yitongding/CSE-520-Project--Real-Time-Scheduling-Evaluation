#Real-Time Scheduling Evaluation
Evaluate the real time system runs on Raspberry Pi


##How to run

####Cyclitest:
Need root permission!

1. idle
./cyclictest -p98 -m -n -c 0 -h 200 -q -l 50000 > idle.txt

2. CPU-busy
./sqrt & /home/pi/llcbench/cachebench/cachebench - m 28 -x 2 -e 2 -d 2 -b & ./cyclictest -p98 -m -n -c 0 -h 200 -q -l 50000 > CPUbusy.txt

3. I/O-busy
netserver
./hackbench 500 & /home/pi/netperf/src/netperf -l 500 & ./cyclictest -p98 -m -n -c 0 -h 200 -q -l 50000 > IObusy.txt

4. multi-core
taskset -c 0 ./cyclictest -p98 -m -n -c 0 -h 200 -q -l 50000 > CPU0.txt


####WCET:
Need root permission!

1 Idle system: 
/home/pi/Destops/bench_sqrt_fifo -c 1 -p 100 -d 10000 -l 98

2 CPU busy system: 
/home/pi/Destops/sqrt & /home/pi/Destops/bench_sqrt_fifo -c 1 -p 100 -d 10000 -l 98

3 Cache busy system: 
/home/pi/llcbench/cachebench/cachebench & /home/pi/Destops/bench_sqrt_fifo -c 1 -p 100  -d 10000 -l 98

4 I/O busy system:
netserver
./hackbench 500 & /home/pi/netperf/src/netperf & /home/pi/Destops/bench_sqrt_fifo -c 1 -p 100  -d 10000 -l 98





