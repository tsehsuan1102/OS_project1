import argparse
import os
import numpy as np

if __name__ == "__main__" :

	parser = argparse.ArgumentParser()
	parser.add_argument('theoretic_path')
	parser.add_argument('output_path')
	args = parser.parse_args()

	dir_list = os.listdir(args.theoretic_path)
	dir_list.sort()

	ans = []
	output = []

	for filename in dir_list:
		with open(os.path.join(args.theoretic_path, filename)) as f:
			for line in f:
				output.append(line.split()[0])
				#print(output)
		with open(os.path.join(args.output_path, filename)) as f:
			for line in f:
				ans.append(line.split()[0])
				#print(ans)
	if output == ans:
		print('Correct!')
	else:
		print('False!')






