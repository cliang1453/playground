import re
import sys
from pyspark import SparkConf, SparkContext

conf = SparkConf()
sc = SparkContext(conf=conf)

# Next, you’ll need to read the target file into an RDD
# You now have an RDD filled with strings, one per line of the file.
lines = sc.textFile(sys.argv[1])


# Next you’ll want to split the lines into individual words
# first converts each line into an array of words, and then makes
# each of the words an element in the new RDD
words = lines.flatMap(lambda l: re.split(r'[^\w]+', l))

# Next, you’ll want to replace each word with a tuple of that word and the number 1
# The map() operation replaces each word with a tuple of that word and the number 1.
# The pairs RDD is a pair RDD where the word is the key, and all of the values are the number 1.
pairs = words.map(lambda w: (w, 1))

# to get a count of the number of instances of each word, you need only group the
# elements of the RDD by key (word) and add up their values:
# The reduceByKey() operation keeps adding elements’ values together until there are no
# more to add for each key (word).
counts = pairs.reduceByKey(lambda n1, n2: n1 + n2)

# Finally, you can store the results in a file and stop the context:
counts.saveAsTextFile(sys.argv[2])

sc.stop()