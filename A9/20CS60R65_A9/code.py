import ply.lex as lex
import ply.yacc as yacc
import urllib.request, urllib.error, urllib.parse
import sys
import re
import task1
import task2

#Global Variables
movie_name = ""
directors_name = []
writers_name = []
producers_name = []
original_language = ""
cast_character = []
cast_name = []
storyline = ""
genre = ""
box_office_collection = ""
runtime = ""
celeb_link = []
recommend_movie_link = []
recommend_movie_name = []
where_to_watch = []

#functions for lexical analyzer starts here
tokens = (
	'S_MOVIE',
	'E_MOVIE',
	'S_DIRECTOR',
	'S_WRITER1',
	'S_WRITER2',
	'E_WRITER',
	'S_PRODUCER',
	'S_LANGUAGE',
	'S_CAST_NAME',
	'E_CAST_NAME',
	'S_CAST_CHARACTER',
	'E_CAST_CHARACTER',
	'S_CAST_LINK',
	'S_STORYLINE',
	'E_STORYLINE',
	'S_BOX_OFFICE',
	'S_RUNTIME',
	'E_RUNTIME',
	'S_YMAL',
	'S_New_Movie_Link',
	'E_New_Movie_Link',
	'E_New_Movie_Name',
	'S_WTW',
	'E_WTW',
	'END_DIV',
	'ALL',
)

def t_S_MOVIE(t):
	r'<title>'
	return t

def t_E_MOVIE(t):
	r'<\/title>'
	return t


#---------------------------------Director, Writer and Producer-------------------------------------


def t_S_WRITER1(t):
	r'<div\sclass="meta-label\ssubtle"\sdata-qa="movie-info-item-label">Writer\:<\/div>[\s]+<div\sclass="meta-value"\sdata-qa="movie-info-item-value">[\s]+'
	return t

def t_S_WRITER2(t):
	r'<a\shref="/celebrity\/[\w\s\(\)\_\-\"\'\.\&\#:\;]+>'
	return t

def t_E_WRITER(t):
	r'</a>[,\s]*'
	return t

def t_S_DIRECTOR(t):
	r'<a\shref="\/celebrity\/[\w\s\_\-\"\'\.\&\#\;]+\sdata-qa="movie-info-director">'
	return t

def t_S_PRODUCER(t):
	r'<div\sclass="meta-label\ssubtle"\sdata-qa="movie-info-item-label">Producer:</div>[\s]+<div\sclass="meta-value"\sdata-qa="movie-info-item-value">[\s]+'
	return t


#-------------------------------for celebs, cast and crew-------------------------------


def  t_S_CAST_LINK(t):
	r'<a\shref="\s/celebrity/'
	return t

def t_S_CAST_NAME(t):
	r'"\sclass="unstyled\sarticleLink"\sdata-qa="cast-crew-item-link">[\s]+<span\stitle="[\w\s\_\-\'\"\.\&\#\;]+>[\s]+'
	return t

def t_E_CAST_NAME(t):
	r'<\/span>'
	return t

def t_S_CAST_CHARACTER(t):
	r'<span\sclass="characters\ssubtle\ssmaller"\stitle="[\w\s\-\.\'\.\&\#\;]+">[\s]+<br\/>[\s]+'
	return t

def t_E_CAST_CHARACTER(t):
	r'[<br\/>]*[\s]+</span>'
	return t


#---------------------------------------You might also like and where to watch-----------------------------


def t_S_YMAL(t):
	r'<h2\sclass="panel\-heading\srecommendations\-panel__heading\-title">You\smight\salso\slike<\/h2>[\s]+<div\sclass="recommendations\-panel\_\_content\-\-movie">[\s]+<tiles\-carousel\sclass=""\shidden>[\s]+<div\sclass="posters\-container"\sslot="posters\-container">'
	return t

def t_S_New_Movie_Link(t):
	r'[\s]*<a\shref="\/m\/'
	return t

def t_E_New_Movie_Link(t):
	r'\"\sclass\=\"recommendations\-panel\_\_poster\-link">([^*]+?)<span\sslot="title"\sclass="recommendations\-panel\_\_poster\-title">'
	return t

def t_E_New_Movie_Name(t):
	r'[\s]+</tile\-poster\-meta>[\s]+</tile\-poster>[\s]+<\/a>[\s]+'
	return t

def t_S_WTW(t):
	r'<affiliate-icon\sname="'
	return t

def t_E_WTW(t):
	r'"\salignicon="left\scenter"></affiliate-icon>'
	return t


#--------------------------------------others--------------------------------------------


