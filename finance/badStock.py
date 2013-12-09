from stock import stock

class badStock(stock):
    """Derived class implementing the behviour of a bad stock"""

    def __init__(self,cap):
        stock.__init__(self,cap)

    def evolveBad(self,change=0,timeLow=0,timeUp=1):
        """ Evolution method, decrease value by change"""
        self.evolve(-change,True,timeLow,timeUp)
        return self.m_cap
            
