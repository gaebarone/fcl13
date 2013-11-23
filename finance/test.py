import stock
from stock import stock

import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt

iters=100 
y = np.zeros(iters)
x = np.zeros(iters)
print x
aStock = stock(iters)
i=0
while i < iters :
    y[i]= aStock.next()
    x[i]= aStock.getTime()
    i+=1

plt.plot(x,y,color='blue',marker='o',label='a stock')
plt.legend(loc='upper left',frameon=False)
plt.xlabel("time [a.u]")
plt.ylabel("value [a.u.]")
plt.show()
