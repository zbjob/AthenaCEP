#g++ -std=c++11 -O3 cep_cleanData_VLDB_TTL_Evaluation.cpp -o cep_cleanData_VLDB_TTL_Evaluation.out
for i in `seq 1 9`;
do

./cep_cleanData_VLDB_TTL_Evaluation.out 3 0.0"$i"
mv CleanedOutput PM_keys_3TTL_1h_g10.csv
cp PM_keys_3TTL_1h_g10.csv Hybrid_v2/PM_keys_"0$i".csv

../../src_testing_bike_sharing/bin/cep_match -q P1 -s -c bike.eql -T 3 -I -p monitoring_P1.csv > Hybrid_"0$i".txt

python process-latency.py latency.csv 0$i >> latency_Hybrid.dat
python process-throughput.py monitoring_P1.csv 0$i >> throughput_Hybrid.dat
done
