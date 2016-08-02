#!/usr/bin/env python
import os
import sys

def usage_topic():
    print " %s topic add <topic title> - Add topic title" % sys.argv[0]

def usage(val=False):
    #rom account.manage_account import usage_account
    print "\n=============Factlist Utils=============\n"
    print 'Usage:\n'
    
    all = False
    if not val:
        all = True
        
    if all or val == 'topic':
        usage_topic()

    
    print "\n==========================================\n"
    if all:
        raw_input('Press enter key to see native commands....')
        print "\n"
        
if __name__ == "__main__":
    os.environ.setdefault("DJANGO_SETTINGS_MODULE", "factlist.settings")

    from django.core.management import execute_from_command_line
    
    #argc = len(sys.argv)

    #if argc == 1:
    #    usage()

    execute_from_command_line(sys.argv)
