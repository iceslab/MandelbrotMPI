# -*- coding: utf-8 -*-
# Generated by Django 1.9.4 on 2016-04-27 23:05
from __future__ import unicode_literals

from django.conf import settings
from django.db import migrations, models
import django.db.models.deletion
import django.utils.timezone


class Migration(migrations.Migration):

    initial = True

    dependencies = [
        migrations.swappable_dependency(settings.AUTH_USER_MODEL),
    ]

    operations = [
        migrations.CreateModel(
            name='Task',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('created_time', models.DateTimeField(default=django.utils.timezone.now)),
                ('file_path', models.TextField(default=b'')),
                ('status', models.TextField(default=b'pending')),
                ('progress', models.FloatField(default=b'0')),
                ('duration', models.FloatField(default=b'0')),
                ('framerate', models.PositiveIntegerField(default=b'0')),
                ('frameSizeX', models.PositiveIntegerField(default=b'0')),
                ('frameSizeY', models.PositiveIntegerField(default=b'0')),
                ('dotSize', models.FloatField(default=b'0')),
                ('pathStartPointX', models.FloatField(default=b'0')),
                ('pathStartPointY', models.FloatField(default=b'0')),
                ('pathEndPointX', models.FloatField(default=b'0')),
                ('pathEndPointY', models.FloatField(default=b'0')),
                ('zoomStart', models.FloatField(default=b'0')),
                ('zoomEnd', models.FloatField(default=b'0')),
                ('colorStart', models.FloatField(default=b'0')),
                ('colorEnd', models.FloatField(default=b'0')),
                ('user', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to=settings.AUTH_USER_MODEL)),
            ],
        ),
    ]
