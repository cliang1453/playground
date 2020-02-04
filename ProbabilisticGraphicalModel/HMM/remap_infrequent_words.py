from count_freqs import simple_conll_corpus_iterator
import sys

def get_rare_words(corpus_file):

	corpus_file = open(sys.argv[1],"r")
	word_cnt = {}
	for word, ne_tag in simple_conll_corpus_iterator(corpus_file):
		if word not in word_cnt:
			word_cnt[word] = 0
		word_cnt[word] += 1

	rare_word = set([])
	for word, cnt in word_cnt.items():
		if cnt < 5:
			rare_word.add(word)

	return rare_word

def remap(file_name, rare_word):
	corpus_file = open(sys.argv[1],"r")
	output_file = []
	l = corpus_file.readline()
	while l:
		line = l.strip()
		if line:
			fields = line.split(" ")
			word = " ".join(fields[:-1])
			tag = fields[-1]

			if word in rare_word:
				print("_RARE_ " + tag)
			else:
				print(line)
		else:
			print("")
		l = corpus_file.readline()


if __name__ == '__main__':

	if len(sys.argv)!=2: # Expect exactly one argument: the training data file
		usage()
		sys.exit(2)

	try:
		input = open(sys.argv[1],"r")
	except IOError:
		sys.stderr.write("ERROR: Cannot read inputfile %s.\n" % arg)
		sys.exit(1)

	rare_word = get_rare_words(sys.argv[1])
	remap(sys.argv[1], rare_word)