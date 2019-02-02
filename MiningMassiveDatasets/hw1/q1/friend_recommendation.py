'''
Algorithm: Let us use a simple algorithm such that, for each user U , the algorithm rec-
ommends N = 10 users who are not already friends with U , but have the most number of
mutual friends in common with U .
'''

import re
import sys
from pyspark import SparkConf, SparkContext
import itertools

def create_connections(person):
	"""
	input: 
		person: a tuple: (id, [list connected ids in ascending order])
	output: 
		result: a list of tuples
	"""
	if person[1][0] == '':
		return []

	person_id = person[0]
	friends_ids = [int(x) for x in person[1]]
	result = []

	# create existing connections
	for id in friends_ids:
		result.append(((min(person_id, id), max(person_id, id)), 0))

	# create potential connections
	for pair in itertools.combinations(friends_ids, 2):
		result.append(((min(pair), max(pair)), 1))

	return result


def main(argv):

	conf = SparkConf()
	sc = SparkContext(conf=conf)

	# Read files and format the file into  [(id, [list connected ids in ascending order]), (...), ...]
	friends_rdd = sc.textFile(argv[1])
	friends_final_rdd = friends_rdd.map(lambda line: line.split('	')) \
	                        .map(lambda oneFriend: (int(oneFriend[0]), oneFriend[1].split(',')))

	# create connections into [((id1, id2), 0), ((id3, id4), 1)]
	# 0: indicates id1 and id2 have been connected already
	# 1: indicates id3 and id4 have common friends and should be connected 
	connections_rdd = friends_final_rdd.flatMap(lambda person: create_connections(person))

	# count the number of common friends between each pair of id
	count_rdd = connections_rdd.filter(lambda pair: pair[1]==1)\
				   .reduceByKey(lambda n1, n2: n1 + n2)\
				   .sortBy(lambda pair: pair[1], ascending=False)

	# create recommendation for each individual: ((id1, id2), c) => (id1, (id2, c)) and (id2, (id1, c))
	recomm_rdd = count_rdd.flatMap(lambda pair: [(pair[0][0], (pair[0][1], pair[1])), (pair[0][1], (pair[0][0], pair[1]))])

	# create top n recommendation for each individual
	N = 10
	recomm_rdd = recomm_rdd.groupByKey()\
					.mapValues(lambda person: sorted(person, key = lambda x: x[1], reverse=True)[:N])\
					.map(lambda person: (person[0], [x[0] for x in person[1]]))


	# store result
	recomm_rdd.saveAsTextFile(sys.argv[2])
	sc.stop()


if __name__ == '__main__':
	main(sys.argv)

