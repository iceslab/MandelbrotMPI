import json
from django.http import HttpResponse
from django.contrib.auth.decorators import login_required
from .models import Task


@login_required(login_url='/')
def get_tasks_progress(request):
	tasks = Task.objects.filter(user=request.user).order_by('-time')
	response_data={}
	
	for task in tasks:
		response_data[task.id] = [task.progress, task.status]
	return HttpResponse(json.dumps(response_data),content_type="application/json")
