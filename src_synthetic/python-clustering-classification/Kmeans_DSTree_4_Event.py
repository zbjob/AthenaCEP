import numpy as np
from sklearn.cluster import KMeans

#define
#stream file, i
#index coloume for contribution and consumptions,
#the number of clusters

dataAv1 = np.loadtxt('../src_NS/cep_cleanData/Av1.csv', dtype=np.str, delimiter=",")
dataBv2 = np.loadtxt('../src_NS/cep_cleanData/Bv2.csv', dtype=np.str, delimiter=",")
dataCv1 = np.loadtxt('../src_NS/cep_cleanData/Cv1.csv', dtype=np.str, delimiter=",")
dataCv2 = np.loadtxt('../src_NS/cep_cleanData/Cv2.csv', dtype=np.str, delimiter=",")

#data1 = np.loadtxt('../src_NS/cep_cleanData/PM_state_1.csv', dtype=np.str, delimiter=",")
#data2 = np.loadtxt('../src_NS/cep_cleanData/PM_state_2.csv', dtype=np.str, delimiter=",")

#data1 = np.loadtxt('/home/zhaobo/HU/CEP_load_shedding/src_PM_Distribution_test/src_NS/cep_cleanData/PM_state_1.csv', dtype=np.str, delimiter=",")
#data2 = np.loadtxt('/home/zhaobo/HU/CEP_load_shedding/src_PM_Distribution_test/src_NS/cep_cleanData/PM_state_2.csv', dtype=np.str, delimiter=",")
#data = np.loadtxt('/vol/home-vol2/pda/zhaobo/google_cluster/clusterdata-2011-2/CleanData/attr1.csv', dtype=np.str, delimiter=",")
#data = np.loadtxt('/vol/home-vol2/pda/zhaobo/CEP_load_shedding/src_PM_Distribution_test/src_NS/cep_cleanData/CleanedOutput.copy', dtype=np.str, delimiter=",")

outputFile = open("EventlableInfo.txt","w")
#outputFile.write("stateBuffer,label,max,min,center,50percentil"+'\n')
outputFile.write("state, lable, max_contribution, min_contribution, max_consumption,  min_consumption, 50_percentile_contribution, 50_percentile_consumption, AVG_contribution, AVG_consumption, 50_percentile_contribution/50 percent_consumption \n")

#print len(data1)
#print len(data1[0])

CONST_CONTRIBUTION_COL_ABv =2
CONST_CONTRIBUTION_COL_Cv = 1

CONST_CONSUMPTION_COL_ABv =1

CONST_NUM_TUPLES_Av1= len(dataAv1)
CONST_NUM_TUPLES_Bv2= len(dataBv2)
CONST_NUM_TUPLES_Cv1= len(dataCv1)
CONST_NUM_TUPLES_Cv2= len(dataCv2)

CONST_NUM_CLUSTERS_Av1 =4
CONST_NUM_CLUSTERS_Bv2 =4
CONST_NUM_CLUSTERS_Cv1 =3
CONST_NUM_CLUSTERS_Cv2 =3



contributionsAv1 = dataAv1[:,CONST_CONTRIBUTION_COL_ABv].astype(np.float)
contributionsBv2 = dataBv2[:,CONST_CONTRIBUTION_COL_ABv].astype(np.float)

contributionsCv1 = dataCv1[:,CONST_CONTRIBUTION_COL_Cv].astype(np.float)
contributionsCv2 = dataCv2[:,CONST_CONTRIBUTION_COL_Cv].astype(np.float)



contributionsAv1.shape = (CONST_NUM_TUPLES_Av1,1)
contributionsBv2.shape = (CONST_NUM_TUPLES_Bv2,1)

contributionsCv1.shape = (CONST_NUM_TUPLES_Cv1,1)
contributionsCv2.shape = (CONST_NUM_TUPLES_Cv2,1)

contribution_consumptionsAv1 = dataAv1[:,[CONST_CONSUMPTION_COL_ABv,CONST_CONTRIBUTION_COL_ABv]].astype(np.float)
contribution_consumptionsBv2 = dataBv2[:,[CONST_CONSUMPTION_COL_ABv,CONST_CONTRIBUTION_COL_ABv]].astype(np.float)

#print contribution_consumptions1
#print contribution_consumptions2
contribution_consumptionsAv1.shape = (CONST_NUM_TUPLES_Av1,2)
contribution_consumptionsBv2.shape = (CONST_NUM_TUPLES_Bv2,2)

