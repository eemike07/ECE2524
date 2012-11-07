#!/usr/bin/env python

# ECE 2524 Homework 4: Inventory Management
# Michael Surratt

import fileinput, argparse, ast, csv, sys
from operator import itemgetter

parser = argparse.ArgumentParser(description='Inventory Management')
parser.add_argument("-f, --data-file", action='store',  dest=  'fileName',   help="path to the data file to read at startup")
args = parser.parse_args()
List = []

def add__(arg):
	parse_str = arg.replace('{',"")
	parse_str = parse_str.replace('}',"")
	parse_str = parse_str.replace(',',"")
	parse_str = parse_str.replace('\'',"><")
	parse_str = parse_str.replace(':',"><")
	parse_str = parse_str.replace(' ',"")
	parse_str = parse_str.strip()
	argString = parse_str.split('><')	
	
	List.append(argString[4] + "___" + argString[9] + "___" + argString[14] + "___" + argString[18])
	f = open(args.fileName, 'a')
	f.write(argString[4] + "___" + argString[9] + "___" + argString[14] + "___" + argString[18])
	print "Item Successfully Added."
	f.close()

def list__(arg):
    try:
        global List
        allFlag = 0
        withFlag = 0
        strCheck = ""
        EqNum = " "
        actionoccur = 0
        resultsfound = 0  
        
        argString = arg.rsplit()  
        for line in argString:
            if line == "all":
                allFlag = 1
            if line == "with":
                withFlag = 1
            checkEq = line.rsplit("=")
            if checkEq[0] == "Footprint" or checkEq[0] == "PartID" or checkEq[0] == "Quantity":
                actionoccur = 1
                strCheck = checkEq[0]
                EqNum = checkEq[1]
    
        if allFlag is 1 and withFlag is 0:
            actionoccur = 1
            counter = 0
            tabSet = "\t\t"
            for line in List:
                splitList = line.rsplit("___")

                if splitList[1].__len__() > 22:
                    tabSet = "\t"
                else:
                    tabSet = "\t\t"       
                if counter is 0:
                    sys.stdout.write("Footprint" + "\t" + "Description" + "\t\t\t" + "PartID" + "\t\t" + "Quantity \n")
                else:   
                    sys.stdout.write(splitList[0] + "\t\t" + splitList[1] + tabSet + splitList[2] + "\t\t" + splitList[3])
                counter = 1
    
        if allFlag is 1 and withFlag is 1:
            if strCheck == "Footprint":
                setSplit = 0                
            if strCheck == "PartID":
                setSplit = 2
            if strCheck == "Quantity":
                setSplit = 3
            counter = 0
            tabSet = "\t\t"
            for line in List:
                splitList = line.rsplit("___")

                if splitList[1].__len__() > 22:
                    tabSet = "\t"
                else:
                    tabSet = "\t\t"                       
                test1 = splitList[setSplit].replace("\t\n ","")
                test2 = EqNum.replace("\t\n ","")
             
                if counter is 0:
                    sys.stdout.write("Footprint" + "\t" + "Description" + "\t\t\t" + "PartID" + "\t\t" + "Quantity \n")
                elif test1 == test2 or splitList[setSplit].split() == EqNum.split():   
                    sys.stdout.write(splitList[0] + "\t\t" + splitList[1] + tabSet + splitList[2] + "\t\t" + splitList[3])
                    resultsfound = 1
                counter = 1
        sys.stdout.write("\n")
        if actionoccur == 0:
            print "Invalid Input Command."
        if resultsfound == 0 and withFlag is 1:
            print "No results were found."
        sys.stdout.flush()
    except IndexError as e:
        print "Invalid Input Command."
    except UnboundLocalError as e:
        print "Invalid Input Command."
        
def rmv__(arg):
    try:
        global List
        actionoccur = 0
        resultsfound = 0
        argString = arg.rsplit()
        for line in argString:
            checkEq = line.rsplit("=")
            if checkEq[0] == "Footprint" or checkEq[0] == "PartID" or checkEq[0] == "Quantity":
                strCheck = checkEq[0]
                EqNum = checkEq[1]
                       
        if strCheck == "Footprint":
            setSplit = 0
        if strCheck == "PartID":
            setSplit = 2
        if strCheck == "Quantity":
            setSplit = 3
        
        ListPos = 0
        for line in List:
            splitList = line.rsplit("___")
            
            if ListPos is 0:
                pass
            elif splitList[setSplit].split() == EqNum.split():
                del List[ListPos]
                actionoccur = 1

            ListPos = ListPos + 1
        splitList = List[-1].rsplit("___")
        
        if splitList[setSplit].split() == EqNum.split():
            del List[-1]
            actionoccur = 1
            
        if actionoccur == 1:
			f = open(args.fileName, 'r+')
			f.truncate()
			for line in List:
				f.write(line)
			f.close()
			print "Item(s) has been successfully removed."
	elif actionoccur == 0:
			print "No item has been removed."
  
    except IndexError as e:
        print "Invalid Input Command."
    except UnboundLocalError as e:
        print "Invalid Input Command."
        
def set__(arg):
	print "This function has not been completed"
	pass


try:
	f = open(args.fileName , 'r')
	for line in f:
		List.append(line)

except IOError as e:
	print "The specified file does not exist."

for stream in iter(sys.stdin.readline, ''):
	commands = {'add': add__, 'remove': rmv__, 'set': set__, 'list': list__}
	(action, space, strQuery) = stream.partition(" ")
	strQuery = strQuery.rstrip("\n")
	try:
		commands[action](strQuery)
	except KeyError as e:
		print "Invalid input command."
