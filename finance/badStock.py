import stock as st

class goodBad(st):
    """Derived class implementing the behviour of a bad stock"""

    def evolveBad(self,change=0,timeLow=0,timeUp=1):
        """ Evolution method, decrease value by change"""
        self.evolve(-change,True,timeLow,timeUp)
        return self.m_cap
            
