import urllib.request, urllib.error, urllib.parse
import sys
import re

#----------------------------------------------------------------------------------------------------------
# --------------------------------------------TASK-1-------------------------------------------------------
#----------------------------------------------------------------------------------------------------------

url = []
url.append('https://www.rottentomatoes.com/top/bestofrt/top_100_action__adventure_movies/')
url.append('https://www.rottentomatoes.com/top/bestofrt/top_100_animation_movies/')
url.append('https://www.rottentomatoes.com/top/bestofrt/top_100_drama_movies/')
url.append('https://www.rottentomatoes.com/top/bestofrt/top_100_comedy_movies/')
url.append('https://www.rottentomatoes.com/top/bestofrt/top_100_mystery__suspense_movies/')
url.append('https://www.rottentomatoes.com/top/bestofrt/top_100_horror_movies/')
url.append('https://www.rottentomatoes.com/top/bestofrt/top_100_science_fiction__fantasy_movies/')
url.append('https://www.rottentomatoes.com/top/bestofrt/top_100_documentary_movies/')
url.append('https://www.rottentomatoes.com/top/bestofrt/top_100_romance_movies/')
url.append('https://www.rottentomatoes.com/top/bestofrt/top_100_classics_movies/')

for i in range(10):
	response = urllib.request.urlopen(url[i])
	webContent = response.read()
	genre1 = url[i][52:]
	genre1 = genre1[:-1]
	f = open(str(genre1)+'.html', 'wb')
	f.write(webContent)
	f.close

print('Enter <0> --> action__adventure_movies\nEnter <1> --> animation_movies\nEnter <2> --> drama_movies\nEnter <3> --> comedy_movies\nEnter <4> --> mystery__suspense_movies\nEnter <5> --> horror_movies\nEnter <6> --> science_fiction__fantasy_movies\nEnter <7> --> documentary_movies\nEnter <8> --> romance_movies\nEnter <9> --> classics_movies\n')
try:
	value = int(input())
except Exception as e:
	print(f'Error!, Reason: {e}')
genre1 = url[value][52:]
genre = genre1[:-1]
print('Top 100 movies of genre: ' + genre + '\n')

file = open(genre + '.html')

HTML_text = ''
for line in file:
	HTML_text += str(line)

movie_list = re.findall('<a href="/m/[\d]*[\w.-]+" class="unstyled articleLink">[\s\S]{4,150}</a>', HTML_text)

for movie in movie_list:
	before_keyword, keyword, after_keyword = movie.partition('\n')
	a, b, c = after_keyword.partition('<')
	a = a[12:]
	print(a)
print("\nEnter movie name (exactly as shown):")
flick = input()
print('You picked: '+flick)

for movie in movie_list:
	before_keyword, keyword, after_keyword = movie.partition('\n')
	a, b, c = after_keyword.partition('<')
	a = a[12:]
	if flick == a:
		x, y, z = before_keyword.partition('" class="')
		x = x[9:]
		url = 'https://www.rottentomatoes.com'+x
		response = urllib.request.urlopen(url)
		webContent = response.read()
		movie_name = url[33:]
		f = open('movie.html', 'wb')
		f.write(webContent)
		f.close
		break
print('HTML saved!\n\n')