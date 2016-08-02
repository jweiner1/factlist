from __future__ import unicode_literals

from django.db import models

class Topic(models.Model):
    title = models.CharField(blank = False, max_length=128)