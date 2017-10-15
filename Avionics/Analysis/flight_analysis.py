""" 
Used for analyzing the flight of the SSI 2017 IREC rocket

The file is broken up into various useful functions for analysis
and these functions are called at the bottom of the file.

NOTE:
You must copy the folder SA_2017-6-22_Skybass to the parent directory in order to use this file.
Then, run clean_data.py to clean the flight data into a new directory, which will then be read by 
this script.

Author: John Dean
Date: Oct 2017
"""

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import csv 
import math
from mpl_toolkits.mplot3d import Axes3D
import pickle
#from vpython import *
import os


DIRNAME = 'SA_2017-6-22_Skybass_Clean/'
pi = math.pi


def readData():
	global raw_dat
	bno_data = pd.DataFrame.from_csv(DIRNAME+'bno.csv')
	mma_data = pd.DataFrame.from_csv(DIRNAME+'mma.csv')
	bmp_data = pd.DataFrame.from_csv(DIRNAME+'bmp.csv')
	raw_dat = pd.concat([bno_data, mma_data, bmp_data],axis=1)
	raw_dat.index = (raw_dat.index - min(raw_dat.index))/1e7
	
	raw_dat['roll'] = np.nan
	raw_dat['pitch'] = np.nan
	raw_dat['heading'] = np.nan

	for index, row in raw_dat.iterrows():
		if(not pd.isnull(row.a_x)):
			qw,qx,qy,qz = normQuats(row)
			roll = 180/pi * math.atan2(qw*qx + qy*qz, 0.5 - qx*qx - qy*qy)
			pitch = 180/pi * math.asin(2 * (qw*qy - qx*qz))
			heading = 180/pi * math.atan2(qw*qz + qx*qy, 0.5 - qy*qy - qz*qz)
			raw_dat['roll'][index] = roll
			raw_dat['pitch'][index] = pitch
			raw_dat['heading'][index] = heading
	

def normQuats(row):
	q_w = row.q_w
	q_x = row.q_x
	q_y = row.q_y
	q_z = row.q_z
	qnorm = 1/(q_w*q_w + q_x*q_x + q_y*q_y + q_z*q_z)**.5
	q_w *= qnorm
	q_x *= qnorm
	q_y *= qnorm
	q_z *= qnorm
	return q_w, q_x, q_y, q_z 	

def plot9vars():
	plt.subplot(331)
	plt.plot(raw_dat.w_x.dropna())
	plt.title('w_x')

	plt.subplot(334)
	plt.plot(raw_dat.w_y.dropna())
	plt.title('w_y')

	plt.subplot(337)
	plt.plot(raw_dat.w_z.dropna())
	plt.title('w_z')

	plt.subplot(332)
	plt.plot(raw_dat.th_x.dropna())
	plt.title('th_x')

	plt.subplot(335)
	plt.plot(raw_dat.th_y.dropna())
	plt.title('th_y')

	plt.subplot(338)
	plt.plot(raw_dat.th_z.dropna())
	plt.title('th_z')

	plt.subplot(333)
	plt.plot(raw_dat.heading.dropna())
	plt.title('heading')

	plt.subplot(336)
	plt.plot(raw_dat.roll.dropna())
	plt.title('roll')

	plt.subplot(339)
	plt.plot(raw_dat.pitch.dropna())
	plt.title('pitch')
	plt.show()

def plotOrientation():
	endloc = 600000000
	X = np.sin(pi*2/360*(90+raw_dat.pitch.loc[0:endloc].values)) * np.cos(pi*2/360*raw_dat.heading.loc[0:endloc].values)
	Y = np.sin(pi*2/360*(90+raw_dat.pitch.loc[0:endloc].values)) * np.sin(pi*2/360*raw_dat.heading.loc[0:endloc].values)
	Z = np.cos(pi*2/360*(90+raw_dat.pitch.loc[0:endloc].values))
	#X[0] = 0
	#Y[0] = 0
	#Z[0] = 0
	fig = plt.figure()
	ax = fig.add_subplot(111, projection='3d')
	ax.plot(X,Y,Z)
	ax.axis('equal')
	plt.show()

def plotAccels():
	V = np.array([np.linalg.norm(x) for x in proc_dat['abs_v'].values])
	A1 = [x[0] for x in proc_dat['rel_a'].dropna().values]
	t1 = [x for x in proc_dat['rel_a'].dropna().index.values]
	A2 = [x for x in raw_dat['a_x'].dropna().values]
	t2 = [x for x in raw_dat['a_x'].dropna().index.values]
	A3 = [x for x in raw_dat['mma_y'].dropna().values]
	t3 = [x for x in raw_dat['mma_y'].dropna().index.values]
	plt.plot(t1,A1,t2,A2,t3,A3)
	plt.legend(['both','bno','mma'])