def t_S_LANGUAGE(t):
	r'<div\sclass="meta-label\ssubtle"\sdata-qa="movie-info-item-label">Original\sLanguage:</div>[\s]+<div\sclass="meta-value"\sdata-qa="movie-info-item-value">'
	return t

def t_S_STORYLINE(t):
	r'<meta\sname="description"\scontent="'
	return t

def t_E_STORYLINE(t):
	r'[\s]*">'
	return t

def t_S_BOX_OFFICE(t):
	r'<div\sclass="meta-label\ssubtle"\sdata-qa="movie-info-item-label">Box\sOffice\s[\w()\s]+:<\/div>[\s]+<div\sclass="meta-value"\sdata-qa="movie-info-item-value">'
	return t

def t_S_RUNTIME(t):
	r'<div\sclass="meta-label\ssubtle"\sdata-qa="movie-info-item-label">Runtime:<\/div>[\s]+<div\sclass="meta-value"\sdata-qa="movie-info-item-value">[\s]+<time\sdatetime="[a-zA-Z0-9\s]+">[\s]+'
	return t

def t_E_RUNTIME(t):
	r'<\/time>'
	return t

def t_END_DIV(t):
	r'[\s]*</div>'
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
	'''start : movie
	| director
	| language
	| writers
	| producers
	| CAST_N
	| CAST_C
	| BOXOFFICE
	| RUNTIME
	| STORYLINE
	| YouMightAlsoLike
	| WhereToWatch'''
	pass

def p_movie(p):
	'movie : S_MOVIE ALL E_MOVIE'
	global movie_name
	movie_name = str(p[2][:-18])


#---------------------------------Director, Writer and Producer-------------------------------------


def p_director(p):
	'director : S_DIRECTOR ALL E_WRITER'
	global directors_name
	directors_name.append(str(p[2]))

def p_writers(p):
	"writers : S_WRITER1 WRITER2 END_DIV"

def p_writer(p):
	"""WRITER2 : S_WRITER2 ALL E_WRITER WRITER2
	| S_WRITER2 ALL E_WRITER"""
	global writers_name
	writers_name.append(p[2])

def p_producers(p):
	"producers : S_PRODUCER producer END_DIV"

def p_producer(p):
	"""producer : S_WRITER2 ALL E_WRITER producer
	| S_WRITER2 ALL E_WRITER"""
	global producers_name
	producers_name.append(p[2])


#-------------------------------------for celebs, cast and crew-----------------------------------


def p_cast_name(p):
	'CAST_N : S_CAST_LINK ALL S_CAST_NAME ALL E_CAST_NAME'
	global cast_name
	global celeb_link
	cast = str(p[4])
	cast1, cast2 = cast.split('\n')
	cast_name.append(cast1)
	celeb_link.append(p[2])

def p_cast_char(p):
	'CAST_C : S_CAST_CHARACTER ALL E_CAST_CHARACTER'
	global cast_character
	character = str(p[2])
	charc = character.split(',')
	pattern = re.compile(r'\s+')
	roles = []
	for i in range(len(charc)):
		role = re.sub(pattern, '', charc[i])
		roles.append(role)
	cast_character.append(roles)



#---------------------------------------You might also like and where to watch-----------------------------


def p_YMAL(p):
	'YouMightAlsoLike : S_YMAL New_Movie END_DIV'

def p_ymals(p):
	"""New_Movie : S_New_Movie_Link ALL E_New_Movie_Link ALL E_CAST_NAME E_New_Movie_Name New_Movie
	| S_New_Movie_Link ALL E_New_Movie_Link ALL E_CAST_NAME E_New_Movie_Name"""
	global recommend_movie_link
	global recommend_movie_name
	recommend_movie_link.append(p[2])
	recommend_movie_name.append(p[4])

def p_wtw(p):
	'WhereToWatch : S_WTW ALL E_WTW'
	global where_to_watch
	where_to_watch.append(p[2])


#-----------------------------------------others-------------------------------------------------


def p_language(p):
	'language : S_LANGUAGE ALL END_DIV'
	global original_language
	original_language = str(p[2])
	pattern = re.compile(r'\s+')
	original_language = re.sub(pattern, '', original_language)

def p_storyline(p):
	'STORYLINE : S_STORYLINE ALL E_STORYLINE'
	global storyline
	storyline = str(p[2]).strip()

def p_box_office(p):
	'BOXOFFICE : S_BOX_OFFICE ALL END_DIV'
	global box_office_collection
	box_office_collection = str(p[2])

def p_runtime(p):
	'RUNTIME : S_RUNTIME ALL E_RUNTIME'
	global runtime
	runtime = str(p[2]).strip()

def p_error(p):
	pass

#functions for parser ends here

