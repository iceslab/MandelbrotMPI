from django.contrib import admin

from .models import Task


class TaskAdmin(admin.ModelAdmin):
    list_display = ('id', 'user', 'time')
    search_fields = ('user', 'time')
    list_filter = ('user',)
    ordering = ('time',)

admin.site.register(Task, TaskAdmin)