def plotTrajectory3D():
	fig = plt.figure()
	ax = fig.add_subplot(111, projection='3d')
	X = np.array([x[0] for x in proc_dat['abs_x'].dropna().values])
	Y = np.array([x[1] for x in proc_dat['abs_x'].dropna().values])
	Z = np.array([x[2] for x in proc_dat['abs_x'].dropna().values])
	ax.plot(X,Y,Z)

	# Create cubic bounding box to simulate equal aspect ratio
	max_range = np.array([X.max()-X.min(), Y.max()-Y.min(), Z.max()-Z.min()]).max()
	Xb = 0.5*max_range*np.mgrid[-1:2:2,-1:2:2,-1:2:2][0].flatten() + 0.5*(X.max()+X.min())
	Yb = 0.5*max_range*np.mgrid[-1:2:2,-1:2:2,-1:2:2][1].flatten() + 0.5*(Y.max()+Y.min())
	Zb = 0.5*max_range*np.mgrid[-1:2:2,-1:2:2,-1:2:2][2].flatten() + 0.5*(Z.max()+Z.min())
	# Comment or uncomment following both lines to test the fake bounding box:
	for xb, yb, zb in zip(Xb, Yb, Zb):
		ax.plot([xb], [yb], [zb], 'w')
	plt.show()


def Animation():
	endloc = 600000000
	X = np.sin(pi*2/360*(90+raw_dat.pitch.loc[0:endloc].values)) * np.cos(pi*2/360*raw_dat.heading.loc[0:endloc].values)
	Y = np.sin(pi*2/360*(90+raw_dat.pitch.loc[0:endloc].values)) * np.sin(pi*2/360*raw_dat.heading.loc[0:endloc].values)
	Z = np.cos(pi*2/360*(90+raw_dat.pitch.loc[0:endloc].values))
	rocket = arrow(pos=vector(0,0,0),axis=vector(0,1,0),length=2)
	xx = arrow(pos=vector(0,0,0),axis=vector(1,0,0),length=0.5,color=vector(1,0,0))
	yy = arrow(pos=vector(0,0,0),axis=vector(0,1,0),length=0.5,color=vector(0,1,0))
	zz = arrow(pos=vector(0,0,0),axis=vector(0,0,1),length=0.5,color=vector(0,0,1))

	camera = vector(0,0,0) # for generality; need not be at origin
	scene.forward = vector(-1,-1,-1)

	for i in range(1,round(len(X)/2)):

		print(i*2)
		rate(10)
		rocket.axis=vector(X[i*2],Z[i*2],Y[i*2])
		#os.popen('import -window 0x6000183 frames/vp'+str(i).zfill(4)+'.gif')
	print('done')


def processData():
	global proc_dat
	proc_dat = pd.DataFrame(index=raw_dat.index.values, columns=['abs_a','abs_v','abs_x','abs_dir','rel_a','rel_v','alpha',])
	for key in proc_dat:
		proc_dat[key].loc[0.0] = np.zeros(3)

	abs_a = np.zeros(3)
	abs_v = np.zeros(3)
	abs_x = np.zeros(3)
	rel_a = np.zeros(3)
	abs_g = np.array([0,0,-9.8])
	rel_g = np.array([0,0,-9.8])

	#rotation matrix
	R = np.zeros((3,3)); 

	#loop variables
	i = 0
	last_t = 0
	
	for index, row in raw_dat.iterrows():
		if (index > 54):
			break
		#if we have new bno data
		if (not pd.isnull(raw_dat['a_x'].loc[index])):
			q_w,q_x,q_y,q_z = normQuats(row)
			R = np.array(  [[q_w**2 + q_x**2 - q_y**2 - q_z**2 , 2*q_x*q_y - 2*q_w*q_z		   ,  2*q_x*q_z + 2*q_w*q_y],
							[2*q_x*q_y + 2*q_w*q_z   	   , q_w**2 - q_x**2 + q_y**2 - q_z**2 ,  2*q_y*q_z - 2*q_w*q_x],
							[2*q_x*q_z - 2*q_w*q_y 		   , 2*q_y*q_z + 2*q_w*q_x         ,  q_w**2 - q_x**2 - q_y**2 + q_z**2]] )
			rel_g = R.transpose()@abs_g
			rel_a = np.array([row.a_x,row.a_y,row.a_z]).transpose()
			if abs(rel_a[0] - rel_g[0]) > 38:
				mma_last = raw_dat['mma_y'][0:index].last_valid_index()
				rel_a[0] = raw_dat['mma_y'].loc[mma_last]
			abs_a = R@rel_a

		if (i >=1):
			proc_dat['rel_a'].loc[index] = rel_a
			proc_dat['abs_a'].loc[index] = abs_a
			proc_dat['abs_v'].loc[index] = proc_dat['abs_v'].loc[last_t] + abs_a * (index-last_t)
			proc_dat['abs_x'].loc[index] = proc_dat['abs_x'].loc[last_t] + proc_dat['abs_v'].loc[index] * (index-last_t)
			proc_dat['abs_dir'].loc[index] = R@np.array([1,0,0]).transpose()
			proc_dat['alpha'].loc[index] = np.arccos(proc_dat['abs_dir'].loc[index]@proc_dat['abs_v'].loc[index]/np.linalg.norm(proc_dat['abs_v'].loc[index]))
		i += 1
		last_t = index


