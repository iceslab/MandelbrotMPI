from django.db import models
from django.contrib.auth.models import User
from django.utils import timezone


class Task(models.Model):
    user = models.ForeignKey(User, on_delete=models.CASCADE, )
    time = models.DateTimeField(default=timezone.now)
    cos = models.TextField('cos', default="", )
