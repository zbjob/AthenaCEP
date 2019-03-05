#!/bin/bash
for i in `seq 1 9`;
do
../../src_testing_bike_sharing/bin/cep_match -q P1 -s -c bike.eql -T 1  -y 0.0"$i" -p monitoring_P1.csv > RIS_0"$i".txt
python process-latency.py latency.csv 0$i >> latency_RIS.dat
python process-throughput.py monitoring_P1.csv 0$i >> throughput_RIS.dat
done