def save(objs):
	for obj in objs:
		if obj == 'raw_dat':
			with open('raw_dat.pickle','wb') as f:
				pickle.dump(raw_dat,f)
		if obj == 'proc_dat':
			with open('proc_dat.pickle','wb') as f:
				pickle.dump(proc_dat,f)


def load(objs):
	global raw_dat
	global proc_dat
	for obj in objs:
		if obj == 'raw_dat':
			with open('raw_dat.pickle','rb') as f:
				raw_dat = pickle.load(f)
		if obj == 'proc_dat':
			with open('proc_dat.pickle','rb') as f:
				proc_dat = pickle.load(f)

#***** Global VAriables *****#
raw_dat = pd.DataFrame()
proc_dat = pd.DataFrame()
DIRNAME = 'SA_2017-6-22_Skybass_Clean/'
pi = math.pi


#**** Do things *****# 
readData();
processData();
save(['proc_dat','raw_dat'])
#load(['raw_dat','proc_dat'])
#processData()
#save(['proc_dat'])
'''
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
t_s = 30
t_e = 32
skip = 10
factor = 50
X = np.array([x[0] for x in proc_dat['abs_x'].dropna()[t_s:t_e].values])
Y = np.array([x[1] for x in proc_dat['abs_x'].dropna()[t_s:t_e].values])
Z = np.array([x[2] for x in proc_dat['abs_x'].dropna()[t_s:t_e].values])
ax.plot(X,Y,Z)

Xv = np.array([x[0] for x in proc_dat['abs_x'].dropna()[t_s:t_e:skip].values])
Yv = np.array([x[1] for x in proc_dat['abs_x'].dropna()[t_s:t_e:skip].values])
Zv = np.array([x[2] for x in proc_dat['abs_x'].dropna()[t_s:t_e:skip].values])

U = np.array([x[0] for x in proc_dat['abs_dir'].dropna()[t_s:t_e:skip].values]) * factor
V = np.array([x[1] for x in proc_dat['abs_dir'].dropna()[t_s:t_e:skip].values]) * factor
W = np.array([x[2] for x in proc_dat['abs_dir'].dropna()[t_s:t_e:skip].values]) * factor

ax.quiver(Xv,Yv,Zv,U,V,W,color='g',arrow_length_ratio=0.1)

# Create cubic bounding box to simulate equal aspect ratio
max_range = np.array([X.max()-X.min(), Y.max()-Y.min(), Z.max()-Z.min()]).max()
Xb = 0.5*max_range*np.mgrid[-1:2:2,-1:2:2,-1:2:2][0].flatten() + 0.5*(X.max()+X.min())
Yb = 0.5*max_range*np.mgrid[-1:2:2,-1:2:2,-1:2:2][1].flatten() + 0.5*(Y.max()+Y.min())
Zb = 0.5*max_range*np.mgrid[-1:2:2,-1:2:2,-1:2:2][2].flatten() + 0.5*(Z.max()+Z.min())
# Comment or uncomment following both lines to test the fake bounding box:
for xb, yb, zb in zip(Xb, Yb, Zb):
	ax.plot([xb], [yb], [zb], 'w')
#plt.show()
'''
proc_dat['alpha'].loc[0.0] = 0
plt.plot(proc_dat['alpha'].dropna()*180/pi)
plt.xlabel('time (s)')
plt.ylabel('alpha ')
plt.show()



