'''
Created on Nov 16, 2014

@author: jweiner
'''

import socket
from inspect import currentframe, getframeinfo
import ntpath
import time

LOG_BASE_PORT = 5000
LVL_FATAL = 0
LVL_CRIT = 1
LVL_ERROR = 2
LVL_WARN = 3
LVL_INFO = 4
LVL_DEBUG = 5
LVL_NOISE = 6

TRC_CRAIGSLIST = 'craigslist'
TRC_SCRAPE = 'scrape'
TRC_ACCOUNT = 'account'

lvl_string = ['FATAL',
              'CRITICAL',
              'ERROR',
              'WARN',
              'INFO',
              'DEBUG',
              'NOISE']

log_apps = [#( app_name, force_xmit_lvl)
            ('topic',        LVL_WARN),
            ('siteview',    LVL_WARN),
            ('common',      LVL_WARN),
            ('general',     LVL_WARN),
            ]

def get_log_port(app):
    port = LOG_BASE_PORT
    
    for (a,l) in log_apps:
        if app == a:
            break
        port += 1
    
    return port
    
def log(app, level, msg):
    from factlist.settings import LOCAL_LOGS, LOG_REMOTE_IP

#     if app == 'ebay':
#         return
#     if level == LVL_NOISE:
#         return
#     if level == LVL_DEBUG:
#         return
    if LOCAL_LOGS:
        log_ip = '127.0.0.1'
    else:
        log_ip = LOG_REMOTE_IP
        
    port = get_log_port(app)
    cf = currentframe()
    filename = ntpath.basename(getframeinfo(cf.f_back).filename)
    line_no = cf.f_back.f_lineno
    current_time = time.strftime("%X %x")
    sock = socket.socket(socket.AF_INET, # Internet
                         socket.SOCK_DGRAM) # UDP
    msg = '%s:%s:%s:%s(%d)::%s' % (current_time, app, lvl_string[level], filename, line_no, msg)
    sock.sendto(msg.encode('utf-8'), (log_ip, port))
    if level == LVL_ERROR:
        print msg