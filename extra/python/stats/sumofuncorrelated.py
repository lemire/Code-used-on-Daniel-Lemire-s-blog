# https://www.johndcook.com/blog/2017/09/10/negative-correlation-introduced-by-success/
from numpy import arange
from scipy.stats import norm, pearsonr
import matplotlib.pyplot as plt

# Correlation.
# The function pearsonr returns correlation and a p-value.
def corr(x, y):
    return pearsonr(x, y)[0]

x = norm.rvs(100, 15, 10000)
y = norm.rvs(100, 15, 10000)
z = x + y

span = arange(80, 260, 10)
c = [ corr( x[z > low], y[z > low] ) for low in span ]

plt.plot( span, c )
plt.xlabel( "minimum sum" )
plt.ylabel( "correlation coefficient" )
plt.show()
