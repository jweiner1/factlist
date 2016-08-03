from django.shortcuts import render
from django.http import HttpResponse, HttpResponseRedirect
from siteview.decorators import siteview

@siteview
def index(request):
    return HttpResponse("Hello, world. You're at topic app.")