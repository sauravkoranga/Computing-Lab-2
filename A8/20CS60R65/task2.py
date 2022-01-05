import ply.lex as lex
import ply.yacc as yacc
import re
import task1

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

#functions for lexical analyzer starts here
tokens = (
	'S_MOVIE',
	'E_MOVIE',
	'S_GENRE',
	'S_DIRECTOR',
	'S_WRITER1',
	'S_WRITER2',
	'S_PRODUCER1',
	'S_PRODUCER2',
	'S_LANGUAGE',
	'S_CAST_NAME',
	'E_CAST_NAME',
	'S_CAST_CHARACTER',
	'E_CAST_CHARACTER',
	'S_STORYLINE',
	'S_BOX_OFFICE',
	'S_RUNTIME',
	'E_RUNTIME',
	'END_DIV',
	'END_A',
	'END_LI',
	'STAR',
	'ALL',
)

def t_S_MOVIE(t):
	r'<title>'
	return t

def t_E_MOVIE(t):
	r'<\/title>'
	return t

def t_S_GENRE(t):
	r'Genre:</div>[\s]+<div\sclass="meta-value\sgenre"\sdata-qa="movie-info-item-value">'
	return t

def t_S_DIRECTOR(t):
	r'<a\shref="\/celebrity\/[\w\s\_\-\'\.\&\#\;]+_[\w\s\_\-\'\.\&\#\;]+"\sdata-qa="movie-info-director">'
	return t

def t_END_A(t):
	r'[\s]*</a>'
	return t

def t_S_WRITER1(t):
	r'<div\sclass="meta-label\ssubtle"\sdata-qa="movie-info-item-label">Writer\:<\/div>[\s]+<div\sclass="meta-value"\sdata-qa="movie-info-item-value">[\s]+'
	return t

def t_S_WRITER2(t):
	r'<a\shref="\/celebrity\/[\w\s\_\-\'\.\&\#\;]+_[\w\s\_\-\'\.\&\#\;]+">'
	return t

def t_END_DIV(t):
	r'<\/div>'
	return t

def t_STAR(t):
	r'[,\s]+'
	return t

def t_S_LANGUAGE(t):
	r'<div\sclass="meta-label\ssubtle"\sdata-qa="movie-info-item-label">Original\sLanguage:</div>[\s]+<div\sclass="meta-value"\sdata-qa="movie-info-item-value">'
	return t

def t_S_PRODUCER1(t):
	r'<div\sclass="meta-label\ssubtle"\sdata-qa="movie-info-item-label">Producer:</div>[\s]+<div\sclass="meta-value"\sdata-qa="movie-info-item-value">[\s]+'
	return t

def t_S_PRODUCER2(t):
	r'<a\shref="\/celebrity\/[\w\s\_\-\'\.\&\#\;]+">'
	return t

def t_S_CAST_NAME(t):
	r'<a\shref="\s/celebrity/[\w\s\_\-\'\.\&\#\;]+"\sclass="unstyled\sarticleLink"\sdata-qa="cast-crew-item-link">[\s]+<span\stitle="[\w\s\_\-\'\.\&\#\;]+">[\s]+'
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

def t_S_STORYLINE(t):
	r'<div\sid="movieSynopsis"\sclass="movie_synopsis\sclamp\sclamp-6\sjs-clamp"\sstyle="clear:both"\sdata-qa="movie-info-synopsis">[\s]+'
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

