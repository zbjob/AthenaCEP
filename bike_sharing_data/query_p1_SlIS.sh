#!/bin/bash
rm latency_SlIS.dat
rm throughput_SlIS.dat
for i in `seq 1 60`;
do
../../src_testing_bike_sharing/bin/cep_match -q P1 -s -c bike.eql -T 1  -z 0."$i" -p monitoring_P1.csv > SlIS_"$i".txt
python process-latency.py latency.csv $i >> latency_SlIS.dat
python process-throughput.py monitoring_P1.csv $i >> throughput_SlIS.dat
done
