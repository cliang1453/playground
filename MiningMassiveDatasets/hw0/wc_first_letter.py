'''
Write a Spark application which outputs the number of words that start with each
letter. This means that for every letter we want to count the total number of (non-
unique) words that start with that letter. In your implementation ignore the letter case,
i.e., consider all words as lower case. You can ignore all non-alphabetic characters.
'''

import re
import sys
from pyspark import SparkConf, SparkContext

conf = SparkConf()
sc = SparkContext(conf=conf)

# Read files
lines = sc.textFile(sys.argv[1])

# Read lines 
words = lines.flatMap(lambda l: re.split(r'[^\w]+', l))

# filter out empty words
words = words.filter(lambda w: w!="")

# map to lower case
words = words.map(lambda w: w.lower())

# filter out special characters
words = words.filter(lambda w: re.match(r'^[abcdefghijklmnopqrstuvwxyz]', w[0]))

# map to (first char, 1)
pairs = words.map(lambda w: (w[0], 1))

# count number of words started with the same character
counts = pairs.reduceByKey(lambda n1, n2: n1 + n2)

# store result
counts.saveAsTextFile(sys.argv[2])

sc.stop()