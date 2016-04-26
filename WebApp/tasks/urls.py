from django.conf.urls import url, include
from . import views
from . import api

urlpatterns = [
    url(r'^$', views.index, name='index'),
    url(r'^generate', views.add, name='generate'),
    url(r'^fractals', views.fractals, name='fractals'),
    url(r'^animations', views.animations, name='animations'),
    url(r'^accounts/', include('registration.backends.hmac.urls')),
    url(r'^(?P<task_id>\d+)/del_task', views.delete, name='del_task'),
    url(r'^get_tasks_progress/',api.get_tasks_progress),
]
