import json
from django.http import HttpResponse
from django.contrib.auth.decorators import login_required
from .models import Task

counter = 0

@login_required(login_url='/')
def test_js_func(request):
	global counter
	tasks = Task.objects.filter(user=request.user).order_by('-time')
	response_data={}
	
	for task in tasks:
		counter += 1
		response_data[task.id] = task.progress
	return HttpResponse(json.dumps(response_data),content_type="application/json")
