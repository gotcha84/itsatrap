import sys

if len(sys.argv) > 1:
	min_ele = 9999.0
	min_counter = -1
	counter = 0
	print sys.argv[1]
	with open(sys.argv[1]) as f:
		for line in f:
			tmp = line.split(" ")

			if tmp[0] == "v":
				if float(tmp[2]) < min_ele:
					print tmp
					min_ele = float(tmp[2])
					min_counter = counter
			counter+=1
	f.close()
	print min_ele, min_counter
