
from django.http import HttpResponse, HttpResponseRedirect
from django.shortcuts import render

def home(request):
    return HttpResponse("Hello, world. You're at /index.")
#     return render(request, 'home.html')