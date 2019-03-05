import csv
from datetime import datetime
from dateutil.parser import parse

ReferenceTime = datetime(2018,10,1)

#with open('201805-fordgobike-tripdata.csv') as csv_file:
with open('201810-citibike-tripdata.csv') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    line_count = 0
    for row in csv_reader:
        #print row
        if line_count == 0:
            #print row
            line_count += 1
        else:
            timeStr = row[1]
            t = datetime.strptime(timeStr, '%Y-%m-%d %H:%M:%S.%f')
            diff = (t - ReferenceTime).total_seconds() * 1000000
            row[1] = str((int(diff)))

            timeStr = row[2]
            t = datetime.strptime(timeStr, '%Y-%m-%d %H:%M:%S.%f')
            diff = (t - ReferenceTime).total_seconds() * 1000000
            row[2] = str((int(diff)))


            rowStr = str(line_count)
            for item in row:
                rowStr += ","+ item
            print rowStr

            line_count += 1


