import numpy as np 
import market 

class stock():
    """class for decribing stock evolution"""
    m_cap = 0
    m_time = 0 
    def __init__(self,cap):
        stock.m_cap=cap
        stock.m_time=0
        print "new stock with start value of ", stock.m_cap 

    def bet(betVal):
        stock.m_cap = stock.m_cap+betVal
    
    def getCap(self):
        return stock.m_cap
    
    def getTime(self):
        return stock.m_time
    
    def next(self):
        val=self.evolve()
        return val 

    def evolve(self,change=0,use=False):
        if use:
            stock.m_cap = stock.m_cap*change + stock.m_cap
        else:
            stock.m_cap += np.random.normal(0,.1,1)*stock.m_cap  
        
        stock.m_time+=1
        return stock.m_cap


