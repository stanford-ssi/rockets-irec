# -*- coding: utf-8 -*-

#Orien is a god for helping me debug the parser

#import pprint
#from IPython.display import display
#import platform

#creates class wind tags that holds the date and time

class WindDate: 
    #hr is stored in the 24-hr clock
    def __init__(self,year,month,day,hr):
        self.y = year
        self.m = month
        self.d = day
        self.hour = hr
    
    #useful tool for printing accessed values in the map
    def __str__(self):
        return "the year is: " + str(self.y) + "the month is" + str(self.m) + "the day is: " + str(self.d) + "the hour is: " + str(self.hour)
    
    #add functions that allow for lookups - asks "my month, day, hour, same as theirs?"
    def __eq__(self,other):
        return (self.m == other.m and self.d == other.d and self.hour == other.hour and self.y == other.y)
        
    #python compares key values by asking if they are the same memory address, not same content. so the program needs this hash function to allow for key lookups
    def __hash__(self):
        return self.d + self.y + self.m + self.hour #TODO: make more efficient so you can't allow the stuff to get hashed into the same bucket

#creates class that holds the wind values - includes direction and speed
class WindValues:
    #hr is stored in the 24-hr clock
    def __init__(self,direction, speed):
        self.d = direction
        self.s = speed

    #useful tool for printing accessed values in the map
    def __str__(self):
        return "the direction is: " + str(self.d) + "the month is" + str(self.s)
        
#parses the textfile, populating a hashmap with those classes for each wind date and wind value
advance_amt = 44 #num characters the filereader needs to move right before reaching the start of the date entry
parse_length = 87 #num characters the line that starts with the date goes to
data_file = open('hourly ASOS data 2012-2017.txt') 

#advances file to line where info actually starts (past title line and category headings line)
data_file.readline()
data_file.readline()

#create the dictionary (python's map) that we're going to add the file vals to
windMap = {} #format for initializing a dictionary

while(True):
    currentLine = data_file.readline()
    if(currentLine == ""):
        break 
    
    #extracts the data that's different from each line in the data file (some stuff like location is always the same)
    toParse = currentLine[44:87]
    parsedVals = toParse.split(",") #windTag variables and WindValues variables all stored in single list entry because there is no comma separating them 
    
    #populate the class variables for an individual line in the file
    dateInfo = parsedVals[0]
    y = int(dateInfo[0:4])
    m = int(dateInfo[4:6])
    d = int(dateInfo[6:8])
    hour = int(parsedVals[1])
    
    windKey = WindDate(y,m,d,hour)
    
    if(windKey.m == 1 and windKey.d == 18 and windKey.y == 2012 and windKey.hour == 153):
       print(windKey)
    
    #populate the wind vals class
    #skip over the empty space that gets parsed in list entry 5
    direction = int(parsedVals[6])
    speed = float(parsedVals[8].strip())
    specificWindVals = WindValues(direction,speed)

    #add both populated classes to the map
    windMap[windKey] = specificWindVals


data_file.close() #frees up any system allocations taken up by the file. TODO: call closer later?

#test the program by accessing different values in the map
test = WindDate(2012,1,18,153)

testWindValues = windMap[test]
print(testWindValues.s)

#TODO: break up parsing file and graphic creating file

###
#create the histogram for speed
###

###
#create the polar plot for direction
###