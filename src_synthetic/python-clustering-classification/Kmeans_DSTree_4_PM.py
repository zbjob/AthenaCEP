import numpy as np
from sklearn.cluster import KMeans

#define
#stream file, i
#index coloume for contribution and consumptions,
#the number of clusters

data1 = np.loadtxt('../src_NS/cep_cleanData/PM_state_1.csv', dtype=np.str, delimiter=",")
data2 = np.loadtxt('../src_NS/cep_cleanData/PM_state_2.csv', dtype=np.str, delimiter=",")
#data1 = np.loadtxt('/home/zhaobo/HU/CEP_load_shedding/src_PM_Distribution_test/src_NS/cep_cleanData/PM_state_1.csv', dtype=np.str, delimiter=",")
#data2 = np.loadtxt('/home/zhaobo/HU/CEP_load_shedding/src_PM_Distribution_test/src_NS/cep_cleanData/PM_state_2.csv', dtype=np.str, delimiter=",")
#data = np.loadtxt('/vol/home-vol2/pda/zhaobo/google_cluster/clusterdata-2011-2/CleanData/attr1.csv', dtype=np.str, delimiter=",")
#data = np.loadtxt('/vol/home-vol2/pda/zhaobo/CEP_load_shedding/src_PM_Distribution_test/src_NS/cep_cleanData/CleanedOutput.copy', dtype=np.str, delimiter=",")

outputFile = open("lableInfo.txt","w")
#outputFile.write("stateBuffer,label,max,min,center,50percentil"+'\n')
outputFile.write("state, lable, max_contribution, min_contribution, max_consumption,  min_consumption, 50_percentile_contribution, 50_percentile_consumption, AVG_contribution, AVG_consumption, 50_percentile_contribution/50 percent_consumption \n")
print len(data1)
print len(data1[0])

CONST_CONTRIBUTION_COL_STATE1 =2
CONST_CONSUMPTION_COL_STATE1 =1
CONST_NUM_TUPLES_STATE1 = len(data1)
CONST_NUM_CLUSTERS_STATE1 =4

CONST_CONTRIBUTION_COL_STATE2= 3
CONST_CONSUMPTION_COL_STATE2 = 2
CONST_NUM_TUPLES_STATE2 = len(data2)
CONST_NUM_CLUSTERS_STATE2 = 4


contributions1 = data1[:,CONST_CONTRIBUTION_COL_STATE1].astype(np.float)
contributions2 = data2[:,CONST_CONTRIBUTION_COL_STATE2].astype(np.float)

contributions1.shape = (CONST_NUM_TUPLES_STATE1,1)
contributions2.shape = (CONST_NUM_TUPLES_STATE2,1)

contribution_consumptions1 = data1[:,[CONST_CONSUMPTION_COL_STATE1,CONST_CONTRIBUTION_COL_STATE1]].astype(np.float)
contribution_consumptions2 = data2[:,[CONST_CONSUMPTION_COL_STATE2,CONST_CONTRIBUTION_COL_STATE2]].astype(np.float)

print contribution_consumptions1
print contribution_consumptions2
contribution_consumptions1.shape = (CONST_NUM_TUPLES_STATE1,2)
contribution_consumptions2.shape = (CONST_NUM_TUPLES_STATE2,2)

av1 = data1[:,0].astype(np.float)
av1.shape = (CONST_NUM_TUPLES_STATE1,1)

av1bv2 = data2[:,[0,1]].astype(np.float)
av1bv2.shape = (CONST_NUM_TUPLES_STATE2,2)

#consumptions  = data[:,1].astype(np.float)
#av1 = data1[:,0].astype(np.float)

#contributions.shape = (CONST_NUM_TUPLES, 1)
#consumptions.shape = (CONST_NUM_TUPLES, 1)

#nonzeroContributionIdx1 = np.nonzero(contributions1)
#nonzeroContribution1 = contributions1[nonzeroContributionIdx1]
#nonzeroContribution1.shape = (len(nonzeroContribution1),1)
#
#nonzeroContributionIdx2 = np.nonzero(contributions2)
#nonzeroContribution2 = contributions2[nonzeroContributionIdx2]
#nonzeroContribution2.shape = (len(nonzeroContribution2),1)



