'''
Created on Nov 16, 2014

@author: jweiner
'''
import sys
from utils import lvl_string, log_apps, LOG_BASE_PORT

param = sys.argv[1]
if param == 'num_apps':
    print len(log_apps)
elif param == 'num_levels':
    print len(lvl_string)
elif param == 'apps':
    for app,lvl in log_apps:
        print app
elif param == 'levels':
    for lvl in lvl_string:
        print lvl
elif param == 'base_port':
    print LOG_BASE_PORT
    
