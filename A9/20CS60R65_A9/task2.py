import ply.lex as lex 
import ply.yacc as yacc
import re
import os
import urllib.request, urllib.error, urllib.parse

#Global variables
h_rated = ""
l_rated = ""
bday = ""
movies = []
year = []
#functions for lexical analyzer starts here
tokens = (
	'S_h_rated',
	'S_l_rated',
	'S_bd',
	'E_bd',
	'S_MOVIES',
	'S_YEAR',
	'S_S',
	'E_S',
	'E_YEAR',
	'END',
	'ALL',
	)


#---------------------------------------celeb data-------------------------------------------


def t_S_h_rated(t):
	r'<p\sclass="celebrity-bio__item"\sdata-qa="celebrity-bio-highest-rated">[\s]+Highest\sRated:[\s]+<span\sclass="label">[\s]+<span[\s]+class="icon\sicon--tiny\sicon__(certified-)*fresh"[\s]+title="(certified-)*fresh"></span>[\s]+[0-9%]+[\s]+<a\sclass="celebrity-bio__link"\shref="[\w/\^\*\(\)\;\'\.\+\-\\,:\s\d!@#$%&*]+">[\s]+'
	return t

def t_S_l_rated(t):
	r'<p\sclass="celebrity-bio__item"\sdata-qa="celebrity-bio-lowest-rated">[\s]+Lowest\sRated:[\s]+<span\sclass="label">[\s]+<span[\s]+class="icon\sicon--tiny\sicon__(certified-)*(fresh)*(rotten)*"[\s]+title="(certfied-)*(fresh)*(rotten)*"></span>[\s]+[0-9%]+[\s]+<a\sclass="celebrity-bio__link"\shref="[\w/\^\*\(\)\;\'\.\+\-\\,:\s\d!@#$%&*]+">+[\s]+'
	return t

def t_S_bd(t):
	r'<p\sclass="celebrity-bio__item"\sdata-qa="celebrity-bio-bday">[\s]+Birthday:'
	return t

def t_E_bd(t):
	r'</p>'
	return t


#---------------------------------------celeb all movies--------------------------------------

def t_S_S(t):
	r'<span>'
	return t

def t_E_S(t):
	r'</span>'
	return t

def t_S_YEAR(t):
	r'<td\sclass="celebrity-filmography__year">'
	return t

def t_S_MOVIES(t):
	r'<a\shref="/m/[\s=!&$;+%)(a-zA-Z0-9_. @#/:?,\'-]+">'
	return t

def t_E_YEAR(t):
	r'</td>'
	return t

def t_END(t):
	r'</a>'
	return t

def t_ALL(t):
	r'[\w\^\*\(\)\.\+\-\\,:\s!@#$%&*]+'
	return t

def t_error(t):
	#print(f'Illegal character {t.value[0]!r}')
	t.lexer.skip(1)

#functions for lexical analyzer ends here

#fucntions for parser starts here

def p_start(t):
	'''start : Bithday
	| High_Rated
	| Low_Rated
	| Movie
	| Year'''
	pass

def p_hrated(p):
	'High_Rated : S_h_rated ALL END'
	global h_rated
	h_rated = p[2]

def p_lrated(p):
	'Low_Rated : S_l_rated ALL END'
	global l_rated
	l_rated = p[2]

def p_bday(p):
	'Bithday : S_bd ALL E_bd'
	global bday
	bday = p[2].strip()

def p_movies(p):
	"""Movie : S_MOVIES ALL END
	| S_S ALL E_S"""
	global movies
	movies.append(p[2].strip())

def p_year(p):
	'Year : S_YEAR ALL E_YEAR'
	global year
	year.append(p[2].strip())

def p_error(p):
	pass

#functions for parser ends here


def celeb_data(url):

	#print(url)
	global movies
	global year

	movies = []
	year = []

	try:
		os.remove("celeb.html")
	except:
		pass

	response = urllib.request.urlopen(url)
	webContent = response.read()
	f_celeb = open('celeb.html', 'wb')
	f_celeb.write(webContent)
	f_celeb.close

	file = open('celeb.html', 'r')
	data = file.read()
	lexer = lex.lex()
	parser = yacc.yacc()
	parser.parse(data)

	#removing redundant value
	movies.pop(0)

	while(True):

		print('\nEnter your choice>>\n0----Highest Rated Movie\n1----Lowest Rated Movie\n2----Birthday\n3----Other Movies\n4----Exit to previous menu')

		try:
			user = int(input())
		except Exception as e:
			print(f'Error!, Reason: {e}')

		if user == 0:
			print('-----------------------------------------------------------------------------------')
			print('Highest Rated Movie ---->> '+h_rated)
			print('-----------------------------------------------------------------------------------')

		elif user == 1:
			print('-----------------------------------------------------------------------------------')
			print('Lowest Rated Movie ---->> '+l_rated)
			print('-----------------------------------------------------------------------------------')

		elif user == 2:
			print('-----------------------------------------------------------------------------------')
			print('Bithday ---->> '+bday)
			print('-----------------------------------------------------------------------------------')

		elif user == 3:
			while("" in year) :
   				year.remove("")
			print('-----------------------------------------------------------------------------------')
			print('Enter the year till which you want to enquire movies')
			print('Year should be between 2021 to ', int(year[len(year)-1]))
			#print(year)
			try:
				movie_year = int(input())
			except Exception as e:
				print(f'Error!, Reason: {e}')

			if movie_year >= int(year[len(year)-1]) and movie_year <= 2021:
				i = 0
				for m in movies:
					if movie_year < int(year[i]):
						print(i+1, '----', m, '(', year[i], ')')
						i += 1
					else:
						break
			else:
				print('Invalid Year')

		elif user == 4:
			print('*****************************************************************')
			break

		else:
			print('-----------------------------------------------------------------------------------')
			print('Invalid input!')
			print('-----------------------------------------------------------------------------------')