av1 = dataAv1[:,0].astype(np.float)
av1.shape = (CONST_NUM_TUPLES_Av1,1)

bv2 = dataBv2[:,0].astype(np.float)
bv2.shape = (CONST_NUM_TUPLES_Bv2,1)

cv1 = dataCv1[:,0].astype(np.float)
cv1.shape = (CONST_NUM_TUPLES_Cv1,1)

cv2 = dataCv2[:,0].astype(np.float)
cv2.shape = (CONST_NUM_TUPLES_Cv2,1)

#cv1cv2 = data2[:,[0,1]].astype(np.float)
#av1bv2.shape = (CONST_NUM_TUPLES_STATE2,2)

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


clfAv1 = KMeans(CONST_NUM_CLUSTERS_Av1)
clfAv1.fit(contributionsAv1)
labelsAv1 = clfAv1.labels_
centersAv1 = clfAv1.cluster_centers_

print labelsAv1
#print ("==========================")
#print contribution_consumptions1[:,[0]]
#print contribution_consumptions1[:,[1]]
resultsAv1 = np.zeros((CONST_NUM_TUPLES_Av1,2))
resultsAv1[:,0] = av1.transpose()
resultsAv1[:,1] = labelsAv1.transpose()



clfBv2 = KMeans(CONST_NUM_CLUSTERS_Bv2)
clfBv2.fit(contributionsBv2)
labelsBv2 = clfBv2.labels_
centersBv2 = clfBv2.cluster_centers_

print labelsBv2
resultsBv2 = np.zeros((CONST_NUM_TUPLES_Bv2,2))
resultsBv2[:,0] = bv2.transpose()
resultsBv2[:,1] = labelsBv2.transpose()


clfCv1 = KMeans(CONST_NUM_CLUSTERS_Cv1)
clfCv1.fit(contributionsCv1)
labelsCv1 = clfCv1.labels_
centersCv1 = clfCv1.cluster_centers_

print labelsCv1
resultsCv1 = np.zeros((CONST_NUM_TUPLES_Cv1,2))
resultsCv1[:,0] = cv1.transpose()
resultsCv1[:,1] = labelsCv1.transpose()



clfCv2 = KMeans(CONST_NUM_CLUSTERS_Cv2)
clfCv2.fit(contributionsCv2)
labelsCv2 = clfCv2.labels_
centersCv2 = clfCv2.cluster_centers_

print labelsCv2
resultsCv2 = np.zeros((CONST_NUM_TUPLES_Cv2,2))
resultsCv2[:,0] = cv2.transpose()
resultsCv2[:,1] = labelsCv2.transpose()


#clf2 = KMeans(CONST_NUM_CLUSTERS_STATE2)
#clf2.fit(contribution_consumptions2)
#labels2 = clf2.labels_
#centers2 = clf2.cluster_centers_
#
#print labels2


#output the information of Av1
clusters = []
for i in range(0,CONST_NUM_CLUSTERS_Av1):
    clusters.append([])

for i in range(0,CONST_NUM_TUPLES_Av1):
   clusters[int(resultsAv1[i,1])].append(contribution_consumptionsAv1[i])
#print ("============")
#a = np.array(clusters[0])
#print ("===========")
#print a[:,1]
#print a[:,0]

for i in range(0, CONST_NUM_CLUSTERS_Av1):
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

    outputFile.write("1,"+str(i)+","+ str(max(contribution)) +","+ str(min(contribution))+","+str(max(consumption))+","+str(min(consumption))+","+str(contribution_P50)+","+ str(consumption_P50)+","+str(np.mean(contribution))+","+ str(np.mean(consumption))+ ","+str(contribution_P50/consumption_P50)+'\n')


#    print ("lable: %d, dataNum: %d, max_contribution: %d, max_consumption: %d, min_contribution: %d, min_consumption: %d, centers_contribution %d, centers_consumprion: %d, 50 percentile_contribution: %s, 50 percent_consumption: %s" % (i, len(clusters[i]), max(contribution),max(consumption), min(contribution),min(consumption),  centersAv1[i][0], "{:,}".format(contribution_P50), "{:,}".format(consumption_P50)))
    #print ("c+/c-: %f\n",contribution_P50/consumption_P50)
    #print contribution/consumption
#    #print clusters[i]
##outputFile.write(CONST_NUM_CLUSTERS_STATE1,0,0,0,0)
#outputFile.write("1,"+str(CONST_NUM_CLUSTERS_STATE1)+",0,0,0,0"+'\n')
#
#
#print ("=====================")
#


