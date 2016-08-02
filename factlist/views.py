
from django.http import HttpResponse, HttpResponseRedirect
from django.shortcuts import render

def home(request):
#     return HttpResponse("Hello, world. You're at /index.")
    from django import forms
    class NameForm(forms.Form):
        your_name = forms.CharField(label='Your name', max_length=100)

    template = "your_template.html"
    context = { "form" : NameForm() }
    return render(request, 'home.html', context)