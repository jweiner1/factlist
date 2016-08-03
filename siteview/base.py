'''
Created on Oct 3, 2014

@author: jweiner
'''
from django.core.handlers.wsgi import WSGIRequest
# from account.utils import _getAccounts, get_user, getAccount
# from common.utils import getModelInstance
from django.http.response import HttpResponseRedirect
from django.core.urlresolvers import reverse
from logs.utils import *
# from crosshairs.settings import DEBUG

class SiteLibrary:
    
    def __init__(self, *args, **kwargs):

# "private" vars
        self._d = dict()
        self._request = False
        self._idNum = False
        self._model = False
        self._modelInstance = False
        self._account = False
        self._user = False
                
        for arg in args:
            if type(arg) == WSGIRequest:
                self._request = arg
                log('siteview', LVL_NOISE, 'Found request object! : %s' % str(self._request))
                log('siteview', LVL_NOISE, 'session: %s' % str(self._request.session))
        if not self._request:
            raise Exception('No WSGIRequest object passed to SiteLibrary')
         
        for k,v in kwargs.iteritems():
#             if k == 'model':
#                 self._model = v
            if k == 'origin':
                log('siteview', LVL_DEBUG, 'setting origin:%s' % v)
                self.setOrigin(v)
#             if k == 'idNum':
#                 self._idNum = v
#         
#         self._user = get_user(self._request)
#         log('siteview', LVL_DEBUG, 'self._user:%s' % str(self._user))
#         log('siteview', LVL_DEBUG, 'Acount type:%s' % type(self._user))
# 
#         self._account = getAccount(self._user)
#         if not self._account:
#             raise Exception('No valid Account located in SiteLibrary')
#         self._request.session['accountNumber'] = self._account.accountNumber
# 
#         if self._idNum:
#             if not self._model:
#                 raise Exception('No model set in SiteLibrary, make sure one is declared in the urls.py file')
#             self._modelInstance = getModelInstance(self._account, self._model, self._idNum)
#             if not self._modelInstance:
#                 raise Exception('Not authorized to view this item')
            
# "public" vars     
        self.modelInstance = self._modelInstance
        self.request = self._request
        self.user = self._user
        self.account = self._account
        self.origin = self._getOrigin()
        self.idNum = self._idNum
        
# default dict entries
#         self.updateDict({'tags':self.getAccountProperties()})        
            
# generic functions
    def dict(self):
        return self._d
    
    def _getOrigin(self):
        if 'origin' in self._request.session:
            return self._request.session['origin']
        else:
            return 'login'
    
# sets
    def updateDict(self, kv):
        self._d.update(kv)
        
    def setOrigin(self, origin):
        self.origin = origin
        self._request.session['origin'] = origin
        self._d.update({'origin' : origin})
        
    def setMessage(self, msg):
        self._request.session['message'] = msg
        self._d.update({'message' : msg})
    
# render override
#     def render(self, template_name, *args, **kwargs):
#         from django.shortcuts import render
#         from django.utils.safestring import mark_safe
#         if self.account.status != 'A':
#             if self.account.status == 'N':
#                 log('siteview', LVL_DEBUG, 'Account %d not yet authenticated' % self.account.accountNumber)
#                 return HttpResponseRedirect(reverse('account:authorization', kwargs={'acctId':self.account.id}))
# 
#         if 'message' in self._request.session:
#             self._d.update({'message' : mark_safe(self._request.session['message'])})
#             del self._request.session['message']
#         if 'origin' in self._request.session:
#             self._d.update({'origin' : self._getOrigin()})
#         # disable analytics if in DEBUG mode
#         if DEBUG:
#             self._d.update({'debug' : True})
#         return render(self._request, template_name, self._d)
    
# return to origin
    def returnToOrigin(self):
        if self.getOrigin():
            return HttpResponseRedirect(reverse(self.getOrigin() + ':index'))
        else:
            return HttpResponseRedirect(reverse('login'))
        
    def test(self):
        l = {'jon', 'mary', 343, 1121, "albert"}
        self._request.session['testList'] = l
        for i in self._request.session['testList']:
            print ' entry:' + str(i)
        
# site specific functions
    
    def get_ebay_searches(self):
        return self._account.ebay_search.all().order_by('label')
#     
#     def getAccountAddresses(self):
#         return self._account.address.all()
#     
#     def getAccountUnits(self):
#         return self._account.unit.all()
#     
#     def getAccountTenants(self):
#         return self._account.tenant.all()
#     
#     def getAccountApplications(self):
#         return self.account.application.all()
#     
#     def getAccountLeases(self):
#         return self.account.lease.all()
            