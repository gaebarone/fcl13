import stock
from stock import stock
import virtualMarket
from  virtualMarket import virtualMarket
import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt
import matplotlib.cm as cm


aMarket = virtualMarket(0,100) 
aMarket.AddStock(50)
aMarket.AddStock(10)
#aMarket.AddStockWithCap(5)
#aMarket.AddStockWithCap(10)

aMarket.Evolve()
ys = np.linspace(0,1,aMarket.m_nstocks)
colors = iter(cm.rainbow(np.linspace(0, 1, len(ys))))

plt.figure()

aMarket.listOllClosingValues()

for k in range(0,aMarket.m_nstocks):
    print "For Stock ",k,"Final Capital is ",aMarket.getStock(k).getCap()
    labels="stock(%d)"%k    
    plt.plot(aMarket.getStock(k).getTimes(),aMarket.getStock(k).getAll(),linewidth=0,color=next(colors),marker='o',markersize=2,label=labels)

plt.legend(loc='upper right',frameon=False)
plt.xlabel("time [a.u]")
plt.ylabel("value [a.u.]")
plt.show()
