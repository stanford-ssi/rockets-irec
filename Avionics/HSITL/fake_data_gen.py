import os
import random

if __name__ == '__main__':
	try:
		os.remove("fakedata/fake1.csv")
	except:
		pass
	file = open("fakedata/fake1.csv", "w+")
	for i in range(0,10001):
		if round(random.random()*3) == 0:
			file.write('1' + ',' +  str(i*1000.0) + ',' + str(random.random()*100) + ',' + str(random.random()*100)+'\n')

		if round(random.random()*3) == 1:
			file.write('2' + ',' +  str(i*1000.0) + ','  + str(random.random()*100)+'\n')
			
		if round(random.random()*3) == 2:
			file.write('3' + ',' +  str(i*1000.0) + ',' + str(random.random()*100) + ',' + str(random.random()*100)+'\n')

		if round(random.random()*3) == 3:
			file.write('4' + ',' +  str(i*1000.0) + ',' + str(random.random()*100) + ',' + str(random.random()*100) + ',' + str(random.random()*100)+'\n')
				