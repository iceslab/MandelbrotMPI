from django.shortcuts import redirect, render
from django.contrib.auth.decorators import login_required
from .forms import AddTaskForm
from .models import Task


def index(request):
    if request.user.is_authenticated():
        tasks = Task.objects.filter(user=request.user).order_by('-time')
        return render(request, 'tasks/tasks.html', {'tasks': tasks})
    else:
        return render(request, 'tasks/tasks.html', {})


@login_required(login_url='/')
def add(request):
    if request.method == "POST":
        form = AddTaskForm(request.POST)
        if form.is_valid():
            post = form.save(commit=False)
            post.user = request.user
            post.save()
            return redirect('/', pk=post.pk)
    else:
        form = AddTaskForm()
    return render(request, 'tasks/addTask.html', {'form': form})


@login_required(login_url='/')
def delete(request, task_id):
    Task.objects.filter(user_id=request.user.id, id=task_id).delete()
    return redirect('/')


@login_required(login_url='/')
def fractals(request):
    return render(request, 'tasks/fractals.html', {})


@login_required(login_url='/')
def animations(request):
    return render(request, 'tasks/animations.html', {})
