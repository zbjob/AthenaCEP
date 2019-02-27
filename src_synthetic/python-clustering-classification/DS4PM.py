from sklearn.externals import joblib
import numpy as np

#DSTree model for each state and each time slice. It is a 2D array
DS_State = []
DS_TimeSlice_state1 = []
DS_TimeSlice_state2 = []

DS_TimeSlice_state1.append(joblib.load('/home/bo/CEP_load_shedding/src_PM_Distribution_test/python-clustering-classification/DS_STATE1.pkl'))
DS_TimeSlice_state2.append(joblib.load('/home/bo/CEP_load_shedding/src_PM_Distribution_test/python-clustering-classification/DS_STATE2.pkl'))


DS_State.append(DS_TimeSlice_state1)
DS_State.append(DS_TimeSlice_state2)
#for i in range(0,4):
#    DS_TimeSlice = []
#    for j in range(0,2):
#        DS_TimeSlice.append(joblib.load('DS.pkl'))
#    DS_State.append(DS_TimeSlice)

#DST = joblib.load('DS.pkl')
#def DS(s,ts,val):
#   return DS_State[s][ts].predict(val)

#DS1 loads all the models for all the states
#It recieves the string as a parameter, the format is state, clusters/time slice, and actual payload: the value or the tuple of values to be clustered
#each state buffer has multiple clusters divided by different time slices. which means, each timeslice of each statebuffer has its unique cluster model
#There for the array of models should be [No.state][No.timeslice]

def DS1(s):
    t = s.split(',')
    t = np.array(t)
    t = t.astype(np.int)
    l = len(t)
    t.shape = (1,l)
    attr = t[0,2:l]
    attr.shape = (1,l-2)
    return DS_State[t[0,0]][t[0,1]].predict(attr)

#v1 = np.array(45)
#a = 45
#b = 50
#v = np.array([a,b])
#v.shape = (1,2)
#print v
##print len(v[0])
#print DS(0,0,v1)
#print DS(1,0,v)
#
#s = '1,0,45,50'
#t = s.split(',')
#t = np.array(t)
#t = t.astype(np.int)
#l = len(t)
#t.shape = (1,l)
#vv = t[0,2:l]
#vv.shape = (1,l-2)
#print "predict 45,50"
#print DS_State[t[0,0]][t[0,1]].predict(vv)