#clf1 = KMeans(CONST_NUM_CLUSTERS_STATE1)
#labels1 = [CONST_NUM_CLUSTERS_STATE1]*CONST_NUM_TUPLES_STATE1
#labels1 = np.array(labels1)
#labels1.shape = (CONST_NUM_TUPLES_STATE1,1)
#
#clf1.fit(nonzeroContribution1)
#nonzeroLabels1 = clf1.labels_
#centers1 = clf1.cluster_centers_
#print nonzeroLabels1
#
#labels1[nonzeroContributionIdx1] = nonzeroLabels1
#
#print labels1.transpose()
#print ("=====================")
#
#
#clf2 = KMeans(CONST_NUM_CLUSTERS_STATE2)
#labels2 = [CONST_NUM_CLUSTERS_STATE2]*CONST_NUM_TUPLES_STATE2
#labels2 = np.array(labels2)
#labels2.shape = (CONST_NUM_TUPLES_STATE2,1)
#
#clf2.fit(nonzeroContribution2)
#nonzeroLabels2 = clf2.labels_
#centers2 = clf2.cluster_centers_
#print nonzeroLabels2
#
#labels2[nonzeroContributionIdx2] = nonzeroLabels2
#print labels2.transpose()
#
#
#results1 = np.zeros((CONST_NUM_TUPLES_STATE1,2))
#results1[:,0] = av1.transpose()
#results1[:,1] = labels1.transpose()
#
#print results1


clf1 = KMeans(CONST_NUM_CLUSTERS_STATE1)
clf1.fit(contribution_consumptions1)
labels1 = clf1.labels_
centers1 = clf1.cluster_centers_

print labels1
#print ("==========================")
#print contribution_consumptions1[:,[0]]
#print contribution_consumptions1[:,[1]]
results1 = np.zeros((CONST_NUM_TUPLES_STATE1,2))
results1[:,0] = av1.transpose()
results1[:,1] = labels1.transpose()

clf2 = KMeans(CONST_NUM_CLUSTERS_STATE2)
clf2.fit(contribution_consumptions2)
labels2 = clf2.labels_
centers2 = clf2.cluster_centers_

print labels2



clusters = []
for i in range(0,CONST_NUM_CLUSTERS_STATE1):
    clusters.append([])

for i in range(0,CONST_NUM_TUPLES_STATE1):
   clusters[int(results1[i,1])].append(contribution_consumptions1[i])
#print ("============")
#a = np.array(clusters[0])
#print ("===========")
#print a[:,1]
#print a[:,0]

for i in range(0, CONST_NUM_CLUSTERS_STATE1):
    tmp = np.array(clusters[i])
    contribution = tmp[:,1]
    contribution_P50 = np.percentile(contribution, 50)
    contribution_P25 = np.percentile(contribution, 25)
    consumption = tmp[:,0]
    consumption_P50 = np.percentile(consumption, 50)

    contribution_AVG = np.mean(contribution)
    consumption_AVG = np.mean(consumption)

    #    outputFile.write("1,"+str(i)+","+str(int(max(clusters[i])))+","+str(int(min(clusters[i])))+","+str(float(centers1[i]))+","+str(p)+'\n')
#    outputFile.write("1,"+str(i)+","+str(len(clusters[i]))+","+ str(max(contribution)) +","+ str(max(consumption))+","+str(min(contribution))+","+str(min(consumption))+","+str(centers1[i][1])+"," +str(centers1[i][0])+","+str(contribution_P50)+","+ str(consumption_P50)+","+str(contribution_P25)+'\n')

    outputFile.write("0,"+str(i)+","+ str(max(contribution)) +","+ str(min(contribution))+","+str(max(consumption))+","+str(min(consumption))+","+str(contribution_P50)+","+ str(consumption_P50)+","+str(np.mean(contribution))+","+ str(np.mean(consumption))+ ","+str(contribution_P50/consumption_P50)+'\n')

    print ("lable: %d, dataNum: %d, max_contribution: %d, max_consumption: %d, min_contribution: %d, min_consumption: %d, centers_contribution %d, centers_consumprion: %d, 50 percentile_contribution: %s, 50 percent_consumption: %s" % (i, len(clusters[i]), max(contribution),max(consumption), min(contribution),min(consumption), centers1[i][1], centers1[i][0], "{:,}".format(contribution_P50), "{:,}".format(consumption_P50)))
    #print ("c+/c-: %f\n",contribution_P50/consumption_P50)
    #print contribution/consumption
#    #print clusters[i]
##outputFile.write(CONST_NUM_CLUSTERS_STATE1,0,0,0,0)
#outputFile.write("1,"+str(CONST_NUM_CLUSTERS_STATE1)+",0,0,0,0"+'\n')
#
#
#print ("=====================")
#
results2 = np.zeros((CONST_NUM_TUPLES_STATE2,2))
#results2[:,0] = av1.transpose()
results2[:,1] = labels2.transpose()

