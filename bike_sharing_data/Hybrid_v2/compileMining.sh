#g++ -std=c++11 -O3 cep_cleanData_VLDB_TTL_Evaluation.cpp -o cep_cleanData_VLDB_TTL_Evaluation.out
rm latency_Hybrid.dat
rm throughput_Hybrid.dat
for i in `seq 1 90`;
do

../cep_cleanData_VLDB_TTL_Evaluation.out 3 0."$i"
mv ../CleanedOutput PM_keys_3TTL_1h_g10.csv
cp PM_keys_3TTL_1h_g10.csv PM_keys_"$i".csv

../../../src_testing_bike_sharing/bin/cep_match -q P1 -s -c bike.eql -T 3 -I -p monitoring_P1.csv > Hybrid_"$i".txt

python process-latency.py latency.csv $i >> latency_Hybrid.dat
python process-throughput.py monitoring_P1.csv $i >> throughput_Hybrid.dat
done
