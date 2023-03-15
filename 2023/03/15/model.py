from math import *
from random import *
import numpy

def proba(B):
    return 1/(1+exp(10 - B))

def bug_found(B, W):
    found = 0
    for i in range(W):
        if(B == 0): break
        if(random() <= proba(B)):
            found += 1
            B -= 1
    return found

def expected_bug_found(B, W):
    return numpy.mean([bug_found(B,W) for i in range(1000)])


def expected_recall_precision(B, W):
    bf = expected_bug_found(B, W)
    #print(bf, W)
    return [bf/B,bf/W]
 
for W in range(1,1000):
    B = 10
    r,p = expected_recall_precision(B,W)
    print(W, r, p, 2/(1/r+1/p))