print ("========")
print results2[4,1]

clusters = []
for i in range(0,CONST_NUM_CLUSTERS_STATE2):
    clusters.append([])

for i in range(0,CONST_NUM_TUPLES_STATE2):
    clusters[int(results2[i,1])].append(contribution_consumptions2[i])

for i in range(0, CONST_NUM_CLUSTERS_STATE2):
    tmp = np.array(clusters[i])
    contribution = tmp[:,1]
    print ("======")
    print contribution
    contribution_P50 = np.percentile(contribution, 50)
    contribution_P25 = np.percentile(contribution, 25)
    consumption = tmp[:,0]
    consumption_P50 = np.percentile(consumption, 50)
#    outputFile.write("1,"+str(i)+","+str(int(max(clusters[i])))+","+str(int(min(clusters[i])))+","+str(float(centers1[i]))+","+str(p)+'\n')
#    outputFile.write("2,"+str(i)+","+str(len(clusters[i]))+","+ str(max(contribution)) +","+ str(max(consumption))+","+str(min(contribution))+","+str(min(consumption))+","+str(centers2[i][1])+"," +str(centers2[i][0])+","+str(contribution_P50)+","+ str(consumption_P50)+","+str(contribution_P25)+ ","+str(contribution_P50/consumption_P50)+'\n')

    outputFile.write("1,"+str(i)+","+ str(max(contribution)) +","+ str(min(contribution))+","+str(max(consumption))+","+str(min(consumption))+","+str(contribution_P50)+","+ str(consumption_P50)+","+str(np.mean(contribution))+","+ str(np.mean(consumption))+ ","+str(contribution_P50/consumption_P50)+'\n')

    print ("lable: %d, dataNum: %d, max_contribution: %d, max_consumption: %d, min_contribution: %d, min_consumption: %d, centers_contribution %d, centers_consumprion: %d, 50 percentile_contribution: %s, 50 percent_consumption: %s" % (i, len(clusters[i]), max(contribution),max(consumption), min(contribution),min(consumption), centers2[i][1], centers2[i][0], "{:,}".format(contribution_P50), "{:,}".format(consumption_P50)))
    #print ("c+/c-: %f\n",contribution_P50/consumption_P50)


#for i in range(0, CONST_NUM_CLUSTERS_STATE2):
#    a = np.array(clusters[i])
#    p = np.percentile(a, 50)
#    print ("lable: %d, dataNum: %d, max: %d, min: %d,centers %d, 50 percentile: %s" % (i, len(clusters[i]), max(clusters[i]),min(clusters[i]),centers2[i], "{:,}".format(p)))
#    outputFile.write("2,"+str(i)+","+str(int(max(clusters[i])))+","+str(int(min(clusters[i])))+","+str(float(centers2[i]))+","+str(p)+'\n')
#outputFile.write("2,"+str(CONST_NUM_CLUSTERS_STATE2)+",0,0,0,0"+'\n')

outputFile.close()




from sklearn import tree

#DS = tree.DecisionTreeClassifier()

DS1 = tree.DecisionTreeClassifier(class_weight=None, criterion='gini', max_depth=7,max_features=None, max_leaf_nodes=None, min_samples_leaf=1, min_samples_split=2, min_weight_fraction_leaf=0.0,presort=False, random_state=100, splitter='best')
DS2 = tree.DecisionTreeClassifier(class_weight=None, criterion='gini', max_depth=7,max_features=None, max_leaf_nodes=None, min_samples_leaf=1, min_samples_split=2, min_weight_fraction_leaf=0.0,presort=False, random_state=100, splitter='best')
DS_STATE2 = DS1.fit(av1bv2,labels2)
DS_STATE1 = DS2.fit(av1,labels1)

#import pickle
from sklearn.externals import joblib
joblib.dump(DS_STATE1,'DS_STATE1.pkl')
joblib.dump(DS_STATE2,'DS_STATE2.pkl')

#import graphviz
dot_data = tree.export_graphviz(DS_STATE1, out_file="DS_STATE1.dot",class_names=True,filled=True, rounded=True,special_characters=True)
dot_data = tree.export_graphviz(DS_STATE2, out_file="DS_STATE2.dot",class_names=True,filled=True, rounded=True,special_characters=True)
#graph = graphviz.Source(dot_data)
#graph.render("DS")

import time
start = time.clock()
a = DS_STATE2.predict(av1bv2)
end = time.clock()
print str(end-start)
print a
