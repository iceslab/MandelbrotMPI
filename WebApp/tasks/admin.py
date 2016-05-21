from django.contrib import admin

from .models import Task


class TaskAdmin(admin.ModelAdmin):
    list_display = ('id', 'user', 'created_time')
    search_fields = ('user', 'created_time')
    list_filter = ('user',)
    ordering = ('created_time',)

admin.site.register(Task, TaskAdmin)
