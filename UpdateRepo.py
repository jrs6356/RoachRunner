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
scrCloneSrc = baseSrc + 'scripts/'
sigScrSrc = scrSrc + 'Signal_Conditioning/'
userSrc = scrSrc + 'User_Interface/'

#-------------Destination Addresses--------------
baseAddr = 'C:/Users/Research/OneDrive/Documents/GitHub/RoachRunner/Arduino/'
libAddr = baseAddr + 'libraries/'
cloneAddr = libAddr + 'full_clone/'
adcAddr = libAddr + 'RoachRunner_CS5532/'
digAddr = libAddr + 'RoachRunner_Digipot/'
lcdAddr = libAddr + 'RoachRunner_LCD/'
mathAddr = libAddr + 'RoachRunner_Math/'
sigLibAddr = libAddr + 'RoachRunner_Signal_Conditioner/'

scrAddr = baseAddr + 'scripts/'
scrCloneAddr = scrAddr + 'full_clone/'
sigScrAddr = scrAddr + 'Signal_Conditioning/'
userAddr = scrAddr + 'User_Interface/'

addrs = [cloneAddr,adcAddr,digAddr,lcdAddr,mathAddr,sigLibAddr,scrCloneAddr,sigScrAddr,userAddr]

#-------------Clear Old Files from GitHub--------------
for path in addrs:
    if os.path.exists(path):
        shutil.rmtree(path)

#-------------Copy Current Files to GitHub--------------
shutil.copytree(libSrc, cloneAddr)
shutil.copytree(adcSrc, adcAddr)
shutil.copytree(digSrc, digAddr)
shutil.copytree(lcdSrc, lcdAddr)
shutil.copytree(mathSrc, mathAddr)
shutil.copytree(sigLibSrc, sigLibAddr)

shutil.copytree(scrCloneSrc, scrCloneAddr)
shutil.copytree(sigScrSrc, sigScrAddr)
shutil.copytree(userSrc, userAddr)
