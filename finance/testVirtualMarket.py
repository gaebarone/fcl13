#! /usr/bin/env python
import stock
from stock import stock
from goodStock import goodStock
from badStock import badStock

import virtualMarket
from  virtualMarket import virtualMarket

import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt
import matplotlib.cm as cm

import change_stock as cs
plt.ion()

#aVirtualMarket = virtualMarket(0,100) 
aRealMarket = virtualMarket(0,100)

stock1=stock(0)

i=0
for a in cs.open[0]:
    stock1.addHistoricaldata(a,i)
    i+=1

AgdSt=goodStock(stock1.getAll()[1])
AbdSt=badStock(stock1.getAll()[1])

aRealMarket.AddStockC(AgdSt)
aRealMarket.AddStockC(AbdSt)
aRealMarket.AddStockC(stock1)
aRealMarket.AddStock(cs.open[0][0])
aRealMarket.getStock(0).evolveGood(change=.10,timeLow=0,timeUp=(len(cs.open[0]))-1)
aRealMarket.getStock(1).evolveBad(change=.10,timeLow=0,timeUp=(len(cs.open[0]))-1)
aRealMarket.getStock(2).evolve(change=0,use=False,timeLow=0,timeUp=(len(cs.open[0]))-1)

ys = np.linspace(0,1,aRealMarket.m_nstocks)
colors = iter(cm.rainbow(np.linspace(0, 10*len(ys) , len(ys))))
colors = ["red", 'blue','green', 'yellow']

plt.figure()

aRealMarket.listOllClosingValues()

for k in range(0,aRealMarket.m_nstocks):
    print "For Stock ",k,"Final Capital is ",aRealMarket.getStock(k).getCap()
    labels="stock(%d)"%k    
    #plt.plot(aRealMarket.getStock(k).getTimes(),aRealMarket.getStock(k).getAll(),linewidth=0,color=next(colors),marker='o',markersize=2,label=labels)
    plt.plot(aRealMarket.getStock(k).getTimes(),aRealMarket.getStock(k).getAll(),linewidth=1,color=colors[k],marker='o',markersize=0,label=labels)

plt.legend(loc='upper right',frameon=False)
plt.xlabel("time [a.u]")
plt.ylabel("value [a.u.]")
plt.show()
