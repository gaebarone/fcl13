import numpy as np 
import market 

class stock():
    """class for decribing stock evolution"""
    m_cap = 0
    m_time =0 
    m_val =[]
    m_time_his =[]
    m_iters = 0 

    def __init__(self,cap):
        self.m_cap=cap
        self.m_time=0
        self.m_val.append(cap)
        self.m_time_his.append(0)
        print "new stock with start value of ", self.m_cap 

    def bet(betVal):
        self.m_cap = self.m_cap+betVal
    
    def getCap(self):
        return self.m_cap
    
    def getTime(self):
        return self.m_time
    
    def next(self):
        val=self.evolve()
        return val 

    def getAll(self):
        return self.m_val
    
    def getTimes(self):
        return self.m_time_his 

    def addHistoricaldata(self,currentVal=0,time=0):
        self.m_val.append(currentVal)
        self.m_cap=currentVal
        self.m_iters+=1
        self.m_time=time
        self.m_time_his.append(time)
        return self.getCap()

    def evolve(self,change=0,use=False,timeLow=0,timeUp=1):
        for t in range(timeLow,timeUp):
            if use:
                self.m_cap = self.m_cap*change 
                #self.m_cap = self.m_cap*change + self.m_cap
            else:
                self.m_cap += np.random.normal(0,.1,1)*self.m_cap  
            self.m_iters+=1
            self.m_time+=1
            self.m_val.append(float(self.m_cap))
            self.m_time_his.append(self.m_time)

        return self.m_cap


