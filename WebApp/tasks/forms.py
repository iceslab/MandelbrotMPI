# -*- coding: UTF-8 -*-

from django import forms
from .models import Task


class AddTaskForm(forms.ModelForm):
    framerates = (
         ('24', '24'),
         ('30', '30'),
         ('60', '60'),
    )

    frameSizeX = forms.IntegerField(required=True, label="Szerokość obrazu [px]:")
    frameSizeY = forms.IntegerField(required=True, label="Wysokość obrazu [px]:")
    framerate = forms.ChoiceField(choices=framerates, required=True, label="Klatki na sekundę:")
    duration = forms.FloatField(required=True, label="Czas trwania [s]:")
    pathStartPointX = forms.FloatField(required=True, label="Punkt startowy X")
    pathStartPointY = forms.FloatField(required=True, label="Punkt startowy Y")
    pathEndPointX = forms.FloatField(required=True, label="Punkt końcowy X")
    pathEndPointY = forms.FloatField(required=True, label="Punkt końcowy Y")
    zoomStart = forms.FloatField(required=True, min_value=1, label="Powiększenie startowe")
    zoomEnd = forms.FloatField(required=True, min_value=1, label="Powiększenie końcowe")
    # colorStart = forms.FloatField()
    # colorEnd = forms.FloatField()

    class Meta:
        model = Task
        exclude = ['dotSize', 'progress', 'user', 'file_path', 'status', 'created_time', '', 'colorStart', 'colorEnd']