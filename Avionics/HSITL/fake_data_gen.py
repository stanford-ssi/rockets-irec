import os
import random

if __name__ == '__main__':
	try:
		os.remove("fakedata/fake1.csv")
	except:
		pass
	file = open("fakedata/fake1.csv", "w+")
	for i in range(0,10001):
		file.write(str(round(random.random()*3)) + ',' +  str(i*1000.0) + ',' + str(random.random()*100)+'\n')

