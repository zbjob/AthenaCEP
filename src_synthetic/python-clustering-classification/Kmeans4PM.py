import numpy as np
from sklearn.cluster import KMeans

#define
#stream file, i
#index coloume for contribution and consumptions,
#the number of clusters

data = np.loadtxt('../src_NS/cep_cleanData/CleanedOutput.copy', dtype=np.str, delimiter=",")
#data = np.loadtxt('/home/zhaobo/HU/CEP_load_shedding/src_PM_Distribution_test/src_NS/cep_cleanData/CleanedOutput.copy', dtype=np.str, delimiter=",")
#data = np.loadtxt('/vol/home-vol2/pda/zhaobo/google_cluster/clusterdata-2011-2/CleanData/attr1.csv', dtype=np.str, delimiter=",")
#data = np.loadtxt('/vol/home-vol2/pda/zhaobo/CEP_load_shedding/src_PM_Distribution_test/src_NS/cep_cleanData/CleanedOutput.copy', dtype=np.str, delimiter=",")

CONST_CONTRIBUTION_COL = 2
CONST_CONSUMPTION_COL = 1
CONST_NUM_TUPLES = len(data)
CONST_NUM_CLUSTERS =3

contributions = data[:,CONST_CONTRIBUTION_COL].astype(np.float)
#consumptions  = data[:,1].astype(np.float)
av1 = data[:,0].astype(np.float)

contributions.shape = (CONST_NUM_TUPLES, 1)
#consumptions.shape = (CONST_NUM_TUPLES, 1)
av1.shape = (CONST_NUM_TUPLES,1)

nonzeroContributionIdx = np.nonzero(contributions)
nonzeroContribution = contributions[nonzeroContributionIdx]
nonzeroContribution.shape = (len(nonzeroContribution),1)

clf = KMeans(CONST_NUM_CLUSTERS)
labels = [CONST_NUM_CLUSTERS]*CONST_NUM_TUPLES
labels = np.array(labels)
labels.shape = (CONST_NUM_TUPLES,1)

clf.fit(nonzeroContribution)
nonzeroLabels = clf.labels_
centers = clf.cluster_centers_
print nonzeroLabels

labels[nonzeroContributionIdx] = nonzeroLabels

print labels.transpose()

results = np.zeros((CONST_NUM_TUPLES,2))
results[:,0] = av1.transpose()
results[:,1] = labels.transpose()

#print results[:,1]
clusters = []
for i in range(0,CONST_NUM_CLUSTERS+1):
    clusters.append([])

for i in range(0,CONST_NUM_TUPLES):
    clusters[int(results[i,1])].append(contributions[i])

for i in range(0, CONST_NUM_CLUSTERS):
    a = np.array(clusters[i])
    p = np.percentile(a, 50)
    print ("lable: %d, dataNum: %d, max: %d, min: %d,centers %d, 50 percentile: %s" % (i, len(clusters[i]), max(clusters[i]),min(clusters[i]),centers[i], "{:,}".format(p)))
    #print clusters[i]





from sklearn import tree

#DS = tree.DecisionTreeClassifier()

DS = tree.DecisionTreeClassifier(class_weight=None, criterion='gini', max_depth=5,max_features=None, max_leaf_nodes=None, min_samples_leaf=1, min_samples_split=2, min_weight_fraction_leaf=0.0,presort=False, random_state=100, splitter='best')
DS = DS.fit(av1,labels)

#import pickle
from sklearn.externals import joblib
joblib.dump(DS,'DS.pkl')

#import graphviz
dot_data = tree.export_graphviz(DS, out_file="DS2.dot",class_names=True,filled=True, rounded=True,special_characters=True)
#graph = graphviz.Source(dot_data)
#graph.render("DS")

import time
start = time.clock()
a = DS.predict(av1)
end = time.clock()
print str(end-start)
print a
