'''
Created on Oct 5, 2014

@author: jweiner
'''
from base import SiteLibrary

def siteview(func):
    def inner(*args, **kwargs):
        return func(SiteLibrary(*args, **kwargs))
    return inner