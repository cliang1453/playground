import sys
import http.client
import csv
import os
from collections import defaultdict
import time


def load_similar(data, info, id_):

	if len(data.split("\"results\":[{")) < 2:
		return 

	header, data = data.split("\"results\":[{")
	ms = data.split('},{')
	
	for i in range(len(ms)):
		if i == 5:
			break

		for p in ms[i].split(','):
			
			if len(p.split('\":')) is not 2:
				continue
			
			name, val = p.split('\":')
			if name == "\"id":
				val = int(val)
				if val in info and id_ in info[val]:
					if id_ < val:
						info[id_].append(val)
						info[val].remove(id_)
				else:
					info[id_].append(val)
	

def load_comedy(data):

	info = []
	header, data = data.split("\"results\":[{")
	for m in data.split('},{'):
		
		pair = [None]*2
		for p in m.split(','):
			
			if len(p.split('\":')) is not 2:
				continue
			
			name, val = p.split('\":')

			if name == "\"id":
				pair[0] = val
			if name == "\"title":
				pair[1] = val.strip('\"')
			
			if pair[0] is not None and pair[1] is not None:
				info.append(pair)
				break

	assert len(data.split('},{')) == len(info)
	return info


def write_to_csv(data, output_file):

	if 'name' in output_file:
		with open(output_file, mode='w') as gf:
		    writer = csv.writer(gf)
		    for d in data:
		    	writer.writerow(d)
	else:
		with open(output_file, mode='w') as gf:
		    writer = csv.writer(gf)
		    for key, val in data.items():
		    	for v in val:
		    		writer.writerow([key,v])

def search_comedy(key, year = 2000, num_movies = 300, genre = 35, sort_method = "popularity.desc", output_file='movie_ID_name.csv'):
	
	conn = http.client.HTTPSConnection("api.themoviedb.org")
	page = 1
	info = []

	while len(info) < num_movies:
		payload = "{}"
		conn.request("GET", "/3/discover/movie?with_genres="+ str(genre)\
			+ "&primary_release_date.gte=" + str(year) + "-01-01"\
			+ "&page=" + str(page)\
			+ "&sort_by=" + sort_method\
			+ "&api_key=" + str(key), payload)
		res = conn.getresponse()
		data = res.read()
		info_ = load_comedy(data.decode("utf-8"))
		info.extend(info_)
		page += 1

	output_file = str(os.path.dirname('Q1')) + output_file
	write_to_csv(info, output_file)

	return info


def search_similar(key, movies, output_file='movie_ID_sim_movie_ID.csv'):

	conn = http.client.HTTPSConnection("api.themoviedb.org")
	info = defaultdict(list)
	cnt = 0

	for id_ ,_ in movies:
		
		payload = "{}"
		conn.request("GET", "/3/movie/" + id_ + "/similar?page=1"\
			+ "&api_key=" + str(key), payload)
		res = conn.getresponse()
		data = res.read()
		load_similar(data.decode("utf-8"), info, int(id_))

		if cnt%20 == 0:
			time.sleep(5)
		cnt += 1


	output_file = str(os.path.dirname('Q1')) + output_file
	write_to_csv(info, output_file)

	return info



def main(argv):
    if len(argv) != 2:
        raise ValueError("Usage: python3 %s  <API KEY>\n" % argv[0])

    API_key = argv[1]

    name_list = search_comedy(API_key)
    sim_list = search_similar(API_key, name_list)



if __name__ == '__main__':
    main(sys.argv)
