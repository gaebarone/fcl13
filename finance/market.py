class market:
    """Base for decribing stock evolution"""
    m_cap = 0
    m_time = 0 
    def __init__(self,cap):
        market.m_cap=cap
        market.m_time=0
        print "new stock with start value of ", stock.m_cap 
        

