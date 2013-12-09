import stock as st

class goodStock(st):
    """Derived class implementing the behviour of a good stock"""

    
    def evolveGood(self,change=0,timeLow=0,timeUp=1):
        """ Evolution method, increase value by change"""
        self.evolve(change,True,timeLow,timeUp)
        return self.m_cap
            
