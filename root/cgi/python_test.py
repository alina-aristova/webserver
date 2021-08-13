#!/usr/bin/python

import os
from posix import environ


for param in os.environ.keys():
	if (param == "COOKIE" and os.environ[param] == ""):
		os.environ[param] = "name" + os.environ["USER"]
		recognizer = ""
	elif (param == "COOKIE" and os.environ[param] != ""):
		recognizer = "Oh wait, I already know u!"

print ("Content-Type: text/html")
cookie_count = os.environ["COOKIE"].count('=')
cookies = os.environ["COOKIE"].split(';')
set_cookies = []
for i in range(cookie_count):
	set_cookies.append("Set-Cookie: " + cookies[i])
	if (i == cookie_count - 1):
		print(set_cookies[i], end= "\r\n\r\n")
	else: 
		print(set_cookies[i])

if recognizer == "":
	print("Your data has been stolen, hihihihi:)\n")
else:
	print(recognizer)
	print("Your data is: ")
	for i in range(cookie_count):
		print(cookies[i])
