import shutil
import os
import re

#---------------Source Addresses----------------
baseSrc = 'C:/Users/Research/OneDrive/Documents/Arduino/'
libSrc = baseSrc + 'libraries/'
adcSrc = libSrc + 'RoachRunner_CS5532/'
digSrc = libSrc + 'RoachRunner_Digipot/'
lcdSrc = libSrc + 'RoachRunner_LCD/'
mathSrc = libSrc + 'RoachRunner_Math/'
sigLibSrc = libSrc + 'RoachRunner_Signal_Conditioner/'
scrSrc = baseSrc + 'scripts/Masters/'
sigScrSrc = scrSrc + 'Signal_Conditioning/'
userSrc = scrSrc + 'User_Interface/'

#-------------Destination Addresses--------------
baseAddr = 'C:/Users/Research/OneDrive/Documents/GitHub/RoachRunner/Arduino/'
libAddr = baseAddr + 'libraries/'
adcAddr = libAddr + 'RoachRunner_CS5532/'
digAddr = libAddr + 'RoachRunner_Digipot/'
lcdAddr = libAddr + 'RoachRunner_LCD/'
mathAddr = libAddr + 'RoachRunner_Math/'
sigLibAddr = libAddr + 'RoachRunner_Signal_Conditioner/'
scrAddr = baseAddr + 'scripts/'
sigScrAddr = scrAddr + 'Signal_Conditioning/'
userAddr = scrAddr + 'User_Interface/'

#-------------Clear Old Files from GitHub--------------
shutil.rmtree(adcAddr)
shutil.rmtree(digAddr)
shutil.rmtree(lcdAddr)
shutil.rmtree(mathAddr)
shutil.rmtree(sigLibAddr)

shutil.rmtree(sigScrAddr)
shutil.rmtree(userAddr)

#-------------Copy Current Files to GitHub--------------
shutil.copytree(adcSrc, adcAddr)
shutil.copytree(digSrc, digAddr)
shutil.copytree(lcdSrc, lcdAddr)
shutil.copytree(mathSrc, mathAddr)
shutil.copytree(sigLibSrc, sigLibAddr)

shutil.copytree(sigScrSrc, sigScrAddr)
shutil.copytree(userSrc, userAddr)