def t_END_LI(t):
	r'</div>[\s]+</li>'
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
	| genre
	| director
	| language
	| writers
	| producers
	| CAST_N
	| CAST_C
	| BOXOFFICE
	| RUNTIME
	| STORYLINE'''
	pass

def p_movie(p):
	'movie : S_MOVIE ALL E_MOVIE'
	global movie_name
	movie_name = str(p[2][:-18])

def p_genre(p):
	'genre : S_GENRE ALL END_LI'
	global genre
	print(1)
	pattern = re.compile(r'\s+')
	genre = str(p[2])
	genre = re.sub(pattern, '', genre)

def p_director(p):
	'director : S_DIRECTOR ALL END_A'
	global directors_name
	directors_name.append(str(p[2]))

def p_language(p):
	'language : S_LANGUAGE ALL END_DIV'
	global original_language
	original_language = str(p[2])
	pattern = re.compile(r'\s+')
	original_language = re.sub(pattern, '', original_language)

def p_writers(p):
	"writers : S_WRITER1 WRITER2 END_DIV"

def p_writer(p):
	"""WRITER2 : S_WRITER2 ALL END_A STAR WRITER2
	| S_WRITER2 ALL END_A STAR"""
	global writers_name
	writers_name.append(p[2])

def p_producers(p):
	"producers : S_PRODUCER1 producer END_DIV"

def p_producer(p):
	"""producer : S_PRODUCER2 ALL END_A STAR producer
	| S_PRODUCER2 ALL END_A STAR"""
	global producers_name
	producers_name.append(p[2])

def p_storyline(p):
	'STORYLINE : S_STORYLINE ALL END_DIV'
	global storyline
	storyline = str(p[2])
	storyline = re.sub('\n', '', storyline)
	storyline = re.sub('\t', '', storyline)

def p_cast_name(p):
	'CAST_N : S_CAST_NAME ALL E_CAST_NAME'
	global cast_name
	cast = str(p[2])
	cast1, cast2 = cast.split('\n')
	cast_name.append(cast1)

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

def p_box_office(p):
	'BOXOFFICE : S_BOX_OFFICE ALL END_DIV'
	global box_office_collection
	box_office_collection = str(p[2])

def p_runtime(p):
	'RUNTIME : S_RUNTIME ALL E_RUNTIME'
	global runtime
	runtime = str(p[2])
	runtime = re.sub('\n', '', runtime)
	runtime = re.sub('\t', '', runtime)

def p_error(p):
	pass

#functions for parser ends here

file = open('movie.html', 'r')
data = file.read()
lexer = lex.lex()
parser = yacc.yacc()
parser.parse(data)

write_file = open('log.txt', 'a')

genre = task1.genre

while(True):
	print('Enter your choice>>\n0----MovieName\n1----Director\n2----Writer\n3----Producer\n4----Original Language\n5----Cast with the character name\n6----Storyline\n7----Box office collection\n8----Runtime\n9----Exit')

	try:
		user = int(input())
	except Exception as e:
		print(f'Error!, Reason: {e}')

	if user == 0:
		print('Movie ---->> '+movie_name)
		write_file.write(f'<{genre}><{movie_name}><Movie><{movie_name}>\n')
		write_file.flush()
	elif user == 1:
		print('Director ---->>', end =" ")
		print(directors_name)
		for d in directors_name:
			write_file.write(f'<{genre}><{movie_name}><Director><{d}>\n')
			write_file.flush()
	elif user == 2:
		print('Writer ---->>', end =" ")
		print(writers_name)
		for w in writers_name:
			write_file.write(f'<{genre}><{movie_name}><Writer><{w}>\n')
			write_file.flush()
	elif user == 3:
		print('Producer ---->>', end =" ")
		print(producers_name)
		for p in producers_name:
			write_file.write(f'<{genre}><{movie_name}><Producer><{p}>\n')
			write_file.flush()
	elif user == 4:
		print('Original Language ---->> '+original_language)
		write_file.write(f'<{genre}><{movie_name}><Original Language><{original_language}>\n')
		write_file.flush()
	elif user == 5:
		print('Cast with character name ----')
		for i in range(len(cast_character)):
			print(cast_name[i], cast_character[i])
			write_file.write(f'<{genre}><{movie_name}><Cast with character name><{cast_name[i],cast_character[i]}>\n')
			write_file.flush()
	elif user == 6:
		print('Storyline ---->> '+storyline)
		write_file.write(f'<{genre}><{movie_name}><Storyline><{storyline}>\n')
		write_file.flush()
	elif user == 7:
		print('Box office collection ---->> '+box_office_collection)
		write_file.write(f'<{genre}><{movie_name}><Box office collection><{box_office_collection}>\n')
		write_file.flush()
	elif user == 8:
		print('Runtime ---->> '+runtime)
		write_file.write(f'<{genre}><{movie_name}><Runtime><{runtime}>\n')
		write_file.flush()
	elif user == 9:
		print("Exiting!")
		break
	else:
		print('Invalid input!')

print('TASK2 DONE!')