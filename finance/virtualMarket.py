"""codeauthor:: gaetano gaetano.barone@cern.ch"""
import numpy as np
import stock
from stock import stock 

class virtualMarket():
    """Container Class of stocks"""
    m_nstocks = 0
    m_overAllCapital = 0
    m_overAllVariation = 0 
    m_startingCapStock = 0. 
    m_time = 100 
    m_allStocks=[]
    
    def __init__(self,nstocks,startingCapStock):
        """Container constructor
        """
        self.m_nstocks=nstocks
        self.m_startingCapStock=startingCapStock
        self.m_allStocks=[]
        #self.m_allStocks=np.ndarray((nstocks,),dtype=np.object)
        for i in range(0,self.m_nstocks):
            self.m_allStocks.append(stock(startingCapStock))
            self.m_overAllCapital+=startingCapStock
        print "Constructed Virtual Market with ",self.m_nstocks," stocks " 
   
    def AddStock(self,st):
        """Apend a stock to the container
        :param st: stock to be added
        """
        self.m_nstocks += 1
        self.m_allStocks.append(st);   
        self.m_overAllCapital  += st.getCap()
        print "Added Stock ", self.m_allStocks[self.m_nstocks]," ",self.m_allStocks.getCap()

    def AddStock(self,cap):
        """Apend a stock with capital
        :param cap: intial capital
        """
        self.m_nstocks += 1
        self.m_allStocks.append(stock(cap))
        self.m_overAllCapital  += cap
      

    def randomBet(low=0,up=100):
        """Place a random order with in range low up on the stock
        :param low: lower limit
        :param up: upper limit"""
        for i in range(0,self.m_nstocks):
            self.m_allStocks[i].bet(np.random.rand(low,up))
            
    def setTime(time=100):
        """Set time
        :param time: time to be set"""
        self.m_time=time 

    def getStock(self,s): 
        """stock at position
        :param s: stock index
        :return: stock of index s"""
        return self.m_allStocks[s]
        
    def listOllClosingValues(self):
        """prints the list of final values of all stocks
        :returns: a print out"""
        for i in range (0,self.m_nstocks):
            print self.m_allStocks[i].getCap()

    def Evolve(self):
        """Calls the evolve method for each stock"""
        for t in range(0,self.m_time):
            for i in range (0,self.m_nstocks):
                #print "Evovling stock ",i
                self.m_overAllCapital += self.m_allStocks[i].evolve()
                
