from django import forms
from .models import Task


class AddTaskForm(forms.ModelForm):
    framerates = (
         ('24', '24'),
         ('30', '30'),
         ('60', '60'),
    )

    frameSizeX = forms.IntegerField(required=True, label="Szerokosc")
    frameSizeY = forms.IntegerField(required=True, label="Dlugosc")
    framerate = forms.ChoiceField(choices=framerates, required=True, label="Liczba klatek/sekunde")
    duration = forms.FloatField(required=True, label="Czas Trwania", help_text="(s)")
    pathStartPointX = forms.FloatField(required=True, label="Punkt Startowy X")
    pathStartPointY = forms.FloatField(required=True, label="Punkt Startowy Y")
    pathEndPointX = forms.FloatField(required=True, label="Punkt Koncowy X")
    pathEndPointY = forms.FloatField(required=True, label="Punkt Koncowy Y")
    zoomStart = forms.FloatField(required=True, min_value=1, label="Powiekszenie startowe")
    zoomEnd = forms.FloatField(required=True, min_value=1, label="Powiekszenie koncowe")
    # colorStart = forms.FloatField()
    # colorEnd = forms.FloatField()

    class Meta:
        model = Task
        exclude = ['dotSize', 'progress', 'user', 'file_path', 'status', 'created_time', '', 'colorStart', 'colorEnd']