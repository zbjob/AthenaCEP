g++ -std=c++11 -O3 cep_cleanData_VLDB_TTL_Evaluation.cpp -o cep_cleanData_VLDB_TTL_Evaluation.out

./cep_cleanData_VLDB_TTL_Evaluation.out 3 0.30
mv CleanedOutput PM_keys_3TTL_1h_g10.csv

../../src_testing_bike_sharing/bin/cep_match -q P1 -s -c bike.eql -T 3 -I -p monitoring_P1.csv

python process-latency.py latency.csv 0
python process-throughput.py monitoring_P1.csv 0
