import stock
from stock import stock
import virtualMarket
from  virtualMarket import virtualMarket
import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt


aMarket = virtualMarket(1,100) 
aMarket.Evolve()
print 

y = aMarket.getStock(0).getAll()
x = aMarket.getStock(0).getTimes()

plt.plot(x,y,color='blue',marker='o',label='a stock')
plt.legend(loc='upper left',frameon=False)
plt.xlabel("time [a.u]")
plt.ylabel("value [a.u.]")
plt.show()
