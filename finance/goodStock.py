from stock import stock

class goodStock(stock):
    """Derived class implementing the behviour of a good stock"""
    
    def __init__(self,cap):
        stock.__init__(self,cap)
    
    def evolveGood(self,change=0,timeLow=0,timeUp=1):
        """ Evolution method, increase value by change"""
        self.evolve(change,True,timeLow,timeUp)
        return self.m_cap