def my_recurs(movie_html):

	global directors_name
	global writers_name
	global producers_name
	global cast_character
	global cast_name
	global celeb_link
	global recommend_movie_link
	global recommend_movie_name
	global where_to_watch

	directors_name = []
	writers_name = []
	producers_name = []
	cast_character = []
	cast_name = []
	celeb_link = []
	recommend_movie_link = []
	recommend_movie_name = []
	where_to_watch = []

	file = open('movie.html', 'r')
	data = file.read()
	lexer = lex.lex()
	parser = yacc.yacc()
	parser.parse(data)

	while(True):

		print('Enter your choice>>\n0----MovieName\n1----Director\n2----Writer\n3----Producer\n4----Original Language\n5----Cast with the character name\n6----Storyline\n7----Box office collection\n8----Runtime\n9----YOU MIGHT ALSO LIKE\n10----WHERE TO WATCH\n11----Exit')

		try:
			user = int(input())
		except Exception as e:
			print(f'Error!, Reason: {e}')

		if user == 0:
			print('-----------------------------------------------------------------------------------')
			print('Movie ---->> '+movie_name)
			print('-----------------------------------------------------------------------------------')

		elif user == 1:
			print('-----------------------------------------------------------------------------------')
			print('Director ---->>', end =" ")
			print(directors_name)
			print('-----------------------------------------------------------------------------------')

		elif user == 2:
			print('-----------------------------------------------------------------------------------')
			print('Writer ---->>', end =" ")
			print(writers_name)
			print('-----------------------------------------------------------------------------------')

		elif user == 3:
			print('-----------------------------------------------------------------------------------')
			print('Producer ---->>', end =" ")
			print(producers_name)
			print('-----------------------------------------------------------------------------------')

		elif user == 4:
			print('-----------------------------------------------------------------------------------')
			print('Original Language ---->> '+original_language)
			print('-----------------------------------------------------------------------------------')

		elif user == 5:
			print('-----------------------------------------------------------------------------------')
			print('Cast with character name ----')
			print('*****************************************************')
			print('Pick the number to select your celebrity:')
			print('*****************************************************')
			for i in range(len(cast_character)):
				print(i, '----', cast_name[i], cast_character[i])
			try:
				celeb_choice = int(input())
			except Exception as e:
				print(f'Error!, Reason: {e}')
			print('You selected: ' + str(cast_name[celeb_choice]))
			print('Wait... downloading html!!')
			url = 'https://www.rottentomatoes.com/celebrity/' + celeb_link[celeb_choice]
			#print(url)
			task2.celeb_data(url)

			print('-----------------------------------------------------------------------------------')

		elif user == 6:
			print('-----------------------------------------------------------------------------------')
			print('Storyline ---->> '+storyline)
			print('-----------------------------------------------------------------------------------')

		elif user == 7:
			print('-----------------------------------------------------------------------------------')
			print('Box office collection ---->> '+box_office_collection)
			print('-----------------------------------------------------------------------------------')

		elif user == 8:
			print('-----------------------------------------------------------------------------------')
			print('Runtime ---->> '+runtime)
			print('-----------------------------------------------------------------------------------')

		elif user == 9:
			print('-----------------------------------------------------------------------------------')
			print('YOU MIGHT ALSO LIKE ----->>')
			#print(recommend_movie_link)
			print('Enter your choice')
			for i in range(len(recommend_movie_name)):
				print(str(i)+'----'+recommend_movie_name[i])
			try:
				r_movie = int(input())
			except Exception as e:
				print(f'Error!, Reason: {e}')
			print('Wait downloading html!!')
			url = 'https://www.rottentomatoes.com/m/' + recommend_movie_link[r_movie]
			response = urllib.request.urlopen(url)
			webContent = response.read()
			f_recm = open('movie.html', 'wb')
			f_recm.write(webContent)
			f_recm.close
			my_recurs('movie.html')
			break

		elif user == 10:
			print('-----------------------------------------------------------------------------------')
			print('WHERE TO WATCH ---->>')
			for i in range(len(where_to_watch)):
				print(where_to_watch[i])
			print('-----------------------------------------------------------------------------------')

		elif user == 11:
			print('-----------------------------------------------------------------------------------')
			print("Exiting!")
			print('-----------------------------------------------------------------------------------')
			break
		else:
			print('-----------------------------------------------------------------------------------')
			print('Invalid input!')
			print('-----------------------------------------------------------------------------------')


my_recurs('movie.html')
print('-----------------------------------------------------------------------------------')
print('-----------------------------------------------------------------------------------')
print('-----------------------------------------------------------------------------------')
print('TASK2 DONE!')