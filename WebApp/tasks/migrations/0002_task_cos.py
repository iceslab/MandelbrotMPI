# -*- coding: utf-8 -*-
# Generated by Django 1.9.4 on 2016-03-25 00:19
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('tasks', '0001_initial'),
    ]

    operations = [
        migrations.AddField(
            model_name='task',
            name='cos',
            field=models.TextField(default=b'', verbose_name=b'cos'),
        ),
    ]