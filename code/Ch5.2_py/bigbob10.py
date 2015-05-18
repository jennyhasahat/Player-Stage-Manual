# Simple python client example 
# Based on example0.cc from player distribution
# K. Nickels 7/24/13

import math, sys, os
sys.path.append('/usr/local/lib64/python2.7/site-packages/')
from playercpp import *
bfstr = '';

# Make proxies for Client, blobfinder
robot = PlayerClient("localhost");
bf = BlobfinderProxy(robot,0);

# Read from the proxies
robot.Read();


# Print stuff out for fun
for i in range(bf.GetCount()):
	blob = bf.GetBlob(i);
	bfstr += 'BLOB %d, ' % i
	bfstr += 'color %d, ' % blob.color
	bfstr += 'x %f, ' % blob.x
	bfstr += '\n'
	print bfstr

	robot.Read()
