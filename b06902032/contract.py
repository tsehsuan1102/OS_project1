import argparse
import os
import numpy as np

if __name__ == '__main__':
	
	parser = argparse.ArgumentParser()
	parser.add_argument('input_path')
	parser.add_argument('output_path')
	args = parser.parse_args()
	
	with open(os.path.join(args.output_path, 'TIME_MEASUREMENT_dmesg.txt'), 'r') as f:

		exec_times = [float(line.strip().split(' ')[3]) - float(line.strip().split(' ')[2]) for line in f]
		unit_time = sum(exec_times) / len(exec_times) / 500
	print('- -')
	print('| unit time | %f |' % unit_time)
	print('- -')

	dir_list = os.listdir(args.input_path)
	dir_list.sort()

	total = 0
	number = 0
	
	for filename in dir_list:
		print(filename)
		print('|process|start time|end time|expect exec time|my start time|my end time|my exec time|error rate|')
		print('|- |- |- |- |- |- |- |- |')
		
		theoretical = {}
		if os.path.isdir('theoretical'):

			with open(os.path.join('theoretical', filename), 'r') as f:

				for line in f:
					
					if args.print_theoretical == True:
						print(line.strip())
					theoretical[line.strip().split(' ')[0]] = int(line.strip().split(' ')[1])

		prefix = filename.split('.')[0]
		stdout_path = os.path.join(args.output_path, prefix + '_stdout.txt')
		dmesg_path = os.path.join(args.output_path, prefix + '_dmesg.txt')

		theoretical_time = []
		with open(os.path.join('theoretical-output', prefix + '_stdout.txt'), 'r') as f:
			for line in f:
				theoretical_time.append(line.strip()[1:])
		
		with open(stdout_path, 'r') as f:
		
			pid_to_name = {line.strip().split(' ')[1]: line.strip().split(' ')[0] for line in f}

		minor = np.inf
		with open(dmesg_path, 'r') as f:
			for line in f:
				lines = line.strip().split(' ')
				if minor > float(lines[2]):
					minor = float(lines[2])

		with open(dmesg_path, 'r') as f:
			count = 0
		
			for line in f:
				
				line_list = line.strip().split(' ')
				name = pid_to_name[line_list[1]]
				#print(theoretical[name])
				exec_time = float(line_list[3]) - float(line_list[2])
				subtract = float(theoretical_time[count].split()[2]) - float(theoretical_time[count].split()[1])

				print('|%s | %s | %s | %d | %0.2f | %0.2f | %0.2f | %0.2f%%|' % (name, theoretical_time[count].split()[1], theoretical_time[count].split()[2], float(theoretical_time[count].split()[2]) - float(theoretical_time[count].split()[1]), float(round((float(line_list[2]) - minor)/ unit_time, 3)), float(round((float(line_list[3]) - minor)/ unit_time, 3)), round(exec_time / unit_time, 3), 100*np.abs(subtract - round(exec_time / unit_time, 3)) / (subtract)))
				count += 1;
				total += 100*np.abs(subtract - round(exec_time / unit_time, 3)) / (subtract)
				number += 1

		

		print()
		theoretical_time = []

	print(total/number)