#output Bv2 clustering infor

#results2 = np.zeros((CONST_NUM_TUPLES_STATE2,2))
#results2[:,0] = av1.transpose()
#results2[:,1] = labels2.transpose()

print ("========")
#print results2[4,1]

clusters = []
for i in range(0,CONST_NUM_CLUSTERS_Bv2):
    clusters.append([])

for i in range(0,CONST_NUM_TUPLES_Bv2):
    clusters[int(resultsBv2[i,1])].append(contribution_consumptionsBv2[i])

for i in range(0, CONST_NUM_CLUSTERS_Bv2):
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

    outputFile.write("2,"+str(i)+","+ str(max(contribution)) +","+ str(min(contribution))+","+str(max(consumption))+","+str(min(consumption))+","+str(contribution_P50)+","+ str(consumption_P50)+","+str(np.mean(contribution))+","+ str(np.mean(consumption))+ ","+str(contribution_P50/consumption_P50)+'\n')

#    print ("lable: %d, dataNum: %d, max_contribution: %d, max_consumption: %d, min_contribution: %d, min_consumption: %d, centers_contribution %d, centers_consumprion: %d, 50 percentile_contribution: %s, 50 percent_consumption: %s" % (i, len(clusters[i]), max(contribution),max(consumption), min(contribution),min(consumption),  centersBv2[i][0], "{:,}".format(contribution_P50), "{:,}".format(consumption_P50)))
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

DSAv1 = tree.DecisionTreeClassifier(class_weight=None, criterion='gini', max_depth=7,max_features=None, max_leaf_nodes=None, min_samples_leaf=1, min_samples_split=2, min_weight_fraction_leaf=0.0,presort=False, random_state=100, splitter='best')

DSBv2 = tree.DecisionTreeClassifier(class_weight=None, criterion='gini', max_depth=7,max_features=None, max_leaf_nodes=None, min_samples_leaf=1, min_samples_split=2, min_weight_fraction_leaf=0.0,presort=False, random_state=100, splitter='best')

DS_Av1= DSAv1.fit(av1,labelsAv1)
DS_Bv2= DSBv2.fit(bv2,labelsBv2)

#import pickle
from sklearn.externals import joblib
joblib.dump(DS_Av1,'DS_Av1.pkl')
joblib.dump(DS_Bv2,'DS_Bv2.pkl')

#import graphviz
dot_data = tree.export_graphviz(DS_Av1, out_file="DS_Av1.dot",class_names=True,filled=True, rounded=True,special_characters=True)
dot_data = tree.export_graphviz(DS_Bv2, out_file="DS_Bv2.dot",class_names=True,filled=True, rounded=True,special_characters=True)
#graph = graphviz.Source(dot_data)
#graph.render("DS")

import time
start = time.clock()
a = DS_Bv2.predict(bv2)
b = DS_Av1.predict(av1)
end = time.clock()
print str(end-start)
print b
print a

estimator = DS_Av1
n_nodes = estimator.tree_.node_count
children_left = estimator.tree_.children_left
children_right = estimator.tree_.children_right
feature = estimator.tree_.feature
threshold = estimator.tree_.threshold


# The tree structure can be traversed to compute various properties such
# as the depth of each node and whether or not it is a leaf.
node_depth = np.zeros(shape=n_nodes, dtype=np.int64)
is_leaves = np.zeros(shape=n_nodes, dtype=bool)
stack = [(0, -1)]  # seed is the root node id and its parent depth
while len(stack) > 0:
    node_id, parent_depth = stack.pop()
    node_depth[node_id] = parent_depth + 1

    # If we have a test node
    if (children_left[node_id] != children_right[node_id]):
        stack.append((children_left[node_id], parent_depth + 1))
        stack.append((children_right[node_id], parent_depth + 1))
    else:
        is_leaves[node_id] = True

print("The binary tree structure has %s nodes and has "
      "the following tree structure:"
      % n_nodes)
for i in range(n_nodes):
    if is_leaves[i]:
        print("%snode=%s leaf node." % (node_depth[i] * "\t", i))
    else:
        print("%snode=%s test node: go to node %s if X[:, %s] <= %s else to "
              "node %s."
              % (node_depth[i] * "\t",
                 i,
                 children_left[i],
                 feature[i],
                 threshold[i],
                 children_right[i],
                 ))
print()

