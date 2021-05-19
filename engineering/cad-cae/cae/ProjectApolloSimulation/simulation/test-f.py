#!/usr/bin/env python
#Use python 3
import sys
import math
import collections
import matplotlib.pyplot as plt
import numpy as np
import scipy
import matplotlib

vinfo=sys.version_info
#print('version:{}'.format(vinfo))
assert vinfo.major >= 3 and vinfo.minor>=6
'''
Test functions in finite_volume.py

'''
from finite_volume import *
print('start testing')

a=np.array([1,3,4,5,6,7,8,20])

aplus=diffplus(a,1000)
aminus=diffminus(a,99)
print('a=  {}'.format(a))
print('a+ ={}'.format(aplus))
print('a- ={}'.format(aminus))

print('a+.5= {}'.format(jplus5(a,99,1000)))
print('a-.5= {}'.format(jminus5(a,99,1000)))      
