#!/bin/bash
rm latency_RIS.dat
rm throughput_RIS.dat
for i in `seq 1 60`;
do
../../src_testing_bike_sharing/bin/cep_match -q P1 -s -c bike.eql -T 1  -y 0."$i" -p monitoring_P1.csv > RIS_"$i".txt
python process-latency.py latency.csv $i >> latency_RIS.dat
python process-throughput.py monitoring_P1.csv $i >> throughput_RIS.dat
done
