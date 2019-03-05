../../src_testing_bike_sharing/bin/cep_match -q P1 -s -c bike.eql -T 3 -I -p monitoring_P1.csv
#../../src_testing_bike_sharing/bin/cep_match -q P1 -s -c bike.eql -T 1  -p monitoring_P1.csv
python process-latency.py latency.csv 0
python process-throughput.py monitoring_P1.csv 0
