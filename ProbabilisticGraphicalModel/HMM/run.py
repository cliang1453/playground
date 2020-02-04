from model import BioHmm
import sys

def run(args):
	train_data, test_data, mode = args[1], args[2], args[3]
	model = BioHmm(mode = int(mode))
	model.train(file_name = train_data)
	model.compute_emission_prob()
	model.compute_trigram_prob()
	model.predict(file_name = test_data)


if __name__ == '__main__':

	if len(sys.argv)!=4: # python3 train.py train_file mode test_file > output_file
		sys.exit(2)

	run(sys.argv)