print __doc__

# Author: Gael Varoquaux gael.varoquaux@normalesup.org
# License: BSD

import datetime
import ROOT
import numpy as np
import pylab as pl
import newfinance as finance
#from matplotlib import finance
from matplotlib.collections import LineCollection

from sklearn import cluster, covariance, manifold

###############################################################################
# Retrieve the data from Internet

# Choose a time period reasonnably calm (not too long ago so that we get
# high-tech firms, and before the 2008 crash)
d1 = datetime.datetime(2009, 3, 1) #yyyy-mm-dd
d2 = datetime.datetime(2013, 4, 8)


symbol_dict = {
    'LT.NS': 'Larsen & Toubro Limited',
    'LAXMIMACH.NS': 'Lakshmi Machine Works Ltd.',
    'PLETHICO.BO': 'Plethico Pharmaceuticals Ltd.'
}

symbols, names = np.array(symbol_dict.items()).T

quotes = [finance.quotes_historical_yahoo(symbol, d1, d2, asobject=True)
          for symbol in symbols]

open = np.array([q.open for q in quotes])
close = np.array([q.close for q in quotes])
date = np.array([q.date for q in quotes])


# The daily variations of the quotes are what carry most information
variation = close - open

histograms = []
for loop in range(0, len(quotes)):
    hist = ROOT.TH1F(symbols[loop], symbols[loop], len(open[loop]-1), 0, len(open[loop]))
    for fill in range(0, len(open[loop])):
        hist.SetBinContent(fill+1, open[loop][fill])
        if fill % 20 == 0 and fill != 0:
            hist.GetXaxis().SetBinLabel(fill, str(date[loop][fill]))
    histograms.append(hist)
