Django v1.9.2

http://makemeafractal.pl
http://149.202.43.116:8000

Konfiguracja Bazy w settings.py:
Adres ip servera: 149.202.43.116
Baza: mandel
Port: 3306
User: mandelbrot
Hasło: motznehaslo

Install:
1. python 2.7.3 inne też pewnie śmigają
2. pip

Komendy:
1. pip install django==1.9.2
2. pip install django-registration
3. python manage.py runserver (ip:port)


Konfiguracja apache'a:
<VirtualHost *:80>
	Alias /static /home/dave/mandelbrot/static
	<Directory /home/dave/mandelbrot/static>
		Require all granted
	</Directory>
	<Directory /home/dave/mandelbrot/mandelbrot>
		<Files wsgi.py>
			Require all granted
		</Files>
	</Directory>
	
	WSGIDaemonProcess mandelbrot python-path=/home/dave/mandelbrot:/home/dave/mandelbrot/lib/python2.7/site-packages
	WSGIProcessGroup mandelbrot
	WSGIScriptAlias / /home/dave/mandelbrot/mandelbrot/wsgi.py
</VirtualHost>
 