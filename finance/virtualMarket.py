import numpy as np
import market
from stock import stock 

class virtualMarket():
    ""virtualisation of multiples stocks""
    #All the memeber variables 
    m_nstocks = 0
    m_overAllCapital = 0
    m_overAllVariation = 0 
    m_startingCapStock =0 
    m_allStocks = np.ndarray((m_nstocks,),dtype=np.object)

    # Constructor 
    def __init__(self,nstocks,startingCapStock):
        m_nstocks=nstocks
        m_startingCapStock=startingCapStock
        m_allStocks = np.ndarray((nstocks,),dtype=np.object)
        for i in range(0,m_nstocks):
            m_allStocks[i] = stock(startingCapStock)
            m_overAllCapital[i] += startingCapStock
            
    def randomBet(low=0,up=100):
        for i in range(0,m_nstocks):
            m_allStocks[i].bet(np.random.rand(low,up)
        
                            
