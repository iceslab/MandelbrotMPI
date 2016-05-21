from django.db import models
from django.contrib.auth.models import User
from django.utils import timezone


class Task(models.Model):
    user = models.ForeignKey(User, on_delete=models.CASCADE, )
    created_time = models.DateTimeField(default=timezone.now, )
    file_path = models.TextField(default="", )
    status = models.TextField(default="pending", )
    progress = models.FloatField(default="0", )
    duration = models.FloatField(default="0",)
    framerate = models.PositiveIntegerField(default="0")
    frameSizeX = models.PositiveIntegerField(default="0")
    frameSizeY = models.PositiveIntegerField(default="0")
    dotSize = models.FloatField(default="0")
    pathStartPointX = models.FloatField(default="0")
    pathStartPointY = models.FloatField(default="0")
    pathEndPointX = models.FloatField(default="0")
    pathEndPointY = models.FloatField(default="0")
    zoomStart = models.FloatField(default="0")
    zoomEnd = models.FloatField(default="0")
    colorStart = models.FloatField(default="0")
    colorEnd = models.FloatField(default="0")


def get_dotsize(self):
    return 4.0 / self.frameSizeX
